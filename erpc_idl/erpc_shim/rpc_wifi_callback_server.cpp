/*
 * Generated by erpcgen 1.7.4 on Thu Oct 29 10:00:48 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "rpc_wifi_callback_server.h"
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

static ManuallyConstructed<rpc_wifi_callback_service> s_rpc_wifi_callback_service;


//! @brief Function to read struct binary_t
static void read_binary_t_struct(erpc::Codec * codec, binary_t * data);


// Read struct binary_t function implementation
static void read_binary_t_struct(erpc::Codec * codec, binary_t * data)
{
    uint8_t * data_local;
    codec->readBinary(&data->dataLength, &data_local);
    data->data = (uint8_t *) erpc_malloc(data->dataLength * sizeof(uint8_t));
    if (data->data == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    else
    {
        memcpy(data->data, data_local, data->dataLength);
    }
}


//! @brief Function to free space allocated inside struct binary_t
static void free_binary_t_struct(binary_t * data);


// Free space allocated inside struct binary_t function implementation
static void free_binary_t_struct(binary_t * data)
{
    if (data->data)
    {
        erpc_free(data->data);
    }
}



// Call the correct server shim based on method unique ID.
erpc_status_t rpc_wifi_callback_service::handleInvocation(uint32_t methodId, uint32_t sequence, Codec * codec, MessageBufferFactory *messageFactory)
{
    switch (methodId)
    {
        case krpc_wifi_callback_rpc_wifi_event_callback_id:
            return rpc_wifi_event_callback_shim(codec, messageFactory, sequence);

        case krpc_wifi_callback_rpc_wifi_dns_found_id:
            return rpc_wifi_dns_found_shim(codec, messageFactory, sequence);

        default:
            return kErpcStatus_InvalidArgument;
    }
}

// Server shim for rpc_wifi_event_callback of rpc_wifi_callback interface.
erpc_status_t rpc_wifi_callback_service::rpc_wifi_event_callback_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    binary_t *event = NULL;
    event = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (event == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }

    // startReadMessage() was already called before this shim was invoked.

    read_binary_t_struct(codec, event);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        rpc_wifi_event_callback(event);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif
    }

    if (event)
    {
        free_binary_t_struct(event);
    }
    if (event)
    {
        erpc_free(event);
    }

    return err;
}

// Server shim for rpc_wifi_dns_found of rpc_wifi_callback interface.
erpc_status_t rpc_wifi_callback_service::rpc_wifi_dns_found_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    char * hostname = NULL;
    binary_t *ipaddr = NULL;
    ipaddr = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (ipaddr == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    binary_t *arg = NULL;
    bool isNull;

    // startReadMessage() was already called before this shim was invoked.

    uint32_t hostname_len;
    char * hostname_local;
    codec->readString(&hostname_len, &hostname_local);
    hostname = (char *) erpc_malloc((hostname_len + 1) * sizeof(char));
    if (hostname == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    else
    {
        memcpy(hostname, hostname_local, hostname_len);
        (hostname)[hostname_len] = 0;
    }

    read_binary_t_struct(codec, ipaddr);

    codec->readNullFlag(&isNull);
    if (!isNull)
    {
        arg = (binary_t *) erpc_malloc(sizeof(binary_t));
        if (arg == NULL)
        {
            codec->updateStatus(kErpcStatus_MemoryError);
        }
        read_binary_t_struct(codec, arg);
    }
    else
    {
        arg = NULL;
    }

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        rpc_wifi_dns_found(hostname, ipaddr, arg);
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
        codec->startWriteMessage(kReplyMessage, krpc_wifi_callback_service_id, krpc_wifi_callback_rpc_wifi_dns_found_id, sequence);

        err = codec->getStatus();
    }

    if (hostname)
    {
        erpc_free(hostname);
    }

    if (ipaddr)
    {
        free_binary_t_struct(ipaddr);
    }
    if (ipaddr)
    {
        erpc_free(ipaddr);
    }

    if (arg)
    {
        free_binary_t_struct(arg);
    }
    if (arg)
    {
        erpc_free(arg);
    }

    return err;
}

erpc_service_t create_rpc_wifi_callback_service()
{
    s_rpc_wifi_callback_service.construct();
    return s_rpc_wifi_callback_service.get();
}

void destroy_rpc_wifi_callback_service()
{
    s_rpc_wifi_callback_service.destroy();
}
