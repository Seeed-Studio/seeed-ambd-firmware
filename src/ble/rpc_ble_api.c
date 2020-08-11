
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rpc_ble_api.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#include "rpc_ble_common.h"
#include "rpc_ble_api.h"
#include "rpc_ble_utils.h"
#include "rpc_ble_callback.h"

esp_err_t rpc_ble_gap_register_callback(esp_gap_ble_cb_t callback)
{
    esp_err_t result = ESP_OK;

    esp_gap_ble_cb_event_t cb_event;
    esp_ble_gap_cb_param_t *param;

    le_register_app_cb(rpc_ble_gap_callback);

    return result;
}

extern uint8_t rpc_ble_dev_role;
esp_err_t rpc_ble_dev_init(uint8_t role)
{
  esp_err_t result = ESP_OK;
  rpc_ble_dev_role = role;
  return result;
}
 