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

#ifdef __cplusplus
extern "C"
{
#endif

bool ble_init(void);
void ble_deinit(void);
void ble_start(void);

#ifdef __cplusplus
}
#endif