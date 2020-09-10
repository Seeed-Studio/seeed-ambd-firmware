/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_utils.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#ifdef __cplusplus
extern "C"
{
#endif

#include "FreeRTOS.h"
#include "timers.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
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
#include "elog.h"
#include "ble_common.h"
#include "ble_callback.h"

static void le_scan_cmpl_callback( TimerHandle_t xTimer)
{
  ble_gap_callback(GAP_MSG_LE_SCAN_CMPL, NULL);
  le_scan_stop();
}

T_GAP_CAUSE le_scan_start2(uint32_t tick)
{
  T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
  TimerHandle_t xTimers;
  xTimers = xTimerCreate("le_scan", pdMS_TO_TICKS(tick), pdFALSE, 0, le_scan_cmpl_callback);
  le_scan_start();
  xTimerStart(xTimers, 0);
  return ret;
}
