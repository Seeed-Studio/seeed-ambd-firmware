#ifndef WIO_TLS_BOOTSTRAP_HOST_H
#define WIO_TLS_BOOTSTRAP_HOST_H

#include <stdint.h>
#include <string.h>
#include <erpc/erpc_client_manager.h>
#include <erpc/erpc_codec.h>

// Provided by the existing Seeed_Arduino_rpcUnified library.
extern erpc::ClientManager *g_client;

namespace wio_tls_bootstrap {

enum : int32_t {
    UnsupportedFirmware = -32010,
    InvalidInput = -32011,
    ClientUnavailable = -32012,
    CodecUnavailable = -32013
};

// This method exists only in the custom firmware; do not probe an old peer with it.
inline bool supportsVersion(const char *firmwareVersion)
{
    return firmwareVersion != NULL && strcmp(firmwareVersion, "2.1.3-coloz-tls12.1") == 0;
}

// Read rpc_system_version() first. Entropy must be 64 fresh SAMD51 TRNG bytes;
// epoch must be current UTC obtained before TLS (for example, a checked NTP reply).
// Clear the caller's entropy array after the call. No seed bytes are logged here.
inline int32_t provision(const char *firmwareVersion, const uint8_t entropy[64], uint32_t epoch)
{
    if (!supportsVersion(firmwareVersion))
        return UnsupportedFirmware;
    if (entropy == NULL || epoch < 1704067200U || epoch > 2145916799U)
        return InvalidInput;
    if (g_client == NULL)
        return ClientUnavailable;

    const uint32_t serviceId = 1;
    const uint32_t methodId = 3;
    erpc::RequestContext request = g_client->createRequest(false);
    erpc::Codec *codec = request.getCodec();
    if (codec == NULL)
    {
        g_client->callErrorHandler(kErpcStatus_MemoryError, methodId);
        return CodecUnavailable;
    }

    int32_t result = ClientUnavailable;
    codec->startWriteMessage(erpc::kInvocationMessage, serviceId, methodId, request.getSequence());
    // Fixed uint8[64] arrays are encoded as 64 elements, without a list length.
    for (uint32_t i = 0; i < 64; ++i)
        codec->write(entropy[i]);
    codec->write(epoch);
    if (codec->getStatus() == kErpcStatus_Success)
        g_client->performRequest(request);
    if (codec->getStatus() == kErpcStatus_Success)
        codec->read(&result);
    const erpc_status_t err = codec->getStatus();
    g_client->releaseRequest(request);
    g_client->callErrorHandler(err, methodId);
    return err == kErpcStatus_Success ? result : -32100 - static_cast<int32_t>(err);
}

} // namespace wio_tls_bootstrap
#endif
