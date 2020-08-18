
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_api.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-10
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

#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "trace_app.h"
#include "bte.h"
#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "os_task.h"
#include "os_msg.h"
#include "app_msg.h"
#include "rtk_coex.h"
#include "profile_server.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif
#include "ble_common.h"
#include "ble_api.h"
#include "ble_utils.h"
#include "ble_callback.h"
#include "rpc_ble_api.h"

extern int ble_client_test_main();

uint8_t _Ble_HostInitialize(void)
{
  log_d("_Ble_HostInitialize called");
  ble_client_test_main();
  return 0;
}

RPC_T_GAP_CAUSE rpc_le_adv_set_param(RPC_T_LE_ADV_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_adv_set_param called");
  return le_adv_set_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_le_adv_get_param(RPC_T_LE_ADV_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_adv_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PARAM_SIZE;
  uint8_t *p_value = (uint8_t *)malloc(value->dataLength * sizeof(uint8_t));
  ret = le_adv_get_param(param, p_value);
  value->data = p_value;
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_adv_start(void)
{
  log_d("rpc_le_adv_start called");
  return le_adv_start();
}

RPC_T_GAP_CAUSE rpc_le_adv_stop(void)
{
  log_d("rpc_le_adv_start called");
  return le_adv_stop();
}

RPC_T_GAP_CAUSE rpc_le_adv_update_param(void)
{
  log_d("rpc_le_adv_update_param called");
  return le_adv_update_param();
}

bool rpc_le_gap_init(uint8_t link_num)
{
  log_d("rpc_le_gap_init called");
  return le_gap_init(link_num);
}

void rpc_le_gap_msg_info_way(bool use_msg)
{
  log_d("rpc_le_gap_msg_info_way called");
  return le_gap_init(use_msg);
}

uint8_t rpc_le_get_max_link_num(void)
{
  log_d("rpc_le_get_max_link_num called");
  return le_get_max_link_num();
}

RPC_T_GAP_CAUSE rpc_le_set_gap_param(RPC_T_GAP_LE_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_adv_update_param called");
  return le_set_gap_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_le_get_gap_param(RPC_T_GAP_LE_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PARAM_SIZE;
  int8_t *p_value = (uint8_t *)malloc(value->dataLength * sizeof(uint8_t));
  ret = le_get_gap_param(param, p_value);
  value->data = p_value;
  return ret;
}

RPC_T_GAP_CAUSE rpc_modify_white_list(RPC_T_GAP_WHITE_LIST_OP operation, const binary_t *bd_addr, RPC_T_GAP_REMOTE_ADDR_TYPE bd_type)
{
  log_d("rpc_modify_white_list called");
  //return modify_white_list(operation, bd_addr->data, bd_type);
  return RPC_GAP_CAUSE_SUCCESS;
}

RPC_T_GAP_CAUSE rpc_le_gen_rand_addr(RPC_T_GAP_RAND_ADDR_TYPE rand_addr_type, binary_t *random_bd)
{
  log_d("rpc_le_gen_rand_addr called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  random_bd->dataLength = DEFAULT_BT_ADDR_SIZE;
  uint8_t *p_value = (uint8_t *)malloc(random_bd->dataLength * sizeof(uint8_t));
  ret = le_gen_rand_addr(rand_addr_type, p_value);
  random_bd->data = p_value;
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_set_rand_addr(const binary_t *random_bd)
{
  log_d("rpc_le_set_rand_addr called");
  return le_set_rand_addr(random_bd->data);
}

RPC_T_GAP_CAUSE rpc_le_scan_set_param(RPC_T_LE_SCAN_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_scan_set_param called");
  return le_set_gap_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_le_scan_get_param(RPC_T_LE_SCAN_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_scan_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PARAM_SIZE;
  uint8_t *p_value = (uint8_t *)malloc(value->dataLength * sizeof(uint8_t));
  ret = le_scan_get_param(param, p_value);
  value->data = p_value;
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_scan_start(void)
{
  log_d("rpc_le_scan_start called");
  return le_scan_start();
}

RPC_T_GAP_CAUSE rpc_le_scan_stop(void)
{
  log_d("rpc_le_scan_stop called");
  return le_scan_stop();
}

//! @name rpc_gap_config
//@{
void rpc_gap_config_cccd_not_check(RPC_T_GAP_CONFIG_GATT_CCCD_NOT_CHECK cccd_not_check_flag)
{
  log_d("rpc_gap_config_cccd_not_check called");
  return gap_config_cccd_not_check(cccd_not_check_flag);
}

void rpc_gap_config_ccc_bits_count(uint8_t gatt_server_ccc_bits_count, uint8_t gatt_storage_ccc_bits_count)
{
  log_d("rpc_gap_config_ccc_bits_count called");
  return gap_config_ccc_bits_count(gatt_server_ccc_bits_count, gatt_storage_ccc_bits_count);
}

void rpc_gap_config_max_attribute_table_count(uint8_t gatt_max_attribute_table_count)
{
  log_d("rpc_gap_config_max_attribute_table_count called");
  return gap_config_max_attribute_table_count(gatt_max_attribute_table_count);
}

void rpc_gap_config_max_mtu_size(uint16_t att_max_mtu_size)
{
  log_d("rpc_gap_config_max_mtu_size called");
  return gap_config_max_mtu_size(att_max_mtu_size);
}

void rpc_gap_config_bte_pool_size(uint8_t bte_pool_size)
{
  log_d("rpc_gap_config_bte_pool_size called");
  return gap_config_bte_pool_size(bte_pool_size);
}

void rpc_gap_config_bt_report_buf_num(uint8_t bt_report_buf_num)
{
  log_d("gap_config_bt_report_buf_num called");
  return rpc_gap_config_bt_report_buf_num(bt_report_buf_num);
}

void rpc_gap_config_le_key_storage_flag(uint16_t le_key_storage_flag)
{
  log_d("rpc_gap_config_le_key_storage_flag called");
  return gap_config_le_key_storage_flag(le_key_storage_flag);
}

void rpc_gap_config_max_le_paired_device(uint8_t max_le_paired_device)
{
  log_d("rpc_gap_config_max_le_paired_device called");
  return gap_config_max_le_paired_device(max_le_paired_device);
}

void rpc_gap_config_max_le_link_num(uint8_t le_link_num)
{
  log_d("rpc_gap_config_max_le_link_num called");
  return gap_config_max_le_link_num(le_link_num);
}
//@}
