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

  typedef struct
  {
    uint16_t flags; // service flags
    bool is_primary;
    uint8_t uuid[2 + 14];
    uint8_t uuid_length;
    uint32_t permissions;
    uint16_t vlaue_length;
    uint8_t *p_value;
  } ble_service_t;

  typedef struct
  {
    uint16_t flags; // CHAR flags
    uint8_t uuid[2 + 14];
    uint8_t uuid_length;
    uint8_t properties;
    uint32_t permissions;
    uint16_t vlaue_length;
    uint8_t *p_value;
  } ble_char_t;

  typedef struct
  {
    uint16_t flags; // desc flags
    uint8_t uuid[2 + 14];
    uint8_t uuid_length;
    uint32_t permissions;
    uint16_t vlaue_length;
    uint8_t *p_value;
  } ble_desc_t;

  typedef struct s_ble_ble_desc_list
  {
    ble_desc_t desc;
    uint8_t handle;
    struct s_ble_ble_desc_list *next;
  } ble_desc_list_t;

  typedef struct s_ble_char_list
  {
    ble_char_t CHAR;
    uint8_t handle;
    uint8_t desc_length;
    ble_desc_list_t *desc_list;
    struct s_ble_char_list *next;
  } ble_char_list_t;

  typedef struct
  {
    ble_service_t service;
    bool is_alloc;
    uint8_t handle;
    ble_char_list_t *char_list;
    uint8_t char_length;
    uint8_t attr_num;
    T_ATTRIB_APPL *attr_tbl;
  } ble_service_list_t;

  bool ble_server_init(uint8_t num);

  void free_attr_tbl(T_SERVER_ID srvc_id);

  T_SERVER_ID ble_service_start(uint8_t app_id);

  uint8_t ble_create_service(ble_service_t service);

  bool ble_delete_service(uint8_t app_id);

  uint8_t ble_get_servie_handle(uint8_t app_id);

  void print_ble_serive_list(void);

  uint8_t ble_create_char(uint8_t app_id, ble_char_t CHAR);

  uint8_t ble_create_desc(uint8_t app_id, uint8_t char_handle, ble_desc_t desc);

#ifdef __cplusplus
}
#endif

#endif /* _BLE_SERVER_H_ */