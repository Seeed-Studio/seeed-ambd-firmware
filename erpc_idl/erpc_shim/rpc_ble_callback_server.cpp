/*
 * Generated by erpcgen 1.7.4 on Mon Oct 19 14:52:05 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "rpc_ble_callback_server.h"
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

static ManuallyConstructed<rpc_ble_callback_service> s_rpc_ble_callback_service;


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


//! @brief Function to write struct binary_t
static void write_binary_t_struct(erpc::Codec * codec, const binary_t * data);


// Write struct binary_t function implementation
static void write_binary_t_struct(erpc::Codec * codec, const binary_t * data)
{
    codec->writeBinary(data->dataLength, data->data);
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
erpc_status_t rpc_ble_callback_service::handleInvocation(uint32_t methodId, uint32_t sequence, Codec * codec, MessageBufferFactory *messageFactory)
{
    switch (methodId)
    {
        case krpc_ble_callback_rpc_ble_handle_gap_msg_id:
            return rpc_ble_handle_gap_msg_shim(codec, messageFactory, sequence);

        case krpc_ble_callback_rpc_ble_gap_callback_id:
            return rpc_ble_gap_callback_shim(codec, messageFactory, sequence);

        case krpc_ble_callback_rpc_ble_gattc_callback_id:
            return rpc_ble_gattc_callback_shim(codec, messageFactory, sequence);

        case krpc_ble_callback_rpc_ble_gatts_callback_id:
            return rpc_ble_gatts_callback_shim(codec, messageFactory, sequence);

        default:
            return kErpcStatus_InvalidArgument;
    }
}

// Server shim for rpc_ble_handle_gap_msg of rpc_ble_callback interface.
erpc_status_t rpc_ble_callback_service::rpc_ble_handle_gap_msg_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    binary_t *gap_msg = NULL;
    gap_msg = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (gap_msg == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    RPC_T_APP_RESULT result;

    // startReadMessage() was already called before this shim was invoked.

    read_binary_t_struct(codec, gap_msg);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_ble_handle_gap_msg(gap_msg);
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
        codec->startWriteMessage(kReplyMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_handle_gap_msg_id, sequence);

        codec->write(static_cast<int32_t>(result));

        err = codec->getStatus();
    }

    if (gap_msg)
    {
        free_binary_t_struct(gap_msg);
    }
    if (gap_msg)
    {
        erpc_free(gap_msg);
    }

    return err;
}

// Server shim for rpc_ble_gap_callback of rpc_ble_callback interface.
erpc_status_t rpc_ble_callback_service::rpc_ble_gap_callback_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint8_t cb_type;
    binary_t *cb_data = NULL;
    cb_data = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (cb_data == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    RPC_T_APP_RESULT result;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&cb_type);

    read_binary_t_struct(codec, cb_data);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_ble_gap_callback(cb_type, cb_data);
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
        codec->startWriteMessage(kReplyMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gap_callback_id, sequence);

        codec->write(static_cast<int32_t>(result));

        err = codec->getStatus();
    }

    if (cb_data)
    {
        free_binary_t_struct(cb_data);
    }
    if (cb_data)
    {
        erpc_free(cb_data);
    }

    return err;
}

// Server shim for rpc_ble_gattc_callback of rpc_ble_callback interface.
erpc_status_t rpc_ble_callback_service::rpc_ble_gattc_callback_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint8_t gatt_if;
    uint8_t conn_id;
    binary_t *cb_data = NULL;
    cb_data = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (cb_data == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    binary_t *extra_data = NULL;
    extra_data = (binary_t *) erpc_malloc(sizeof(binary_t));
    if (extra_data == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }
    RPC_T_APP_RESULT result;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&gatt_if);

    codec->read(&conn_id);

    read_binary_t_struct(codec, cb_data);

    read_binary_t_struct(codec, extra_data);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_ble_gattc_callback(gatt_if, conn_id, cb_data, extra_data);
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
        codec->startWriteMessage(kReplyMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gattc_callback_id, sequence);

        codec->write(static_cast<int32_t>(result));

        err = codec->getStatus();
    }

    if (cb_data)
    {
        free_binary_t_struct(cb_data);
    }
    if (cb_data)
    {
        erpc_free(cb_data);
    }

    if (extra_data)
    {
        free_binary_t_struct(extra_data);
    }
    if (extra_data)
    {
        erpc_free(extra_data);
    }

    return err;
}

// Server shim for rpc_ble_gatts_callback of rpc_ble_callback interface.
erpc_status_t rpc_ble_callback_service::rpc_ble_gatts_callback_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint8_t gatt_if;
    uint8_t conn_id;
    uint16_t attrib_index;
    RPC_T_SERVICE_CALLBACK_TYPE event;
    uint16_t property;
    binary_t *read_cb_data = NULL;
    binary_t *write_cb_data = NULL;
    binary_t *app_cb_data = NULL;
    bool isNull;
    int32_t _tmp_local;
    RPC_T_APP_RESULT result;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&gatt_if);

    codec->read(&conn_id);

    codec->read(&attrib_index);

    codec->read(&_tmp_local);
    event = static_cast<RPC_T_SERVICE_CALLBACK_TYPE>(_tmp_local);

    codec->read(&property);

    codec->readNullFlag(&isNull);
    if (!isNull)
    {
        read_cb_data = (binary_t *) erpc_malloc(sizeof(binary_t));
        if (read_cb_data == NULL)
        {
            codec->updateStatus(kErpcStatus_MemoryError);
        }
    }
    else
    {
        read_cb_data = NULL;
    }

    codec->readNullFlag(&isNull);
    if (!isNull)
    {
        write_cb_data = (binary_t *) erpc_malloc(sizeof(binary_t));
        if (write_cb_data == NULL)
        {
            codec->updateStatus(kErpcStatus_MemoryError);
        }
        read_binary_t_struct(codec, write_cb_data);
    }
    else
    {
        write_cb_data = NULL;
    }

    codec->readNullFlag(&isNull);
    if (!isNull)
    {
        app_cb_data = (binary_t *) erpc_malloc(sizeof(binary_t));
        if (app_cb_data == NULL)
        {
            codec->updateStatus(kErpcStatus_MemoryError);
        }
        read_binary_t_struct(codec, app_cb_data);
    }
    else
    {
        app_cb_data = NULL;
    }

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        result = rpc_ble_gatts_callback(gatt_if, conn_id, attrib_index, event, property, read_cb_data, write_cb_data, app_cb_data);
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
        codec->startWriteMessage(kReplyMessage, krpc_ble_callback_service_id, krpc_ble_callback_rpc_ble_gatts_callback_id, sequence);

        if (*read_cb_data != NULL)
        {
            write_binary_t_struct(codec, read_cb_data);
        }

        codec->write(static_cast<int32_t>(result));

        err = codec->getStatus();
    }

    if (read_cb_data)
    {
        free_binary_t_struct(read_cb_data);
    }
    if (read_cb_data)
    {
        erpc_free(read_cb_data);
    }

    if (write_cb_data)
    {
        free_binary_t_struct(write_cb_data);
    }
    if (write_cb_data)
    {
        erpc_free(write_cb_data);
    }

    if (app_cb_data)
    {
        free_binary_t_struct(app_cb_data);
    }
    if (app_cb_data)
    {
        erpc_free(app_cb_data);
    }

    return err;
}

erpc_service_t create_rpc_ble_callback_service()
{
    s_rpc_ble_callback_service.construct();
    return s_rpc_ble_callback_service.get();
}

void destroy_rpc_ble_callback_service()
{
    s_rpc_ble_callback_service.destroy();
}
