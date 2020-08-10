
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rpc_ble_callback.h
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
#include "elog.h"


void rpc_ble_handle_gap_msg(T_IO_MSG *p_gap_msg);
void rpc_ble_handle_io_msg(T_IO_MSG io_msg);
T_APP_RESULT rpc_ble_gap_callback(uint8_t cb_type, void *p_cb_data);

#ifdef __cplusplus
}
#endif

#endif /* _PRC_BLE_CALLBACK_H_ */
