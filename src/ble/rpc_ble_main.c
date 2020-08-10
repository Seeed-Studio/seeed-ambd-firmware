
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      rec_ble_main.c
   * @brief     Source file for BLE project, mainly used for initialize modules
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

#include <os_sched.h>
#include <string.h>
#include <gap.h>
#include <gap_config.h>
#include <gap_bond_le.h>
#include <gap_scan.h>
#include <profile_client.h>
#include <gap_msg.h>
#include <gcs_client.h>
#include <bte.h>
#include "wifi_constants.h"
#include "FreeRTOS.h"
#include <wifi/wifi_conf.h>
#include "task.h"
#include "rtk_coex.h"
#include <stdio.h>


#include "rpc_ble_flags.h"
#include "rpc_ble_callback.h"
#include "rpc_ble_task.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

T_GAP_DEV_STATE rpc_ble_gap_dev_state = {0, 0, 0, 0, 0}; /**< GAP device state */


/** @brief Default scan interval (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_INTERVAL     0x520
/** @brief Default scan window (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_WINDOW       0x520



void ble_init()
{
	gap_config_max_le_link_num(3);
	bte_init();
	le_gap_init(3);

	/* Device name and device appearance */
    uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "BLE_CENTRAL_CLIENT";
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

    /* Scan parameters */
    uint8_t  scan_mode = GAP_SCAN_MODE_ACTIVE;
    uint16_t scan_interval = DEFAULT_SCAN_INTERVAL;
    uint16_t scan_window = DEFAULT_SCAN_WINDOW;
    uint8_t  scan_filter_policy = GAP_SCAN_FILTER_ANY;
    uint8_t  scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;

    /* GAP Bond Manager parameters */
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t  auth_oob = false;
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint8_t  auth_sec_req_enable = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

    /* Set device name and device appearance */
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);

    /* Set scan parameters */
    le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(scan_mode), &scan_mode);
    le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(scan_interval), &scan_interval);
    le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(scan_window), &scan_window);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
                      &scan_filter_policy);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
                      &scan_filter_duplicate);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
#if F_BT_LE_SMP_OOB_SUPPORT
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    /* register gap message callback */
    le_register_app_cb(rpc_ble_gap_callback);

#if F_BT_LE_5_0_SET_PHY_SUPPORT
	uint8_t  phys_prefer = GAP_PHYS_PREFER_ALL;
	uint8_t  tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							  GAP_PHYS_PREFER_CODED_BIT;
	uint8_t  rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							  GAP_PHYS_PREFER_CODED_BIT;
	le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);
#endif

	client_init(1);

	rpc_ble_app_task_init();
}
