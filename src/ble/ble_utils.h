/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_utils.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#ifndef _PRC_BLE__UTLIS_H_
#define _PRC_BLE__UTLIS_H_

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_conn_le.h>
#include "ble_common.h"
#include "rpc_ble_api.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

/*============================================================================*
 *                              Variables
 *============================================================================*/

/*============================================================================*
 *                              Marcos
 *============================================================================*/
#define FORMATION_BINARY(dest, source, dataType) \
  dest->dataLength = sizeof(dataType);           \
  dest->data = (uint8_t *)source

#define FORMTION_BINARY_GET_PARAM(LEN, FUN, PARAM)                       \
  value->dataLength = LEN;                                               \
  p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t)); \
  ret = FUN(PARAM, p_value);                                             \
  value->data = (uint8_t *)p_value

#define FORMTION_BINARY_GET_CONN_PARAM(LEN, FUN, PARAM, CONN_ID)         \
  value->dataLength = LEN;                                               \
  p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t)); \
  ret = FUN(PARAM, p_value, CONN_ID);                                    \
  value->data = (uint8_t *)p_value

#define FORMTION_BINARY_GET_PARAM_INVALID                                \
  value->dataLength = 1;                                                 \
  p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t)); \
  value->data = (uint8_t *)p_value;                                      \
  ret = RPC_GAP_CAUSE_INVALID_PARAM

#define DEFAULT_PARAM_SIZE 4
#define DEFAULT_BT_ADDR_SIZE 6
#define DEFAULT_PKEY_SIZE 6

/*============================================================================*
 *                              Functions
 *============================================================================*/

#endif