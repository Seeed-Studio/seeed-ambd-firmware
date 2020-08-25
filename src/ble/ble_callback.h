
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_callback.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#ifndef _PRC_BLE_CALLBACK_H_
#define _PRC_BLE_CALLBACK_H_


#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <profile_client.h>
#include <app_msg.h>
#include "ble_main.h"
#include "elog.h"


void ble_handle_gap_msg(T_IO_MSG *p_gap_msg);
void ble_handle_io_msg(T_IO_MSG io_msg);
T_APP_RESULT ble_gap_callback(uint8_t cb_type, void *p_cb_data);
T_APP_RESULT ble_gatt_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

#ifdef __cplusplus
}
#endif

#endif /* _PRC_BLE_CALLBACK_H_ */
