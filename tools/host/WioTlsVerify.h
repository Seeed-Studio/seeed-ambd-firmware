#pragma once

#include <Arduino.h>
#include <rpcWiFi.h>
#include <rpcWiFiClientSecure.h>
#include <erpc/erpc_port.h>
#include <erpc/rpc_system.h>
#include "WioTlsBootstrap.h"
#include "WioSamd51Entropy.h"
#include "WioTlsTestCa.h"

namespace wio_tls_bootstrap {
namespace verification_detail {

enum : int32_t {
    CertificateVerifyFailed = -0x2700,
    EntropyUnavailable = -32201,
    WifiUnavailable = -32203
};

inline void report(const char *name, bool ok, int32_t error, const char *expected)
{
    Serial.print("WIO_TLS_VERIFY name=");
    Serial.print(name);
    Serial.print(" ok=");
    Serial.print(ok ? 1 : 0);
    Serial.print(" error=");
    Serial.print(error);
    Serial.print(" expected=");
    Serial.println(expected);
}

inline bool freshProvision(const char *version, uint32_t epoch, const char *name)
{
    uint8_t entropy[64] = {};
    int32_t result = EntropyUnavailable;
    if (samd51Entropy(entropy))
        result = provision(version, entropy, epoch);
    clearEntropy(entropy, sizeof(entropy));
    report(name, result == 0, result, "fresh_trng_and_utc");
    return result == 0;
}

inline bool connection(const char *name, const char *ca, const char *host, bool expectSuccess)
{
    // Progress is emitted before each synchronous connect so a serial observer
    // can keep its UI responsive. Each TLS connect has its own 12-second limit.
    Serial.print("WIO_TLS_VERIFY_RUN name=");
    Serial.println(name);
    WiFiClientSecure client;
    client.setCACert(ca);
    const bool connected = client.connect(host, 443, 12000) == 1;
    char unusedMessage[100];
    const int32_t error = connected ? 0 : client.lastError(unusedMessage, sizeof(unusedMessage));
    client.stop();
    // A timeout or socket error does not prove that certificate checking works.
    const bool ok = expectSuccess ? connected : (!connected && error == CertificateVerifyFailed);
    report(name, ok, error, expectSuccess ? "connected" : "x509_failure");
    return ok;
}

} // namespace verification_detail

// Runs entirely through the RTL8720's rpcWiFi TLS client. It intentionally
// changes the peer clock for one negative test, then unconditionally restores
// fresh entropy and current UTC before any return from that portion of the test.
// Log output contains step names and status codes, never credentials or URLs.
inline bool verifyConnections(const char *rootCa, uint32_t currentEpoch,
                              const char *host = "t0.tianditu.gov.cn")
{
    using namespace verification_detail;
    if (rootCa == NULL || rootCa[0] == '\0' || host == NULL || host[0] == '\0' ||
        currentEpoch < 1704067200U || currentEpoch > 2145916799U)
    {
        report("input", false, InvalidInput, "valid_ca_host_and_utc");
        Serial.println("WIO_TLS_VERIFY_RESULT FAIL");
        return false;
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        report("wifi", false, WifiUnavailable, "connected");
        Serial.println("WIO_TLS_VERIFY_RESULT FAIL");
        return false;
    }

    char *version = rpc_system_version();
    if (!supportsVersion(version))
    {
        report("firmware", false, UnsupportedFirmware, "custom_tls_firmware");
        if (version != NULL) erpc_free(version);
        Serial.println("WIO_TLS_VERIFY_RESULT FAIL");
        return false;
    }

    Serial.println("WIO_TLS_VERIFY_BEGIN");
    bool coreOk = freshProvision(version, currentEpoch, "initial_provision");
    coreOk = connection("good_ca", rootCa, host, true) && coreOk;
    coreOk = connection("wrong_ca", UnrelatedTestCa, host, false) && coreOk;

    // Both controls use the same public root and certificate SAN set. A direct
    // IP probe cannot isolate client hostname checking when a virtual server
    // rejects its IP SNI before sending any certificate. These are independent
    // TLS handshakes; only an actual X.509 error satisfies the negative control.
    const bool domainPositive = connection("domain_good_host", DomainTestRootCa, "badssl.com", true);
    const bool domainNegative = connection("domain_wrong_host", DomainTestRootCa, "wrong.host.badssl.com", false);
    const bool domainOk = domainPositive && domainNegative;

    const bool oldTimeProvisioned = freshProvision(version, 1704067200U, "old_utc_provision");
    coreOk = oldTimeProvisioned && coreOk;
    if (oldTimeProvisioned)
        coreOk = connection("certificate_date", rootCa, host, false) && coreOk;
    else
        report("certificate_date", false, EntropyUnavailable, "x509_failure");

    // Do not short-circuit restoration when an earlier check has failed.
    const bool restored = freshProvision(version, currentEpoch, "restore_current_utc");
    coreOk = restored && coreOk;
    coreOk = connection("good_after_restore", rootCa, host, true) && coreOk;

    static const char *const wrongNames[] = { "repeat_wrong_ca_1", "repeat_wrong_ca_2", "repeat_wrong_ca_3" };
    static const char *const goodNames[] = { "repeat_good_ca_1", "repeat_good_ca_2", "repeat_good_ca_3" };
    for (unsigned round = 0; round < 3; ++round)
    {
        coreOk = connection(wrongNames[round], UnrelatedTestCa, host, false) && coreOk;
        coreOk = connection(goodNames[round], rootCa, host, true) && coreOk;
    }
    erpc_free(version);
    report("core_summary", coreOk, coreOk ? 0 : -1, "PASS");
    report("domain_summary", domainOk, domainOk ? 0 : -1, "PASS");
    const bool allOk = coreOk && domainOk;
    report("summary", allOk, allOk ? 0 : -1, "PASS");
    Serial.println(allOk ? "WIO_TLS_VERIFY_RESULT PASS" : "WIO_TLS_VERIFY_RESULT FAIL");
    return allOk;
}

} // namespace wio_tls_bootstrap
