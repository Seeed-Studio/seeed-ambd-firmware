
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_main.c
   * @brief     Source file for WiFi project, mainly used for initialize modules
   * @author    Hongtai.Liu
   * @date      2020-09-24
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/

#include "elog.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "rpc_system_header.h"
#include "wifi_callback.h"

bool wifi_init()
{

  wifi_callback_init();

  wifi_on(RTW_MODE_NONE);

  wifi_manager_init();

  wifi_event_reg_init();

  return true;
}
