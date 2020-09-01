/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_client.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-21
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/** Add Includes here **/
#include <bt_flags.h>
#include <trace_app.h>
#include <string.h>
#include <os_mem.h>
#include <ble_client.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
/** @brief  GAPS Client link table */
static uint8_t ble_client_link_num[BLE_CLIENT_MAX_APPS];
/**<  GATTS client ID. */
static T_CLIENT_ID ble_client_id[BLE_CLIENT_MAX_APPS];
/**<  Callback used to send data to app from ble_client client layer. */

static void ble_client_discover_state_cb0(uint8_t conn_id, T_DISCOVERY_STATE discovery_state)
{
    log_i("ble_client_discover_state_cb0: conn_id:%d discovery_state:%d ", conn_id, discovery_state);
    T_BLE_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = BLE_CLIENT_CB_TYPE_DISCOVERY_STATE;
    cb_data.app_id = 0;
    cb_data.cb_content.discov_state.state = discovery_state;

    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);
    
    return;
}

static void ble_client_discover_result_cb0(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type,
                                           T_DISCOVERY_RESULT_DATA result_data)
{
    log_i("ble_client_discover_result_cb0: conn_id: %d result_type:%d ", conn_id, result_type);
    T_BLE_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = BLE_CLIENT_CB_TYPE_DISCOVERY_RESULT;
    cb_data.app_id = 0;
    cb_data.cb_content.discov_result.discov_type = result_type;
    memcpy(&(cb_data.cb_content.discov_result.result), result_data.p_srv_disc_data, sizeof(T_BLE_CLIENT_DISCOV_RESULT_DATA));
    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);
    return;
}

static void ble_client_read_result_cb0(uint8_t conn_id, uint16_t cause,
                                       uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_BLE_CLIENT_CB_DATA cb_data;
    log_i("gcs_client_read_result_cb0: conn_id %d handle 0x%x, cause 0x%x", conn_id, handle, cause);

    cb_data.cb_type = BLE_CLIENT_CB_TYPE_READ_RESULT;
    cb_data.app_id = 0;
    cb_data.cb_content.read_result.cause = cause;
    cb_data.cb_content.read_result.handle = handle;
    cb_data.cb_content.read_result.value_size = value_size;
    cb_data.cb_content.read_result.p_value = p_value;

    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);
    return;
}

static void ble_client_write_result_cb0(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                        uint16_t handle, uint16_t cause,
                                        uint8_t credits)
{
    (void)credits;
    T_BLE_CLIENT_CB_DATA cb_data;
    log_i("ble_client_write_result_cb0: conn_id %d handle 0x%x, cause 0x%x", conn_id, handle, cause);

    cb_data.cb_type = BLE_CLIENT_CB_TYPE_WRITE_RESULT;
    cb_data.app_id = 0;
    cb_data.cb_content.write_result.cause = cause;
    cb_data.cb_content.write_result.handle = handle;
    cb_data.cb_content.write_result.type = type;

    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);
    return;
}

static T_APP_RESULT ble_client_notify_ind_cb0(uint8_t conn_id, bool notify,
                                              uint16_t handle,
                                              uint16_t value_size, uint8_t *p_value)
{
    log_i("ble_client_notify_ind_cb0: conn_id %d handle 0x%x, notify 0x%x", conn_id, handle, notify);
    T_BLE_CLIENT_CB_DATA cb_data;
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    cb_data.cb_type = BLE_CLIENT_CB_TYPE_NOTIF_IND;
    cb_data.app_id = 0;
    cb_data.cb_content.notif_ind.notify = notify;
    cb_data.cb_content.notif_ind.handle = handle;
    cb_data.cb_content.notif_ind.value_size = value_size;
    cb_data.cb_content.notif_ind.p_value = p_value;

    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);
    return app_result;
}

static void ble_client_disc_cb0(uint8_t conn_id)
{
    log_i("ble_client_disc_cb0: conn_id %d ", conn_id);
    T_BLE_CLIENT_CB_DATA cb_data;

    cb_data.cb_type = BLE_CLIENT_CB_TYPE_DISCONNECT_RESULT;
    cb_data.app_id = 0;
    cb_data.cb_content.disconn_result.conn_id = conn_id;

    ble_gatt_client_callback(ble_client_id[0], conn_id, &cb_data);

    return;
}

/**
 * @brief GATTS Client Callbacks.
*/
const T_FUN_CLIENT_CBS ble_client_cbs[BLE_CLIENT_MAX_APPS] = {
    {
        ble_client_discover_state_cb0,  //!< Discovery State callback function pointer
        ble_client_discover_result_cb0, //!< Discovery result callback function pointer
        ble_client_read_result_cb0,     //!< Read response callback function pointer
        ble_client_write_result_cb0,    //!< Write result callback function pointer
        ble_client_notify_ind_cb0,      //!< Notify Indicate callback function pointer
        ble_client_disc_cb0             //!< Link disconnection callback function pointer
    }

};

bool ble_client_init(uint8_t num)
{
    if (num > BLE_CLIENT_MAX_APPS)
    {
        return false;
    }
    for (uint8_t i = 0; i < BLE_CLIENT_MAX_APPS; i++)
    {
        ble_client_id[i] = CLIENT_PROFILE_GENERAL_ID;
        ble_client_link_num[i] = 0;
    }

    client_init(num);

    return true;
}

T_CLIENT_ID ble_add_client(uint8_t app_id, uint8_t link_num)
{
    if (link_num > BLE_LE_MAX_LINKS)
    {
        return 0xff;
    }
    if (app_id > BLE_CLIENT_MAX_APPS)
    {
        return 0xff;
    }

    ble_client_link_num[app_id] = link_num;

    if (false == client_register_spec_client_cb(&ble_client_id[app_id], &ble_client_cbs[app_id]))
    {
        ble_client_id[app_id] = CLIENT_PROFILE_GENERAL_ID;
        log_e("register fail");
        return ble_client_id[app_id];
    }

    log_i("client id %d", ble_client_id[app_id]);
    return ble_client_id[app_id];
}