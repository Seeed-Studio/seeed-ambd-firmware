
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_api.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#include "ble_common.h"
#include "ble_api.h"
#include "ble_utils.h"
#include "ble_callback.h"
//#include "rpc_ble_api.h"

extern uint8_t rpc_ble_dev_role;
esp_err_t rpc_ble_dev_init(uint8_t role)
{
  esp_err_t result = ESP_OK;
  rpc_ble_dev_role = role;
  return result;
}


// RPC_T_GAP_CAUSE rpc_le_adv_set_param(RPC_T_LE_ADV_PARAM_TYPE param, const binary_t * value)
// {
//   return (RPC_T_GAP_CAUSE)le_adv_set_param(param, value->dataLength, value->data);
// }
