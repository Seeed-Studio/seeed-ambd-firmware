#pragma once
#include <Arduino.h>
#include <stdint.h>

namespace wio_tls_bootstrap {
inline void clearEntropy(uint8_t *bytes, size_t length) {
    volatile uint8_t *cursor = bytes;
    while (length--) *cursor++ = 0;
}

// Wio Terminal's SAMD51 has a hardware TRNG. Never replace a timeout with
// random(), a device identifier, an ADC sample, or the current tick count.
inline bool samd51Entropy(uint8_t output[64]) {
#if defined(__SAMD51__)
    if (!output) return false;
    MCLK->APBCMASK.reg |= MCLK_APBCMASK_TRNG;
    TRNG->CTRLA.reg |= TRNG_CTRLA_ENABLE;
    uint32_t previous = 0;
    for (unsigned word = 0; word < 16; ++word) {
        const uint32_t started = millis();
        while (!(TRNG->INTFLAG.reg & TRNG_INTFLAG_DATARDY)) {
            if (uint32_t(millis() - started) >= 50) {
                clearEntropy(output, 64);
                return false;
            }
            delay(1);
        }
        const uint32_t value = TRNG->DATA.reg; // Reading DATA clears DATARDY.
        if (word && value == previous) {
            clearEntropy(output, 64);
            return false; // Detect a stuck peripheral; caller may retry later.
        }
        previous = value;
        for (unsigned byte = 0; byte < 4; ++byte)
            output[word * 4 + byte] = uint8_t(value >> (byte * 8));
    }
    return true;
#else
    (void)output;
    return false;
#endif
}
} // namespace wio_tls_bootstrap
