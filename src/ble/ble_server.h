/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_server.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#ifndef _BLE_SERVER_H_
#define _BLE_SERVER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Add Includes here */
#include <bt_flags.h>
#include <profile_server.h>
#include "elog.h"
#include "ble_common.h"


/*============================================================================*
 *                              Types
 *============================================================================*/



/*============================================================================*
 *                              Funtions
 *============================================================================*/

T_SERVER_ID ble_add_service(const T_ATTRIB_APPL *ble_service_tbl, uint16_t length);

bool ble_server_init(uint8_t num);

#ifdef __cplusplus
}
#endif


#endif /* _BLE_SERVER_H_ */