
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
#include "ble_common.h"
#include "ble_callback.h"
#include "ble_task.h"
#include "ble_utils.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

T_GAP_DEV_STATE ble_gap_dev_state = {0, 0, 0, 0, 0}; /**< GAP device state */
T_GAP_CONN_STATE ble_gap_conn_state = GAP_CONN_STATE_DISCONNECTED;
T_APP_LINK ble_clinet_link_table[BLE_CLIENT_MAX_LINKS];
uint8_t ble_dev_role = 0; // 0:close 1:server 2:client

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

  return true;
}

void ble_start(void)
{
  log_v("ble_start");
  T_GAP_DEV_STATE new_state;
  le_register_app_cb(ble_gap_callback);
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
