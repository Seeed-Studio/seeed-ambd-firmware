/*
 * Generated by erpcgen 1.7.4 on Fri Sep 18 15:38:09 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_ble_callback_h_)
#define _rpc_ble_callback_h_

// Common types header file
#include "rpc_system_header.h"

/*! @brief rpc_ble_callback identifiers */
enum _rpc_ble_callback_ids
{
    krpc_ble_callback_service_id = 12,
    krpc_ble_callback_rpc_ble_handle_gap_msg_id = 1,
    krpc_ble_callback_rpc_ble_gap_callback_id = 2,
    krpc_ble_callback_rpc_ble_gattc_callback_id = 3,
    krpc_ble_callback_rpc_ble_gatts_callback_id = 4,
};

#if defined(__cplusplus)
extern "C" {
#endif

//! @name rpc_ble_callback
//@{
RPC_T_APP_RESULT rpc_ble_handle_gap_msg(const binary_t * gap_msg);

RPC_T_APP_RESULT rpc_ble_gap_callback(uint8_t cb_type, const binary_t * cb_data);

RPC_T_APP_RESULT rpc_ble_gattc_callback(uint8_t gatt_if, uint8_t conn_id, const binary_t * cb_data, const binary_t * extra_data);

RPC_T_APP_RESULT rpc_ble_gatts_callback(uint8_t gatt_if, uint8_t conn_id, uint16_t attrib_index, RPC_T_SERVICE_CALLBACK_TYPE event, uint16_t property, binary_t * read_cb_data, const binary_t * write_cb_data, const binary_t * app_cb_data);
//@}

#if defined(__cplusplus)
}
#endif

#endif // _rpc_ble_callback_h_
