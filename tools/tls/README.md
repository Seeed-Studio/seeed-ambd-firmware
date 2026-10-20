# RPC TLS library

The RPC HTTPS client uses **Mbed TLS 3.6.7**, the maintained 3.6 LTS branch.
The SDK's old Mbed TLS 2.4 archive and ROM bindings remain available to the
closed Wi-Fi/EAP/Bluetooth components. They must not be replaced globally:
their private context layouts are incompatible with Mbed TLS 3.6.

The private library uses overflow-checked FreeRTOS allocations configured by
the first bootstrap call. It does not consume the SDK's small newlib heap for
TLS record buffers or certificate parsing.

`build.mjs` downloads the [official source release](https://github.com/Mbed-TLS/mbedtls/releases/tag/mbedtls-3.6.7),
checks its published SHA256, and deterministically prefixes its headers,
macros, types and external symbols with `wio_`. No ROM crypto/`*_ALT` library
from the old SDK is used by this client. The generated `wio_tls.h` is the only
public include; `src/wifi/wifi_ssl_client.h` keeps TLS contexts opaque.

## Build

Use Node.js 20 or newer, `tar`, `curl.exe`, and the official Realtek AmebaD
toolchain. On Windows, from the repository root:

```powershell
node tools/tls/build.mjs `
  --toolchain "D:/path/to/ameba_d_asdk_toolchain/1.0.1/bin" `
  --core "D:/path/to/realtek/hardware/AmebaD/3.0.5" `
  --out "D:/build/wio-tls"
```

`--archive <file>` reuses a downloaded source tarball while still validating
its SHA256. `--source <directory>` is a development-only escape hatch without
archive validation; do not use it to produce release firmware.

The output is `lib_wio_mbedtls367.a`, `include/wio_tls.h`, a symbol inventory
and a manifest recording source, configuration, port, compiler and archive
hashes. Link the new archive alongside the original SDK libraries. The build
rejects escaped legacy TLS symbols and unresolved private TLS symbols.

## Capabilities

- TLS 1.2 client, SNI and hostname verification.
- ECDHE-RSA and ECDHE-ECDSA with AES-128/256-GCM and SHA-256/384.
- P-256/P-384 curves, RSA PKCS#1 v1.5/PSS certificate signatures.
- CA chain and validity-date verification; certificates remain mandatory for
  certificate-based HTTPS connections.
- A separate PSK-GCM path for existing PSK applications.
- 16 KiB incoming / 4 KiB outgoing plaintext record buffers. Servers do not
  need to implement the optional maximum-fragment-length extension.

TLS 1.3, static RSA key exchange, CBC suites, MD5 and SHA-1 signatures are not
enabled. The old SDK's internal protocols are outside this private TLS build.

## Entropy and time

The old SDK's `Rand()`, `rtw_get_random_bytes()` and `Gen_RandomSeed()` are
**not** treated as cryptographic entropy. The SAMD51 host must provision a
fresh 64-byte hardware-TRNG seed and a valid UTC epoch through the additional
RPC bootstrap method before opening TLS connections.

`wio_tls_provision(seed, 64, epoch)` initializes or reseeds a private entropy
broker using AES-256 CTR_DRBG. Individual connections retain their own entropy
and CTR_DRBG contexts. The input seed is consumed and zeroized; it is never
printed, persisted in the firmware image, or replaced with tick-based noise.
`wio_tls_ready()` returns false until provisioning succeeds.

The private UTC clock advances from the host epoch using FreeRTOS ticks and
its overflow counter. The provision expires after six hours, and an expired
or missing provision fails closed. The host should refresh it before expiry.
The epoch currently accepts 2024 through 2037 because the bundled SDK uses
signed 32-bit `time_t`. This clock does not change the SDK's global clock.

As with any device supplied time, the host is responsible for the source of
UTC. A plain NTP response is not cryptographically authenticated.

Upstream sources retain their Apache-2.0 / GPL-2.0-or-later license notices;
the official archive contains the full licenses. The local port and builder
are MIT licensed, and the configuration is Apache-2.0 licensed.
