/* State tests for the real broker and private AES/CTR_DRBG implementation.
 * Only scheduling, ticks and sockets are simulated. Test seeds are deliberately
 * deterministic and must never be used by firmware. SPDX-License-Identifier: MIT
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "wio_tls.h"

static unsigned checks;
#define CHECK(expression) do { ++checks; if (!(expression)) { \
    fprintf(stderr, "check failed at line %d: %s\n", __LINE__, #expression); exit(1); \
} } while (0)

static uint64_t ticks;
static int critical_depth;
static int fail_mutex_create;
static int fail_take;
struct test_mutex { int held; };
static struct test_mutex mutex_storage;
static unsigned allocations;
void *pvPortMalloc(size_t length) {
    void *memory = malloc(length);
    if (memory != NULL) ++allocations;
    return memory;
}
void vPortFree(void *memory) {
    if (memory != NULL) { CHECK(allocations > 0); --allocations; }
    free(memory);
}

void test_enter_critical(void) { CHECK(critical_depth == 0); ++critical_depth; }
void test_exit_critical(void) { CHECK(critical_depth == 1); --critical_depth; }
void vTaskSetTimeOutState(TimeOut_t *state) {
    state->xOverflowCount = (BaseType_t)(ticks >> 32);
    state->xTimeOnEntering = (TickType_t)ticks;
}
SemaphoreHandle_t xSemaphoreCreateMutex(void) {
    CHECK(critical_depth == 0); /* Allocation may suspend/resume the scheduler. */
    return fail_mutex_create ? NULL : &mutex_storage;
}
BaseType_t xSemaphoreTake(SemaphoreHandle_t handle, TickType_t timeout) {
    (void)timeout;
    if (fail_take) return 0;
    CHECK(handle != NULL && !handle->held);
    handle->held = 1;
    return pdTRUE;
}
BaseType_t xSemaphoreGive(SemaphoreHandle_t handle) {
    CHECK(handle != NULL && handle->held);
    handle->held = 0;
    return pdTRUE;
}
void vSemaphoreDelete(SemaphoreHandle_t handle) { CHECK(handle != NULL); }

static int socket_result;
int lwip_send(int fd, const void *buffer, size_t length, int flags) {
    (void)fd; (void)buffer; (void)length; (void)flags;
    return socket_result;
}
int lwip_recv(int fd, void *buffer, size_t length, int flags) {
    (void)fd; (void)buffer; (void)length; (void)flags;
    return socket_result;
}

/* Inject failures at API boundaries while successful operations execute the
 * genuine Mbed TLS code. The firmware's implementation is included unchanged.
 */
static int fail_seed, fail_reseed, fail_random;
static int broker_seed(wio_mbedtls_ctr_drbg_context *ctx,
                       int (*entropy)(void *, unsigned char *, size_t), void *p,
                       const unsigned char *custom, size_t length) {
    if (fail_seed) return WIO_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    return wio_mbedtls_ctr_drbg_seed(ctx, entropy, p, custom, length);
}
static int broker_reseed(wio_mbedtls_ctr_drbg_context *ctx,
                         const unsigned char *custom, size_t length) {
    if (fail_reseed) return WIO_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    return wio_mbedtls_ctr_drbg_reseed(ctx, custom, length);
}
static int broker_random(void *ctx, unsigned char *output, size_t length) {
    if (fail_random) return WIO_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    return wio_mbedtls_ctr_drbg_random(ctx, output, length);
}
#define wio_mbedtls_ctr_drbg_seed broker_seed
#define wio_mbedtls_ctr_drbg_reseed broker_reseed
#define wio_mbedtls_ctr_drbg_random broker_random
#include "../port.c"
#undef wio_mbedtls_ctr_drbg_seed
#undef wio_mbedtls_ctr_drbg_reseed
#undef wio_mbedtls_ctr_drbg_random

static int all_zero(const unsigned char *bytes, size_t length) {
    unsigned char bits = 0;
    for (size_t i = 0; i < length; ++i) bits |= bytes[i];
    return bits == 0;
}

int main(void) {
    const unsigned int epoch = 1792425600U;
    const uint64_t lifetime = 6ULL * 60 * 60 * configTICK_RATE_HZ;
    unsigned char seed[64], next_seed[64], uniform[64] = { 0 };
    unsigned char output[32], previous[32];
    size_t written = 123;
    for (size_t i = 0; i < sizeof(seed); ++i) {
        seed[i] = (unsigned char)(i * 17U + 3U);
        next_seed[i] = (unsigned char)(i * 31U + 9U);
    }

    fail_mutex_create = 1;
    CHECK(!wio_tls_ready());
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch) != 0);
    CHECK(wio_mbedtls_hardware_poll(NULL, output, sizeof(output), &written) != 0 && written == 0);
    fail_mutex_create = 0;
    CHECK(!wio_tls_ready());
    CHECK(private_time(NULL) == 0);
    CHECK(wio_tls_provision(NULL, sizeof(seed), epoch) != 0);
    CHECK(wio_tls_provision(seed, sizeof(seed) - 1, epoch) != 0);
    CHECK(wio_tls_provision(seed, sizeof(seed), 1704067199U) != 0);
    CHECK(wio_tls_provision(seed, sizeof(seed), 2145916800U) != 0);
    CHECK(wio_tls_provision(uniform, sizeof(uniform), epoch) != 0);

    fail_seed = 1;
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch) != 0);
    CHECK(!wio_tls_ready() && !host_drbg_initialized);
    CHECK(seed_available == 0 && seed_offset == 0 && all_zero(seed_input, sizeof(seed_input)));
    fail_seed = 0;
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch) == 0);
    CHECK(wio_tls_ready());
    CHECK(private_time(NULL) == epoch);
    void *heap_probe = wio_mbedtls_calloc(4, 16);
    CHECK(heap_probe != NULL && all_zero(heap_probe, 64));
    wio_mbedtls_free(heap_probe);
    CHECK(wio_mbedtls_calloc(SIZE_MAX, 2) == NULL);
    CHECK(wio_mbedtls_calloc(0, 16) == NULL);
    CHECK(seed_available == 0 && seed_offset == 0 && all_zero(seed_input, sizeof(seed_input)));
    CHECK(wio_mbedtls_hardware_poll(NULL, previous, sizeof(previous), &written) == 0 && written == sizeof(previous));
    CHECK(!all_zero(previous, sizeof(previous)));
    CHECK(wio_mbedtls_hardware_poll(NULL, output, sizeof(output), &written) == 0 && written == sizeof(output));
    CHECK(memcmp(previous, output, sizeof(output)) != 0);

    /* Crossing the upstream 10000-call automatic-reseed interval must work;
     * the broker requires explicit fresh host entropy within six hours.
     */
    for (unsigned i = 0; i < 10005; ++i)
        CHECK(wio_mbedtls_hardware_poll(NULL, output, sizeof(output), &written) == 0);
    CHECK(wio_tls_ready());
    ticks = lifetime - 1;
    CHECK(wio_tls_ready());
    CHECK(private_time(NULL) == epoch + 6U * 60U * 60U - 1U);
    ticks = lifetime;
    CHECK(!wio_tls_ready() && private_time(NULL) == 0);
    CHECK(wio_mbedtls_hardware_poll(NULL, output, sizeof(output), &written) != 0 && written == 0);

    /* A valid fresh seed recovers an expired broker. Invalid input does not
     * destroy an already valid seed or reset its clock.
     */
    CHECK(wio_tls_provision(next_seed, sizeof(next_seed), epoch + 100) == 0);
    CHECK(wio_tls_ready());
    CHECK(wio_tls_provision(uniform, sizeof(uniform), epoch + 100) != 0);
    CHECK(wio_tls_ready() && private_time(NULL) == epoch + 100);
    fail_reseed = 1;
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch + 200) != 0);
    CHECK(!wio_tls_ready() && private_time(NULL) == 0);
    CHECK(seed_available == 0 && all_zero(seed_input, sizeof(seed_input)));
    fail_reseed = 0;
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch + 200) == 0);
    fail_random = 1;
    CHECK(wio_mbedtls_hardware_poll(NULL, output, sizeof(output), &written) != 0 && written == 0);
    CHECK(!wio_tls_ready());
    fail_random = 0;
    CHECK(wio_tls_provision(next_seed, sizeof(next_seed), epoch + 300) == 0);

    /* Keep elapsed time and expiry monotonic across both a tick rollover and
     * a complete extra 32-bit epoch of ticks while disconnected.
     */
    ticks = UINT32_MAX - 500U;
    CHECK(wio_tls_provision(seed, sizeof(seed), epoch + 400) == 0);
    ticks += 2000;
    CHECK(wio_tls_ready() && private_time(NULL) == epoch + 402);
    CHECK(wio_mbedtls_ms_time() == (wio_mbedtls_ms_time_t)ticks);
    ticks += 1ULL << 32;
    CHECK(!wio_tls_ready() && private_time(NULL) == 0);
    CHECK(wio_tls_provision(next_seed, sizeof(next_seed), epoch + 500) == 0);
    fail_take = 1;
    CHECK(!wio_tls_ready());
    CHECK(private_time(NULL) == 0);
    fail_take = 0;
    CHECK(wio_tls_ready());

    int socket = 3;
    socket_result = -1;
    errno = EAGAIN;
    CHECK(wio_mbedtls_net_send(&socket, output, 1) == WIO_MBEDTLS_ERR_SSL_WANT_WRITE);
    CHECK(wio_mbedtls_net_recv(&socket, output, 1) == WIO_MBEDTLS_ERR_SSL_WANT_READ);
    errno = ECONNRESET;
    CHECK(wio_mbedtls_net_send(&socket, output, 1) == WIO_MBEDTLS_ERR_NET_CONN_RESET);
    CHECK(wio_mbedtls_net_recv(&socket, output, 1) == WIO_MBEDTLS_ERR_NET_CONN_RESET);
    socket_result = 0;
    CHECK(wio_mbedtls_net_recv(&socket, output, 1) == 0);
    socket = -1;
    CHECK(wio_mbedtls_net_recv(&socket, output, 1) == WIO_MBEDTLS_ERR_NET_INVALID_CONTEXT);
    CHECK(!mutex_storage.held && critical_depth == 0);
    wio_mbedtls_ctr_drbg_free(&host_drbg);
    CHECK(allocations == 0);
    printf("broker-state: %u checks passed\n", checks);
    return 0;
}
