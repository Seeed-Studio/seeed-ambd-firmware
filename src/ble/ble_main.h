/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_main.h
   * @brief     This file is used to ble init & deinit functions.
   * @author    Hongtai.Liu
   * @date      2020-08-18
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "ble_client.h"
#include "ble_server.h"
#include "ble_common.h"
#include "ble_callback.h"
#include "ble_task.h"
#include "ble_utils.h"
#include "ble_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DEBUG_LOCAL 0

bool ble_init(void);
void ble_deinit(void);
void ble_start(void);

#ifdef __cplusplus
}
#endif