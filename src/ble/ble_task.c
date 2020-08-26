
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_task.c
   * @brief     Routines to create ble task and handle events & messages
   * @author    Hongtai.Liu
   * @date      2020-08-10
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   *****/

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <os_msg.h>
#include <os_task.h>
#include <gap.h>
#include <gap_le.h>
#include <app_msg.h>
#include <stdio.h>
#include <basic_types.h>
#include <gap_msg.h>

#include "ble_task.h"
#include "ble_callback.h"
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define RPC_BLE_TASK_PRIORITY 2                                                                                            //!< Task priorities
#define RPC_BLE_TASK_STACK_SIZE 256 * 8                                                                                    //!<  Task stack size
#define RPC_BLE_MAX_NUMBER_OF_GAP_MESSAGE 0x20                                                                             //!<  GAP message queue size
#define RPC_BLE_MAX_NUMBER_OF_IO_MESSAGE 0x20                                                                              //!<  IO message queue size
#define RPC_BLE_MAX_NUMBER_OF_EVENT_MESSAGE (RPC_BLE_MAX_NUMBER_OF_GAP_MESSAGE + RPC_BLE_MAX_NUMBER_OF_IO_MESSAGE) //!< Event message queue size

/*============================================================================*
 *                              Variables
 *============================================================================*/
void *ble_task_handle;     //!< APP Task handle
void *ble_evt_queue_handle; //!< Event queue handle
void *ble_io_queue_handle;  //!< IO queue handle

extern T_GAP_DEV_STATE ble_gap_dev_state;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief        ble task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
void ble_main_task(void *p_param)
{
    (void)p_param;
    uint8_t event;

    os_msg_queue_create(&ble_io_queue_handle, RPC_BLE_MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&ble_evt_queue_handle, RPC_BLE_MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(ble_evt_queue_handle, ble_io_queue_handle, RPC_BLE_MAX_NUMBER_OF_GAP_MESSAGE);

    while (true)
    {
        if (os_msg_recv(ble_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(ble_io_queue_handle, &io_msg, 0) == true)
                {
                    ble_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}

/**
 * @brief  Initialize App task
 * @return void
 */
void ble_task_init()
{
    os_task_create(&ble_task_handle, "ble task", ble_main_task, 0, RPC_BLE_TASK_STACK_SIZE,
                   RPC_BLE_TASK_PRIORITY);
}

void ble_task_deinit(void)
{
    if (ble_io_queue_handle)
    {
        os_msg_queue_delete(ble_io_queue_handle);
    }
    if (ble_evt_queue_handle)
    {
        os_msg_queue_delete(ble_evt_queue_handle);
    }
    if (ble_task_handle)
    {
        os_task_delete(ble_task_handle);
    }
    ble_io_queue_handle = NULL;
    ble_evt_queue_handle = NULL;
    ble_task_handle = NULL;
}
