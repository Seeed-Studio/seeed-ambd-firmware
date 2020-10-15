/*
 * Generated by erpcgen 1.7.4 on Wed Oct 14 16:15:46 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "erpc_client_manager.h"
#include "erpc_port.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_wifi_callback.h"
// import callbacks declaration from other groups
#include "rpc_ble_api.h"
#include "rpc_ble_callback.h"
#include "rpc_wifi_api.h"
}

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif

using namespace erpc;
using namespace std;

extern ClientManager *g_client;

//! @brief Function to write struct binary_t
static void write_binary_t_struct(erpc::Codec * codec, const binary_t * data);


// Write struct binary_t function implementation
static void write_binary_t_struct(erpc::Codec * codec, const binary_t * data)
{
    codec->writeBinary(data->dataLength, data->data);
}



// rpc_wifi_callback interface rpc_wifi_event_callback function client shim.
void rpc_wifi_event_callback(const binary_t * event)
{
    erpc_status_t err = kErpcStatus_Success;

    // Get a new request.
    RequestContext request = g_client->createRequest(true);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kOnewayMessage, krpc_wifi_callback_service_id, krpc_wifi_callback_rpc_wifi_event_callback_id, request.getSequence());

        write_binary_t_struct(codec, event);
        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);
        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, krpc_wifi_callback_rpc_wifi_event_callback_id);

    return;
}