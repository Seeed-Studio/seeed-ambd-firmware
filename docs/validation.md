# TLS validation record

Recorded on 2026-10-20 (Asia/Shanghai) for wireless firmware
`2.1.3-coloz-tls12.1`. This record distinguishes native tests, PC network
checks and observations from a physical Wio Terminal. It does not certify
every application or establish long-term heap stability.

The HTTPS client tested here runs on the RTL8720 through `rpcWiFiClientSecure`.
The SAMD51 supplies hardware-TRNG entropy and UTC; it does not run a replacement
software TLS client. The RPC client uses the private Mbed TLS 3.6.7 library.
The final physical-device run passed both the core certificate tests and the
independent hostname controls, ending with `WIO_TLS_VERIFY_RESULT PASS`.

## Confirmed results

| Check | Environment | Observed result |
| --- | --- | --- |
| TLS client compilation | Realtek ASDK 6.5 GCC, Cortex-M33 | Passed with `-Wall -Wextra -Werror`. |
| TLS ABI isolation | Undefined-symbol inspection of the client object | Modern calls use `wio_mbedtls_*`; no legacy `mbedtls_*` function references. |
| Entropy/UTC broker regression | Native Zig 0.15.2, actual port and private AES/CTR_DRBG | **30144 checks passed**. Only RTOS, tick and socket interfaces are simulated; selected failure results are injected. |
| Host verification helper compilation | Seeed SAMD Wio Terminal target | Standalone diagnostic sketch compiled and linked with the verification function referenced by a serial command. |
| Correct CA | Physical Wio Terminal | TLS connection succeeded. |
| Unrelated, syntactically valid CA | Physical Wio Terminal | Rejected with `-9984` (`-0x2700`, X.509 verification failure). |
| Incorrect UTC, set to 2024-01-01 | Physical Wio Terminal | Correct-CA connection rejected with `-9984`. |
| Current UTC and fresh entropy restored | Physical Wio Terminal | Provisioning and correct-CA connection succeeded after the date test. |
| Three alternating wrong-CA/correct-CA rounds | Physical Wio Terminal | Every wrong-CA connection returned `-9984`; every following correct-CA connection succeeded. |
| Same-root hostname positive control | PC Node.js, TLS 1.2, `ECDHE-RSA-AES128-GCM-SHA256` | `badssl.com` succeeded using ISRG Root X1. |
| Same-root hostname negative control | PC Node.js, identical protocol/cipher/CA | `wrong.host.badssl.com` failed specifically with `ERR_TLS_CERT_ALTNAME_INVALID`. |
| Independent hostname positive/negative controls | Physical Wio Terminal | `badssl.com` connected successfully; `wrong.host.badssl.com`, using the same CA, was rejected with `-9984`. |
| Complete diagnostic matrix | Physical Wio Terminal | `core_summary`, `domain_summary` and `summary` all reported `ok=1`; final result **PASS**. |

The native broker tests cover unavailable mutexes, invalid seed/epoch inputs,
initial seed failure and recovery, reseed/random failures and recovery, seed
buffer clearing, more than 10000 output requests, the exact six-hour expiry
boundary, tick rollover and an additional complete tick-counter period. They
also check RTOS-backed allocation, multiplication overflow, zero initialization
and socket error translation. They are state/algorithm tests, not a concurrent
RTOS stress test.

The three device retry rounds show recovery after failed connections. They do
not, by themselves, prove absence of a leak over extended operation.

## Hostname test design

Hostname validation uses two independent TLS handshakes with the same ISRG Root
X1 trust anchor: `badssl.com` must connect, and `wrong.host.badssl.com` must fail
with `-9984`. Direct-IP probing is unsuitable for this check because a virtual
server can reject IP SNI with a fatal alert before sending a certificate; that
server-side rejection cannot establish successful client hostname validation.

PC preflight confirmed that the endpoint certificate has SAN entries
`*.badssl.com` and `badssl.com`, and that the wrong-host failure is specifically
a hostname mismatch. At the time of preflight its issuer was Let's Encrypt YR1
and its leaf certificate expired on 2026-12-28. The test root is the public ISRG
Root X1 from Node's trusted root store, valid through 2035-06-04. Public service
certificates can change; device results must be assessed from the actual run.

Serial output reports `core_summary` and `domain_summary` separately. The final
device run produced:

```text
WIO_TLS_VERIFY name=core_summary ok=1 error=0 expected=PASS
WIO_TLS_VERIFY name=domain_summary ok=1 error=0 expected=PASS
WIO_TLS_VERIFY name=summary ok=1 error=0 expected=PASS
WIO_TLS_VERIFY_RESULT PASS
```

The final result is PASS only when both groups pass. Network timeouts, socket
errors and fatal alerts do not count as successful X.509 negative tests. The
full final serial capture is retained separately as
`modern-tls-final-certificate-checks.log`.

## Reproduce

After generating the private TLS source/header tree with the
[build workflow](modern-tls.md), run the broker regression with an available
native Zig compiler:

```powershell
node tools/tls/tests/run-broker.mjs `
  --zig D:/tools/zig.exe `
  --tls-build D:/build/tls `
  --out D:/build/broker-tests
```

For the device checks, copy the four files from `tools/host/` into a SAMD51
diagnostic application, include `WioTlsVerify.h`, establish Wi-Fi and obtain
current UTC, then invoke:

```cpp
bool passed = wio_tls_bootstrap::verifyConnections(rootCa, currentEpoch);
```

Use the correct public root for the target endpoint; the optional third
argument selects its hostname. Each connection requests a 12-second TLS
timeout and emits a progress record before starting. Capture the full serial
run, including both group summaries and `WIO_TLS_VERIFY_RESULT`.

The date negative test deliberately changes the wireless clock. The helper
always attempts fresh-TRNG/current-UTC restoration after that portion of the
test, even if an earlier assertion failed. A failed restoration is itself a
failed check and must not be ignored before normal application use.

## Public-header review

The four `tools/host/` headers were inspected for embedded credentials and
private key material:

| Header | Public contents |
| --- | --- |
| `WioSamd51Entropy.h` | SAMD51 TRNG access and volatile clearing of runtime entropy. |
| `WioTlsBootstrap.h` | Public firmware/version identifiers, RPC method IDs and runtime provision parameters. No seed bytes are logged. |
| `WioTlsTestCa.h` | Two public certificate PEMs: an unrelated test CA and ISRG Root X1. No private key PEM. The unrelated CA's temporary private key was kept only in process memory during certificate generation. |
| `WioTlsVerify.h` | Public test hostnames, expected errors and diagnostic code. Runtime seed arrays are cleared; logs contain step names/status codes rather than credentials or request URLs. |

No Wi-Fi SSID/password, Tianditu access key, fixed production entropy seed or
private key is embedded in these headers. Application credentials remain the
responsibility of the calling application and are not needed in this firmware
repository's diagnostic headers.
