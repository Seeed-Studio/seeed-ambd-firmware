
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_api.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#ifndef _PRC_BLE_API_H_
#define _PRC_BLE_API_H_

#include "esp_gap_ble_api.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif
esp_err_t rpc_ble_dev_init(uint8_t role);
#ifdef __cplusplus
}
#endif

#endif /* _PRC_BLE_API_H_ */

