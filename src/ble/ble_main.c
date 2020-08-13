
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

#include <os_sched.h>
#include <string.h>
#include <gap.h>
#include <gap_config.h>
#include <gap_bond_le.h>
#include <gap_scan.h>
#include <profile_client.h>
#include <gap_msg.h>
#include <gcs_client.h>
#include <bte.h>
#include "wifi_constants.h"
#include "FreeRTOS.h"
#include <wifi/wifi_conf.h>
#include "task.h"
#include "rtk_coex.h"
#include <stdio.h>

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