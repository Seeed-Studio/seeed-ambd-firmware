# Wio Terminal modern TLS firmware

The `2.1.3-coloz-tls12.1` build adds a private **Mbed TLS 3.6.7** client
for the existing `rpcWiFi` HTTPS interface. It enables TLS 1.2 with
ECDHE-RSA/ECDHE-ECDSA and AES-GCM, SNI, CA chain, hostname and certificate-date
verification. TLS 1.3 is not enabled in this configuration.

The [validation record](validation.md) includes native broker regression tests
and physical Wio Terminal checks for CA trust, hostname, certificate dates and
recovery after failed connections.

The vendor SDK's Mbed TLS 2.4 library and ROM interfaces remain available to
its closed Wi-Fi components. New TLS headers, macros, types and symbols use a
separate namespace. Do not globally replace `lib_mbedtls240.a` or select
`config_all.h` in the vendor SDK.

## Host initialization is required

This is not a drop-in update for applications that open TLS without preparing
entropy and time. The old SDK's pseudo-random functions are not used as a
cryptographic entropy source. Before the first HTTPS connection:

1. Connect Wi-Fi and obtain a current UTC epoch (for example, from a checked
   NTP reply). Ordinary NTP does not authenticate the time server.
2. Read `rpc_system_version()` and require `2.1.3-coloz-tls12.1`.
3. Generate 64 fresh hardware-TRNG bytes on the SAMD51.
4. Call `wio_tls_bootstrap::provision(version, seed, epoch)` and clear the seed.
5. Supply the appropriate CA to `WiFiClientSecure` and connect normally.

Copy `tools/host/WioTlsBootstrap.h` and `tools/host/WioSamd51Entropy.h` into
the host sketch. They use the existing Seeed RPC library; no global library
replacement is needed. The extension appends system service method 3, leaving
existing method IDs unchanged. Never send this method to stock firmware.

```cpp
#include <rpcWiFi.h>
#include <erpc/rpc_system.h>
#include <erpc/erpc_port.h>
#include "WioTlsBootstrap.h"
#include "WioSamd51Entropy.h"

// Run after Wi-Fi and UTC acquisition. utcEpoch is seconds since 1970.
bool initializeWirelessTls(uint32_t utcEpoch) {
    uint8_t seed[64] = {};
    char *version = rpc_system_version();
    bool ok = wio_tls_bootstrap::supportsVersion(version) &&
              wio_tls_bootstrap::samd51Entropy(seed);
    if (ok) ok = wio_tls_bootstrap::provision(version, seed, utcEpoch) == 0;
    wio_tls_bootstrap::clearEntropy(seed, sizeof(seed));
    if (version) erpc_free(version);
    return ok;
}
```

Provisioning expires after six hours. Refresh before expiry (for example,
every three hours), and provision again after a wireless-core reset.
Without a valid provision, or without CA/PSK credentials, new TLS connections
fail closed. Supported UTC epochs currently span 2024–2037 due to the SDK's
signed 32-bit `time_t`. [TLS configuration and port details](../tools/tls/README.md).

## Build on Windows

Use Arduino CLI, Node.js 20 or newer, Windows `curl.exe`, and `tar.exe`.
Keep the SDK, toolchain and build output in a task-specific directory. The
official package index is:

```text
https://files.seeedstudio.com/arduino/package_realtek.com_amebad_index.json
```

Install `realtek:AmebaD@3.0.5` through Arduino CLI using that index. It brings
`ameba_d_asdk_toolchain@1.0.1` and `ameba_d_tools@1.0.4`. The released Core alone
is insufficient: the RPC firmware requires Seeed's modified asynchronous TCP
structures and matching precompiled libraries. Reconcile it with the fixed
official Core commit, verifying every downloaded Git blob:

```powershell
node tools/build/Prepare-PinnedCore.mjs `
  --tree D:/build/core-tree.json `
  --core D:/build/data/packages/realtek/hardware/AmebaD/3.0.5 `
  --cache D:/build/core-blobs --apply
```

Obtain `core-tree.json` from the official GitHub API endpoint
`https://api.github.com/repos/Seeed-Studio/ArduinoCore-ambd/git/trees/f81bca75e433e35b2d27cadd178596888940fb5a?recursive=1`.
The synchronizer validates its identity and paths, stages verified differences,
and writes `.wio-pinned-core.json` only after checking all files.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/build/Build-WioWireless.ps1 `
  -ArduinoCli D:/tools/arduino-cli.exe `
  -ConfigFile D:/build/arduino-cli.yaml `
  -CoreDirectory D:/build/data/packages/realtek/hardware/AmebaD/3.0.5 `
  -ToolchainDirectory D:/build/data/packages/realtek/tools/ameba_d_asdk_toolchain/1.0.1 `
  -ToolsDirectory D:/build/data/packages/realtek/tools/ameba_d_tools/1.0.4 `
  -OutputDirectory D:/build/firmware -TlsDirectory D:/build/tls
```

The build downloads the pinned upstream Mbed TLS source archive and verifies
SHA256, builds its isolated library, compiles the firmware, checks linked TLS
symbols, and emits three images plus a manifest in `images/`. It never flashes.
Generated dependencies, private source copies, objects and logs are excluded
from Git. The manifest records compiler, source commit, SDK commit and hashes.

## Flash and recover

Build the firmware using the steps above, then use the three images and
manifest in `<OutputDirectory>/images/`. Compiled images, generated manifests
and intermediate build files are not checked into this source tree.

Use [the Windows flashing script](../tools/flash/README.md). It checks the three
images, manifest, target USB serial number and native-tool results, and preserves
logs. Do not run the vendor `erase` command before an ordinary update.

The updater temporarily replaces the SAMD51 application with a serial bridge.
After updating the RTL8720, re-upload the SAMD51 application containing the host
initialization above. Verify the reported wireless version, then test HTTPS with
the correct CA, an unrelated CA, and an invalid certificate date.

Retain the official non-JP v2.1.3 three-image package for recovery. The official
flashing utility cannot export the current device firmware; a downloaded stock
package is a recovery image, not a complete backup of the device's prior state.
Returning to stock wireless firmware also requires restoring a compatible
SAMD51 application.
