
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

#ifdef __cplusplus
extern "C"
{
#endif

#include "elog.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "rpc_system_header.h"
#include "wifi_callback.h"

bool wifi_init(void);


bool wifi_is_scaning();
uint16_t wifi_scan_get_ap_num();
int32_t wifi_scan_get_ap_records(uint16_t number, rtw_scan_result_t *_scanResult);
int32_t wifi_scan_start();

#ifdef __cplusplus
}
#endif

