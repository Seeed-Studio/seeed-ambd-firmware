
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rpc_ble_task.h
   * @brief     Routines to create ble task and handle events & messages
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#include "elog.h"

#ifndef _PRC_BLE_TASK_H_
#define _RPC_BLE_TASK_H_

void rpc_ble_main_task(void *p_param);
void rpc_ble_app_task_init();

#endif /* _RPC_BLE_TASK_H_ */
