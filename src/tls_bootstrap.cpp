#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "erpc_shim/rpc_system.h"

// Keep the modern TLS header/type namespace out of SDK C++ translation units.
extern "C" int wio_tls_provision(const unsigned char *seed, size_t length, unsigned int epoch);

extern "C" int32_t rpc_system_tls_provision(const uint8_t entropy[64], uint32_t epoch)
{
    if (entropy == NULL)
        return -1;
    uint8_t seed[64];
    memcpy(seed, entropy, sizeof(seed));
    const int32_t result = wio_tls_provision(seed, sizeof(seed), epoch);
    volatile uint8_t *wipe = seed;
    for (size_t i = 0; i < sizeof(seed); ++i)
        wipe[i] = 0;
    return result;
}
