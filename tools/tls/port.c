/* Socket transport for the private Mbed TLS 3.6 client. The SDK keeps using its
 * own 2.4 transport and ABI. SPDX-License-Identifier: MIT
 */
#include "wio_tls.h"
#include "lwip/sockets.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

/* The RTL8720 SDK's Rand()/rtw_get_random_bytes() are not treated as an entropy
 * source. SAMD51 provides a fresh 64-byte hardware-TRNG seed over the physical
 * eRPC UART. This DRBG is separate from every TLS connection's DRBG.
 */
static wio_mbedtls_ctr_drbg_context host_drbg;
static SemaphoreHandle_t host_mutex;
static int host_drbg_initialized;
static int allocator_configured;
static int host_seeded;
static unsigned int clock_epoch;
static TimeOut_t clock_start;
static unsigned char seed_input[64];
static size_t seed_available;
static size_t seed_offset;

static void *tls_calloc(size_t count, size_t size)
{
    size_t total;
    void *memory;
    if (size != 0 && count > (size_t) -1 / size) return NULL;
    total = count * size;
    if (total == 0) return NULL;
    memory = pvPortMalloc(total);
    if (memory != NULL) memset(memory, 0, total);
    return memory;
}

static int seed_entropy(void *unused, unsigned char *output, size_t length)
{
    (void) unused;
    if (length > seed_available) return WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    memcpy(output, seed_input + seed_offset, length);
    wio_mbedtls_platform_zeroize(seed_input + seed_offset, length);
    seed_offset += length;
    seed_available -= length;
    return 0;
}

static int ensure_mutex(void)
{
    SemaphoreHandle_t candidate;
    /* The RPC entry normally runs in one task. Protect lazy creation as well so
     * future independent TLS callers cannot create two entropy brokers.
     */
    if (host_mutex != NULL) return 1;
    candidate = xSemaphoreCreateMutex();
    if (candidate == NULL) return host_mutex != NULL;
    taskENTER_CRITICAL();
    if (host_mutex == NULL) {
        host_mutex = candidate;
        candidate = NULL;
    }
    taskEXIT_CRITICAL();
    if (candidate != NULL) vSemaphoreDelete(candidate);
    return host_mutex != NULL;
}

static uint64_t tick_time(void)
{
    TimeOut_t now;
    /* FreeRTOS reports its overflow counter atomically with the tick value, so
     * a disconnected device cannot accidentally refresh its clock at wrap.
     */
    vTaskSetTimeOutState(&now);
    return ((uint64_t) (unsigned int) now.xOverflowCount << (8 * sizeof(TickType_t))) + now.xTimeOnEntering;
}

static uint64_t clock_elapsed(void)
{
    uint64_t start = ((uint64_t) (unsigned int) clock_start.xOverflowCount << (8 * sizeof(TickType_t))) + clock_start.xTimeOnEntering;
    return tick_time() - start;
}

static int clock_is_fresh(void)
{
    return host_seeded && clock_elapsed() / configTICK_RATE_HZ < 6U * 60U * 60U;
}

wio_mbedtls_ms_time_t wio_mbedtls_ms_time(void)
{
    return (wio_mbedtls_ms_time_t) (tick_time() * 1000 / configTICK_RATE_HZ);
}

static wio_mbedtls_time_t private_time(wio_mbedtls_time_t *result)
{
    wio_mbedtls_time_t current = 0;
    if (host_mutex != NULL && xSemaphoreTake(host_mutex, portMAX_DELAY) == pdTRUE) {
        if (clock_is_fresh())
            current = (wio_mbedtls_time_t) (clock_epoch + clock_elapsed() / configTICK_RATE_HZ);
        xSemaphoreGive(host_mutex);
    }
    if (result != NULL) *result = current;
    return current;
}

int wio_tls_ready(void)
{
    int ready;
    if (!ensure_mutex()) return 0;
    if (xSemaphoreTake(host_mutex, portMAX_DELAY) != pdTRUE) return 0;
    ready = clock_is_fresh();
    xSemaphoreGive(host_mutex);
    return ready;
}

int wio_tls_provision(const unsigned char *seed, size_t length, unsigned int epoch)
{
    const unsigned char personalization[] = "Wio Terminal RPC TLS entropy broker v1";
    int result;
    unsigned char different = 0;
    size_t index;
    /* Reject obviously invalid requests. Statistical checks are not a
     * substitute for the host TRNG contract.
     */
    if (seed == NULL || length != sizeof(seed_input) ||
        epoch < 1704067200U || epoch > 2145916799U)
        return WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    for (index = 1; index < length; ++index) different |= seed[index] ^ seed[0];
    if (different == 0 || !ensure_mutex())
        return WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    if (xSemaphoreTake(host_mutex, portMAX_DELAY) != pdTRUE)
        return WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    if (!allocator_configured) {
        /* The SDK's newlib heap is only 4 KiB. Use FreeRTOS for every private
         * Mbed TLS allocation, including parsing a server certificate chain.
         * This is the namespaced platform API, not the SDK's 2.4 allocator.
         */
        wio_mbedtls_platform_set_calloc_free(tls_calloc, vPortFree);
        allocator_configured = 1;
    }
    host_seeded = 0;
    memcpy(seed_input, seed, length);
    seed_offset = 0;
    seed_available = length;
    if (!host_drbg_initialized) {
        wio_mbedtls_ctr_drbg_init(&host_drbg);
        result = wio_mbedtls_ctr_drbg_seed(&host_drbg, seed_entropy, NULL,
                                          personalization, sizeof(personalization) - 1);
        if (result == 0) {
            host_drbg_initialized = 1;
            /* The broker is explicitly reseeded from host TRNG data within
             * six hours. Never silently try to reuse its consumed seed buffer
             * at the generic 10000-request interval. Connection DRBGs retain
             * their upstream defaults and can request fresh broker output.
             */
            wio_mbedtls_ctr_drbg_set_reseed_interval(&host_drbg, INT_MAX - 1);
        }
        else wio_mbedtls_ctr_drbg_free(&host_drbg);
    } else {
        result = wio_mbedtls_ctr_drbg_reseed(&host_drbg, personalization,
                                            sizeof(personalization) - 1);
    }
    wio_mbedtls_platform_zeroize(seed_input, sizeof(seed_input));
    seed_available = seed_offset = 0;
    if (result == 0) {
        clock_epoch = epoch;
        vTaskSetTimeOutState(&clock_start);
        wio_mbedtls_platform_set_time(private_time);
        host_seeded = 1;
    }
    xSemaphoreGive(host_mutex);
    return result;
}

int wio_mbedtls_hardware_poll(void *unused, unsigned char *output, size_t length,
                             size_t *written)
{
    int result = WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    (void) unused;
    if (written == NULL || output == NULL) return result;
    *written = 0;
    if (!ensure_mutex()) return result;
    if (xSemaphoreTake(host_mutex, portMAX_DELAY) != pdTRUE) return result;
    if (clock_is_fresh()) {
        result = wio_mbedtls_ctr_drbg_random(&host_drbg, output, length);
        if (result == 0) *written = length;
        else host_seeded = 0; /* Explicit reprovision required after any failure. */
    }
    xSemaphoreGive(host_mutex);
    return result;
}

int wio_mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len)
{
    int fd = *(int *) ctx;
    int result;
    if (fd < 0) return WIO_MBEDTLS_ERR_NET_INVALID_CONTEXT;
    result = lwip_send(fd, buf, len, 0);
    if (result >= 0) return result;
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        return WIO_MBEDTLS_ERR_SSL_WANT_WRITE;
    if (errno == EPIPE || errno == ECONNRESET)
        return WIO_MBEDTLS_ERR_NET_CONN_RESET;
    return WIO_MBEDTLS_ERR_NET_SEND_FAILED;
}

int wio_mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
    int fd = *(int *) ctx;
    int result;
    if (fd < 0) return WIO_MBEDTLS_ERR_NET_INVALID_CONTEXT;
    result = lwip_recv(fd, buf, len, 0);
    if (result >= 0) return result;
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        return WIO_MBEDTLS_ERR_SSL_WANT_READ;
    if (errno == ECONNRESET) return WIO_MBEDTLS_ERR_NET_CONN_RESET;
    return WIO_MBEDTLS_ERR_NET_RECV_FAILED;
}
