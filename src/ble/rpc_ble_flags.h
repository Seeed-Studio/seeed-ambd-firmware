
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rpc_ble_flags.h
   * @brief     This file is used to config ble functions.
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#ifndef _PRC_BLE_FLAGS_H_
#define _RPC_BLE_FLAGS_H_

#include <bt_flags.h>
#include "elog.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @brief  Config client LE link number */
#define BLE_CLIENT_MAX_LINKS 3

/** @brief  Config the discovery table number of gcs_client */
#define BLE_CLIENT_MAX_DISCOV_TABLE_NUM 40

/** @brief  Config set physical: 0-Not built in, 1-built in*/
#define F_BT_LE_5_0_SET_PHY_SUPPORT 1

/** @brief  Config server LE link number */
#define BLE_SERVER_MAX_LINKS 1


#endif /* _RPC_BLE_FLAGS_H_ */
