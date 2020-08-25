
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_common.h
   * @brief     This file is used to config ble functions.
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#ifndef _PRC_BLE_COMMON_H_
#define _PRC_BLE_COMMON_H_

#include <bt_flags.h>
#include "elog.h"
#include "gap.h"
#include "gap_msg.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @brief  Config client LE link number */
#define BLE_CLIENT_MAX_LINKS 1

/** @brief  Config client app number*/
#define BLE_CLIENT_MAX_APPS 1

/** @brief  Config the discovery table number of gcs_client */

/** @brief  Config set physical: 0-Not built in, 1-built in*/
#define F_BT_LE_5_0_SET_PHY_SUPPORT 1

/** @brief  Config server LE link number */
#define BLE_SERVER_MAX_LINKS 1

#define BLE_DEVICE_ROLE_SERVER 1
#define BLE_DEVICE_ROLE_CLIENT 2


/**
 * @brief  Application Link control block defination.
 */
typedef struct
{
    T_GAP_CONN_STATE        conn_state;          /**< Connection state. */
    T_GAP_REMOTE_ADDR_TYPE  bd_type;             /**< remote BD type*/
    uint8_t                 bd_addr[GAP_BD_ADDR_LEN]; /**< remote BD */
} T_APP_LINK; 

extern T_GAP_DEV_STATE ble_gap_dev_state;
extern T_GAP_CONN_STATE ble_gap_conn_state;
extern T_APP_LINK ble_clinet_link_table[BLE_CLIENT_MAX_LINKS];
extern uint8_t ble_dev_role;

#endif /* _RPC_BLE_FLAGS_H_ */
