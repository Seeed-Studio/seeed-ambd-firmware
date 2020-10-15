
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

#define WL_SSID_MAX_LENGTH 32
#define WL_NETWORKS_LIST_MAXNUM 50

static uint16_t _networkCount = 0;
static bool isScaning = false;
static rtw_scan_result_t _scan_networks[WL_NETWORKS_LIST_MAXNUM] = {0};

bool wifi_init()
{

  wifi_callback_init();

  wifi_on(RTW_MODE_NONE);

  wifi_manager_init();

  wifi_event_reg_init();

  return true;
}

rtw_result_t wifi_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result)
{
  rtw_scan_result_t *record = &malloced_scan_result->ap_details;
  record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

  if (malloced_scan_result->scan_complete != RTW_TRUE)
  {
    if (_networkCount < WL_NETWORKS_LIST_MAXNUM)
    {
      memcpy(&(_scan_networks[_networkCount]), record, sizeof(rtw_scan_result_t));
      _networkCount++;
    }
  }
  else
  {
    isScaning = false;
    wifi_callback_ind(SYSTEM_EVENT_SCAN_DONE, NULL, 0);
  }
  return RTW_SUCCESS;
}

uint16_t wifi_scan_get_ap_num()
{
  log_d("called");
  return _networkCount;
}

int32_t wifi_scan_get_ap_records(uint16_t number, rtw_scan_result_t *_scanResult)
{
  log_d("called");
  if (number > WL_NETWORKS_LIST_MAXNUM || _scanResult == NULL)
  {
    return RTW_ERROR;
  }

  memcpy(_scanResult, _scan_networks, sizeof(rtw_scan_result_t) * number);
  return RTW_SUCCESS;
}

bool wifi_is_scaning()
{
  return isScaning;
}

int32_t wifi_scan_start()
{
  log_d("called");
  _networkCount = 0;
  int32_t ret = wifi_scan_networks(wifi_scan_result_handler, NULL);
  if (ret == RTW_SUCCESS)
  {
    isScaning = true;
  }
  return ret;
}
