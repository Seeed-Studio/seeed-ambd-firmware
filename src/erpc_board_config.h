
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      wifi_api.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-09-18
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#ifndef _ERPC_BOARD_CONFIG_H_
#define _ERPC_BOARD_CONFIG_H_

#ifdef SEEED_XIAO_M0
#define ERPC_SERIAL             Serial3
#define ERPC_SERIAL_BADU        1843200
#define ERPC_SERIAL_SENDDELAY   20

#elif SEEED_WIO_TERMINAL
#define ERPC_SERIAL       Serial2
#define ERPC_SERIAL_BADU  1843200

#else
#error "BOARD_TYPE ERROR"
#endif

#endif /* _ERPC_BOARD_CONFIG_H_ */

