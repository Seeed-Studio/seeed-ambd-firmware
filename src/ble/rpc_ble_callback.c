
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rpc_ble_callback.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdio.h>
#include <app_msg.h>
#include <string.h>
#include <trace_app.h>
#include <gap_scan.h>
#include <gap.h>
#include <gap_msg.h>
#include <gap_bond_le.h>
#include <gcs_client.h>
#include "platform_opts_bt.h"


#include "rpc_ble_callback.h" 

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void rpc_ble_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
    {
        rpc_ble_handle_gap_msg(&io_msg);
    }
    break;
    default:
        break;
    }
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void rpc_ble_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    //T_LE_GAP_MSG gap_msg;
    //memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));
    (void *) p_gap_msg;
    log_v("ble_central_app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
}


/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT rpc_ble_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    (void *) p_cb_data;
    log_v("rpc_ble_gap_callback: cb_type %d", cb_type);

    //ble_gap_callback(cb_event, param); // after resort param, callbacak to client
}






