/*
 * Generated by erpcgen 1.7.4 on Wed Nov  4 11:07:47 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "erpc_client_manager.h"
#include "erpc_port.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_ble_callback.h"
// import callbacks declaration from other groups
#include "rpc_system.h"
#include "rpc_ble_api.h"
#include "rpc_wifi_api.h"
#include "rpc_wifi_callback.h"
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



// rpc_ble_callback interface rpc_ble_handle_gap_msg function client shim.
RPC_T_APP_RESULT rpc_ble_handle_gap_msg(const binary_t * gap_msg)
{
    erpc_status_t err = kErpcStatus_Success;

    RPC_T_APP_RESULT result;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_handle_gap_msg_id, request.getSequence());

        write_binary_t_struct(codec, gap_msg);

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        int32_t _tmp_local;

        codec->read(&_tmp_local);
        result = static_cast<RPC_T_APP_RESULT>(_tmp_local);

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, krpc_ble_callback_rpc_ble_handle_gap_msg_id);

    if (err)
    {
        return (RPC_T_APP_RESULT) -1;
    }

    return result;
}

// rpc_ble_callback interface rpc_ble_gap_callback function client shim.
RPC_T_APP_RESULT rpc_ble_gap_callback(uint8_t cb_type, const binary_t * cb_data)
{
    erpc_status_t err = kErpcStatus_Success;

    RPC_T_APP_RESULT result;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gap_callback_id, request.getSequence());

        codec->write(cb_type);

        write_binary_t_struct(codec, cb_data);

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        int32_t _tmp_local;

        codec->read(&_tmp_local);
        result = static_cast<RPC_T_APP_RESULT>(_tmp_local);

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, krpc_ble_callback_rpc_ble_gap_callback_id);

    if (err)
    {
        return (RPC_T_APP_RESULT) -1;
    }

    return result;
}

// rpc_ble_callback interface rpc_ble_gattc_callback function client shim.
RPC_T_APP_RESULT rpc_ble_gattc_callback(uint8_t gatt_if, uint8_t conn_id, const binary_t * cb_data, const binary_t * extra_data)
{
    erpc_status_t err = kErpcStatus_Success;

    RPC_T_APP_RESULT result;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gattc_callback_id, request.getSequence());

        codec->write(gatt_if);

        codec->write(conn_id);

        write_binary_t_struct(codec, cb_data);

        write_binary_t_struct(codec, extra_data);

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        int32_t _tmp_local;

        codec->read(&_tmp_local);
        result = static_cast<RPC_T_APP_RESULT>(_tmp_local);

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, krpc_ble_callback_rpc_ble_gattc_callback_id);

    if (err)
    {
        return (RPC_T_APP_RESULT) -1;
    }

    return result;
}

// rpc_ble_callback interface rpc_ble_gatts_callback function client shim.
RPC_T_APP_RESULT rpc_ble_gatts_callback(uint8_t gatt_if, uint8_t conn_id, uint16_t attrib_index, RPC_T_SERVICE_CALLBACK_TYPE event, uint16_t property, binary_t * read_cb_data, const binary_t * write_cb_data, const binary_t * app_cb_data)
{
    erpc_status_t err = kErpcStatus_Success;

    RPC_T_APP_RESULT result;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gatts_callback_id, request.getSequence());

        codec->write(gatt_if);

        codec->write(conn_id);

        codec->write(attrib_index);

        codec->write(static_cast<int32_t>(event));

        codec->write(property);

        if (read_cb_data == NULL)
        {
            codec->writeNullFlag(true);
        }
        else
        {
            codec->writeNullFlag(false);
        }

        if (write_cb_data == NULL)
        {
            codec->writeNullFlag(true);
        }
        else
        {
            codec->writeNullFlag(false);
            write_binary_t_struct(codec, write_cb_data);
        }

        if (app_cb_data == NULL)
        {
            codec->writeNullFlag(true);
        }
        else
        {
            codec->writeNullFlag(false);
            write_binary_t_struct(codec, app_cb_data);
        }

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        int32_t _tmp_local;

        if (read_cb_data != NULL)
        {
            read_binary_t_struct(codec, read_cb_data);
        }

        codec->read(&_tmp_local);
        result = static_cast<RPC_T_APP_RESULT>(_tmp_local);

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, krpc_ble_callback_rpc_ble_gatts_callback_id);

    if (err)
    {
        return (RPC_T_APP_RESULT) -1;
    }

    return result;
}
