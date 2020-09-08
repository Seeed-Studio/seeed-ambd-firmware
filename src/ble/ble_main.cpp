
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_main.c
   * @brief     Source file for BLE project, mainly used for initialize modules
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_drv.h"
#include "trace_app.h"
#include "bte.h"
#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "os_task.h"
#include "os_msg.h"
#include "app_msg.h"
#include "rtk_coex.h"
#include "profile_server.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif

#include "ble_main.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

T_GAP_DEV_STATE ble_gap_dev_state = {0, 0, 0, 0, 0}; /**< GAP device state */
T_GAP_CONN_STATE ble_gap_conn_state = GAP_CONN_STATE_DISCONNECTED;
T_APP_LINK ble_clinet_link_table[BLE_LE_MAX_LINKS];
T_GAP_ROLE ble_dev_role = GAP_LINK_ROLE_SLAVE; // 0:close 1:server 2:client

bool ble_init()
{
  log_v("ble_init");

  //int bt_stack_already_on = 0;
  //(void) bt_stack_already_on;
  T_GAP_DEV_STATE new_state;

  if (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE)))
  {
    wiFiDrv.wifiDriverInit();
  }

  /*Wait WIFI init complete*/
  while (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE)))
  {
    vTaskDelay(1000 / portTICK_RATE_MS);
  }

  //judge BLE central is already on
  le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
  if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
  {
    //bt_stack_already_on = 1;
    log_d("[BLE Central]BT Stack already on");
    return false;
  }
  else
  {
    bte_init();
  }

  gap_config_max_le_link_num(BLE_LE_MAX_LINKS);
  le_gap_init(BLE_LE_MAX_LINKS);

  /* Device name and device appearance */
  uint8_t device_name[GAP_DEVICE_NAME_LEN] = "Wio Terminal";
  uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
  uint8_t slave_init_mtu_req = false;

  /* Scan parameters */
  uint8_t scan_mode = GAP_SCAN_MODE_ACTIVE;
  uint16_t scan_interval = DEFAULT_SCAN_INTERVAL;
  uint16_t scan_window = DEFAULT_SCAN_WINDOW;
  uint8_t scan_filter_policy = GAP_SCAN_FILTER_ANY;
  uint8_t scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;

  /* Advertising parameters */
  uint8_t adv_evt_type = GAP_ADTYPE_ADV_IND;
  uint8_t adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
  uint8_t adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
  uint8_t adv_chann_map = GAP_ADVCHAN_ALL;
  uint8_t adv_filter_policy = GAP_ADV_FILTER_ANY;
  uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
  uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MAX;

  /* GAP Bond Manager parameters */
  uint8_t auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
  uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
  uint8_t auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
  uint8_t auth_oob = false;
  uint8_t auth_use_fix_passkey = false;
  uint32_t auth_fix_passkey = 0;
  uint8_t auth_sec_req_enable = false;
  uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

  /* Set device name and device appearance */
  le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
  le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
  le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                   &slave_init_mtu_req);

  /* Set scan parameters */
  le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(scan_mode), &scan_mode);
  le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(scan_interval), &scan_interval);
  le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(scan_window), &scan_window);
  le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
                    &scan_filter_policy);
  le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
                    &scan_filter_duplicate);

  /* Set advertising parameters */
  le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
  le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
  le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
  le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
  le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
  le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
  le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);

  /* Setup the GAP Bond Manager */
  gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
  gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
  gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
  gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
  le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
  le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                    &auth_use_fix_passkey);
  le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
  le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                    &auth_sec_req_flags);

  uint8_t phys_prefer = GAP_PHYS_PREFER_ALL;
  uint8_t tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
                           GAP_PHYS_PREFER_CODED_BIT;
  uint8_t rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
                           GAP_PHYS_PREFER_CODED_BIT;
  le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
  le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
  le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);

  le_register_app_cb(ble_gap_callback);
  client_register_general_client_cb(ble_gatt_client_callback);
  server_register_app_cb(ble_gatt_server_callback);

  return true;
}

void ble_start(void)
{
  log_v("ble_start");

  T_GAP_DEV_STATE new_state;
  ble_task_init();

  bt_coex_init();
  /*Wait BT init complete*/
  do
  {
    vTaskDelay(100 / portTICK_RATE_MS);
    le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
  } while (new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

  /*Start BT WIFI coexistence*/
  wifi_btcoex_set_bt_on();
}

void ble_deinit(void)
{
  log_v("ble_deinit");
  ble_task_deinit();
  T_GAP_DEV_STATE state;
  le_get_gap_param(GAP_PARAM_DEV_STATE, &state);
  free_ble_service_list();
  if (state.gap_init_state != GAP_INIT_STATE_STACK_READY)
  {
    log_d("[BLE Central]BT Stack is not running");
  }
  else
  {
    bte_deinit();
    log_d("[BLE Central]BT Stack deinitalized");
  }
}
