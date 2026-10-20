#pragma once
// Public certificate only. Its ephemeral private key was never saved.
// Deliberately unrelated to the real server CA; used only for rejection tests.
namespace wio_tls_bootstrap {
static const char UnrelatedTestCa[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDCjCCAfKgAwIBAgIJAOy3F3II2ZGjMA0GCSqGSIb3DQEBCwUAMDIxMDAuBgNVBAMTJ1dpbyBU\n"
    "TFMgVW5yZWxhdGVkIFRlc3QgUm9vdCBQdWJsaWMgT25seTAeFw0yNDAxMDEwMDAwMDBaFw0zNzEy\n"
    "MzEyMzU5NTlaMDIxMDAuBgNVBAMTJ1dpbyBUTFMgVW5yZWxhdGVkIFRlc3QgUm9vdCBQdWJsaWMg\n"
    "T25seTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANihejYF43tbKmJWqA314dFpDhN8\n"
    "HNQCdHczEg0ToIzgAx8Bw2Vqoa98NrOeuDL+vticl0s8SCXHAgZLuW74RlyQWlEtuJ+dyyba6VkZ\n"
    "KNntJFGZrC9NJpk/J9sDpqi8uDCkz7FX21enrJPfFDZ/aAfUmyYqA17tPsU2T/i5ypQ9BSEngDkg\n"
    "F9AMINit1IedmEv8g6d04t8HpmWt68ZibXDx/nAG2RPKWb9fT/m8z05QuiRwKuYqeLlSAYlrry0y\n"
    "YKvUQ7reIHl8y7kQ1XdU+rumieuWcBZT/WDS/zqYlFW5XTrYyL/p2o7UZKoggGk+Ejn3WKH7KLgS\n"
    "Y+vd3yntrHUCAwEAAaMjMCEwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwDQYJKoZI\n"
    "hvcNAQELBQADggEBAF4UE1kbjfAfW8X4e49yvMllfVN3irTzS8c0b1P2z6dZZM2IJUpkAfBjXaif\n"
    "2UzmSIyMRYROK9KkGpRMlf5BDD2xr5DbT2K54pVz9DaPrQWW2sTmGC1MVuefbYK9XhqotO+Y3jcJ\n"
    "F66LKlqXKMxq3qpsZElvQ8Pg5P4JbxtBWBlO63Srl2rBYgLdd+xcBuUwAxhoDXuJqAhDMxg1Q1mm\n"
    "YdSaXq2XkTByOp5dSfAeM4BDyZIElfiL5DF0m1y8Xy5hXKyFhQ5KfIjgJxvDwxwHZR4XSi6vwLU3\n"
    "gWOrSWkxpWr3icNsmSmnkIRWn/HpgsghwR/H66383e6/EoOuGITUUm8=\n"
    "-----END CERTIFICATE-----\n"
    ;

// Public ISRG Root X1 from the Node.js trusted root store.
static const char DomainTestRootCa[] =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n"
    ;
} // namespace wio_tls_bootstrap
