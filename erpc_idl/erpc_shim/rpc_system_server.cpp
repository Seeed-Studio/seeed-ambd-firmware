/*
 * Generated by erpcgen 1.7.4 on Fri Dec 11 16:32:29 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "rpc_system_server.h"
#include <new>
#include "erpc_port.h"
#include "erpc_manually_constructed.h"

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif

using namespace erpc;
using namespace std;

#if ERPC_NESTED_CALLS_DETECTION
extern bool nestingDetection;
#endif

static ManuallyConstructed<rpc_system_service> s_rpc_system_service;



// Call the correct server shim based on method unique ID.
erpc_status_t rpc_system_service::handleInvocation(uint32_t methodId, uint32_t sequence, Codec * codec, MessageBufferFactory *messageFactory)
{
    switch (methodId)
    {
        case krpc_system_rpc_system_version_id:
            return rpc_system_version_shim(codec, messageFactory, sequence);

        case krpc_system_rpc_system_ack_id:
            return rpc_system_ack_shim(codec, messageFactory, sequence);

        default:
            return kErpcStatus_InvalidArgument;
    }
}

// Server shim for rpc_system_version of rpc_system interface.
erpc_status_t rpc_system_service::rpc_system_version_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    char * result = NULL;

    // startReadMessage() was already called before this shim was invoked.

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_system_version();
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, krpc_system_service_id, krpc_system_rpc_system_version_id, sequence);

        codec->writeString(strlen(result), result);

        err = codec->getStatus();
    }

    if (result)
    {
        erpc_free(result);
    }

    return err;
}

// Server shim for rpc_system_ack of rpc_system interface.
erpc_status_t rpc_system_service::rpc_system_ack_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint8_t c;
    uint8_t result;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&c);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_system_ack(c);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, krpc_system_service_id, krpc_system_rpc_system_ack_id, sequence);

        codec->write(result);

        err = codec->getStatus();
    }

    return err;
}

erpc_service_t create_rpc_system_service()
{
    s_rpc_system_service.construct();
    return s_rpc_system_service.get();
}

void destroy_rpc_system_service()
{
    s_rpc_system_service.destroy();
}
