
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
#include "timers.h"
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
#include "ble_main.h"
#include "rpc_ble_header.h"
#include "rpc_ble_api.h"

//! @name host
//@{

bool rpc_ble_init(void)
{
  log_d("rpc_ble_init called");
  return ble_init();
}

void rpc_ble_start(void)
{
  log_d("rpc_ble_start called");
  return ble_start();
}

void rpc_ble_deinit(void)
{
  log_d("rpc_ble_deinit called");
  return ble_deinit();
}
//@}

//! @name rpc_gap_adv
//@{

RPC_T_GAP_CAUSE rpc_le_adv_set_param(RPC_T_LE_ADV_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_adv_set_param called");
  return le_adv_set_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_le_adv_get_param(RPC_T_LE_ADV_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_adv_get_param called\n\r");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_LE_ADV_PARAM_TYPE)param)
  {
  case GAP_PARAM_ADV_LOCAL_ADDR_TYPE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_LOCAL_ADDR_TYPE);
    break;
  }
  case GAP_PARAM_ADV_DATA:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_DATA);
    break;
  }
  case GAP_PARAM_SCAN_RSP_DATA:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_SCAN_RSP_DATA);
    break;
  }
  case GAP_PARAM_ADV_EVENT_TYPE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_EVENT_TYPE);
    break;
  }
  case GAP_PARAM_ADV_DIRECT_ADDR_TYPE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_DIRECT_ADDR_TYPE);
    break;
  }
  case GAP_PARAM_ADV_DIRECT_ADDR:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_DIRECT_ADDR);
    break;
  }
  case GAP_PARAM_ADV_CHANNEL_MAP:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_CHANNEL_MAP);
    break;
  }
  case GAP_PARAM_ADV_FILTER_POLICY:
  {
    FORMTION_BINARY_GET_PARAM(1, le_adv_get_param, GAP_PARAM_ADV_FILTER_POLICY);
    break;
  }
  case GAP_PARAM_ADV_INTERVAL_MIN:
  {
    FORMTION_BINARY_GET_PARAM(2, le_adv_get_param, GAP_PARAM_ADV_INTERVAL_MIN);
    break;
  }
  case GAP_PARAM_ADV_INTERVAL_MAX:
  {
    FORMTION_BINARY_GET_PARAM(2, le_adv_get_param, GAP_PARAM_ADV_INTERVAL_MAX);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
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
//@}

//! @name rpc_gap_le
//@{

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
  log_d("rpc_le_get_gap_param called\n\r");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_GAP_LE_PARAM_TYPE)param)
  {
  case GAP_PARAM_DEV_STATE:
  {
    FORMTION_BINARY_GET_PARAM(sizeof(T_GAP_DEV_STATE), le_get_gap_param, GAP_PARAM_DEV_STATE);
    break;
  }
  case GAP_PARAM_APPEARANCE:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_DEV_STATE);
    break;
  }
  case GAP_PARAM_DEVICE_NAME:
  {
    FORMTION_BINARY_GET_PARAM(sizeof(GAP_DEVICE_NAME_LEN), le_get_gap_param, GAP_PARAM_DEVICE_NAME);
    break;
  }
  case GAP_PARAM_LATEST_CONN_BD_ADDR:
  {
    FORMTION_BINARY_GET_PARAM(DEFAULT_BT_ADDR_SIZE, le_get_gap_param, GAP_PARAM_LATEST_CONN_BD_ADDR);
    break;
  }
  case GAP_PARAM_LATEST_CONN_BD_ADDR_TYPE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_LATEST_CONN_BD_ADDR_TYPE);
    break;
  }
  case GAP_PARAM_DEFAULT_PHYS_PREFER:
  {
    FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_DEFAULT_PHYS_PREFER);
    break;
  }
  case GAP_PARAM_DEFAULT_TX_PHYS_PREFER:
  {
    FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_DEFAULT_TX_PHYS_PREFER);
    break;
  }
  case GAP_PARAM_DEFAULT_RX_PHYS_PREFER:
  {
    FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_DEFAULT_RX_PHYS_PREFER);
    break;
  }
  // case GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_OCTETS:
  // {
  //   FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_OCTETS);
  //   break;
  // }
  // case GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_TIME:
  // {
  //   FORMTION_BINARY_GET_PARAM(1, le_get_gap_param, GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_TIME);
  //   break;
  // }
  case GAP_PARAM_LOCAL_FEATURES:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_LOCAL_FEATURES);
    break;
  }
  case GAP_PARAM_DS_POOL_ID:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_DS_POOL_ID);
    break;
  }
  case GAP_PARAM_DS_DATA_OFFSET:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_DS_DATA_OFFSET);
    break;
  }
  case GAP_PARAM_LE_REMAIN_CREDITS:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_LE_REMAIN_CREDITS);
    break;
  }
  case GAP_PARAM_MAX_WL_SIZE:
  {
    FORMTION_BINARY_GET_PARAM(2, le_get_gap_param, GAP_PARAM_MAX_WL_SIZE);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_modify_white_list(RPC_T_GAP_WHITE_LIST_OP operation, const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type)
{
  log_d("rpc_le_modify_white_list called");
  return le_modify_white_list(operation, bd_addr, bd_type);
}

RPC_T_GAP_CAUSE rpc_le_gen_rand_addr(RPC_T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t random_bd[6])
{
  log_d("rpc_le_gen_rand_addr called");
  return le_gen_rand_addr(rand_addr_type, random_bd);
}

RPC_T_GAP_CAUSE rpc_le_set_rand_addr(const uint8_t random_bd[6])
{
  log_d("rpc_le_set_rand_addr called");
  return le_set_rand_addr(random_bd);
}

RPC_T_GAP_CAUSE rpc_le_cfg_local_identity_address(const uint8_t addr[6], RPC_T_GAP_IDENT_ADDR_TYPE ident_addr_type)
{
  log_d("rpc_le_cfg_local_identity_address called");
  return le_cfg_local_identity_address(addr, ident_addr_type);
}

RPC_T_GAP_CAUSE rpc_le_set_host_chann_classif(uint8_t p_channel_map)
{
  log_d("rpc_le_set_host_chann_classif called");
  return le_set_host_chann_classif(p_channel_map);
}

RPC_T_GAP_CAUSE rpc_le_write_default_data_len(uint16_t tx_octets, uint16_t tx_time)
{
  log_d("rpc_le_write_default_data_len called");
  return le_write_default_data_len(tx_octets, tx_time);
}

//@}

//! @name rpc_gap_scan
//@{

RPC_T_GAP_CAUSE rpc_le_scan_set_param(RPC_T_LE_SCAN_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_scan_set_param called");
  return le_scan_set_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_le_scan_get_param(RPC_T_LE_SCAN_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_scan_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_LE_SCAN_PARAM_TYPE)param)
  {
  case GAP_PARAM_SCAN_LOCAL_ADDR_TYPE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_scan_get_param, GAP_PARAM_SCAN_LOCAL_ADDR_TYPE);
    break;
  }
  case GAP_PARAM_SCAN_MODE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_scan_get_param, GAP_PARAM_SCAN_MODE);
    break;
  }
  case GAP_PARAM_SCAN_INTERVAL:
  {
    FORMTION_BINARY_GET_PARAM(2, le_scan_get_param, GAP_PARAM_SCAN_INTERVAL);
    break;
  }
  case GAP_PARAM_SCAN_WINDOW:
  {
    FORMTION_BINARY_GET_PARAM(2, le_scan_get_param, GAP_PARAM_SCAN_WINDOW);
    break;
  }
  case GAP_PARAM_SCAN_FILTER_POLICY:
  {
    FORMTION_BINARY_GET_PARAM(1, le_scan_get_param, GAP_PARAM_SCAN_FILTER_POLICY);
    break;
  }
  case GAP_PARAM_SCAN_FILTER_DUPLICATES:
  {
    FORMTION_BINARY_GET_PARAM(1, le_scan_get_param, GAP_PARAM_SCAN_FILTER_DUPLICATES);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_scan_start(void)
{
  log_d("rpc_le_scan_start called");
  return le_scan_start();
}

RPC_T_GAP_CAUSE rpc_le_scan_timer_start(uint32_t tick)
{
  log_d("rpc_le_scan_timer_start called");
  return le_scan_timer_start(tick);
}
RPC_T_GAP_CAUSE rpc_le_scan_stop(void)
{
  log_d("rpc_le_scan_stop called");
  le_scan_timer_stop();
  return le_scan_stop();
}

bool rpc_le_scan_info_filter(bool enable, uint8_t offset, uint8_t len, const uint8_t p_filter[31])
{
  log_d("rpc_le_scan_info_filter called");
  return le_scan_info_filter(enable, offset, len, p_filter);
}
//@}

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
  return gap_config_bt_report_buf_num(bt_report_buf_num);
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

//! @name rpc_gap
//@{

RPC_T_GAP_CAUSE rpc_gap_set_param(RPC_T_GAP_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_gap_set_param called");
  return gap_set_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_gap_get_param(RPC_T_GAP_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_gap_get_param called\n\r");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_GAP_PARAM_TYPE)param)
  {
  case GAP_PARAM_BD_ADDR:
  {
    FORMTION_BINARY_GET_PARAM(DEFAULT_BT_ADDR_SIZE, gap_get_param, GAP_PARAM_BD_ADDR);
    break;
  }
  case GAP_PARAM_BOND_PAIRING_MODE:
  {
    FORMTION_BINARY_GET_PARAM(1, gap_get_param, GAP_PARAM_BOND_PAIRING_MODE);
    break;
  }
  case GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS:
  {
    FORMTION_BINARY_GET_PARAM(2, gap_get_param, GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS);
    break;
  }
  case GAP_PARAM_BOND_IO_CAPABILITIES:
  {
    FORMTION_BINARY_GET_PARAM(1, gap_get_param, GAP_PARAM_BOND_IO_CAPABILITIES);
    break;
  }
  case GAP_PARAM_BOND_OOB_ENABLED:
  {
    FORMTION_BINARY_GET_PARAM(1, gap_get_param, GAP_PARAM_BOND_OOB_ENABLED);
    break;
  }
  case GAP_PARAM_BOND_LE_PAIRING_MODE:
  {
    FORMTION_BINARY_GET_PARAM(1, gap_get_param, GAP_PARAM_BOND_OOB_ENABLED);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
  return ret;
}

RPC_T_GAP_CAUSE rpc_gap_set_pairable_mode(void)
{
  log_d("rpc_gap_set_pairable_mode called");
  return gap_set_pairable_mode();
}
//@}

//! @name rpc_gap_bone
//@{
RPC_T_GAP_CAUSE rpc_le_bond_set_param(RPC_T_LE_BOND_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_le_bond_set_param called");
  return le_bond_set_param(param, value->dataLength, value->dataLength);
}

RPC_T_GAP_CAUSE rpc_le_bond_get_param(RPC_T_LE_BOND_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_le_bond_get_param called\n\r");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_LE_BOND_PARAM_TYPE)param)
  {
  case GAP_PARAM_BOND_OOB_DATA:
  {
    FORMTION_BINARY_GET_PARAM(GAP_OOB_LEN, le_bond_get_param, GAP_PARAM_BOND_OOB_DATA);
    break;
  }
  case GAP_PARAM_BOND_FIXED_PASSKEY:
  {
    FORMTION_BINARY_GET_PARAM(4, le_bond_get_param, GAP_PARAM_BOND_FIXED_PASSKEY);
    break;
  }
  case GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_bond_get_param, GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE);
    break;
  }
  case GAP_PARAM_BOND_SEC_REQ_REQUIREMENT:
  {
    FORMTION_BINARY_GET_PARAM(1, le_bond_get_param, GAP_PARAM_BOND_SEC_REQ_REQUIREMENT);
    break;
  }
  case GAP_PARAM_BOND_MIN_KEY_SIZE:
  {
    FORMTION_BINARY_GET_PARAM(1, le_bond_get_param, GAP_PARAM_BOND_MIN_KEY_SIZE);
    break;
  }
  case GAP_PARAM_BOND_KEY_MANAGER:
  {
    FORMTION_BINARY_GET_PARAM(1, le_bond_get_param, GAP_PARAM_BOND_KEY_MANAGER);
    break;
  }
  case GAP_PARAM_BOND_SIGN_KEY_FLAG:
  {
    FORMTION_BINARY_GET_PARAM(1, le_bond_get_param, GAP_PARAM_BOND_SIGN_KEY_FLAG);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
  return ret;
}
RPC_T_GAP_CAUSE rpc_le_bond_pair(uint8_t conn_id)
{
  log_d("rpc_le_bond_pair called");
  return le_bond_pair(conn_id);
}

RPC_T_GAP_CAUSE rpc_le_bond_get_display_key(uint8_t conn_id, uint32_t *key)
{
  log_d("rpc_le_bond_get_display_key called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  ret = le_bond_get_display_key(conn_id, key);
  return ret;
}

RPC_T_GAP_CAUSE rpc_le_bond_passkey_input_confirm(uint8_t conn_id, uint32_t passcode, RPC_T_GAP_CFM_CAUSE cause)
{
  log_d("rpc_le_bond_passkey_input_confirm called");
  return le_bond_passkey_input_confirm(conn_id, passcode, cause);
}

RPC_T_GAP_CAUSE rpc_le_bond_oob_input_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause)
{
  log_d("rpc_le_bond_oob_input_confirm called");
  return le_bond_oob_input_confirm(conn_id, cause);
}

RPC_T_GAP_CAUSE rpc_le_bond_just_work_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause)
{
  log_d("rpc_le_bond_just_work_confirm called");
  return le_bond_just_work_confirm(conn_id, cause);
}

RPC_T_GAP_CAUSE rpc_le_bond_passkey_display_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause)
{
  log_d("rpc_le_bond_passkey_display_confirm called");
  return le_bond_passkey_display_confirm(conn_id, cause);
}

RPC_T_GAP_CAUSE rpc_le_bond_user_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause)
{
  log_d("rpc_le_bond_user_confirm called");
  return le_bond_user_confirm(conn_id, cause);
}

RPC_T_GAP_CAUSE rpc_le_bond_cfg_local_key_distribute(uint8_t init_dist, uint8_t rsp_dist)
{
  log_d("rpc_le_bond_cfg_local_key_distribute called");
  return le_bond_cfg_local_key_distribute(init_dist, rsp_dist);
}

void rpc_le_bond_clear_all_keys(void)
{
  log_d("rpc_le_bond_clear_all_keys called");
  return le_bond_clear_all_keys();
}

RPC_T_GAP_CAUSE rpc_le_bond_delete_by_idx(uint8_t idx)
{
  log_d("rpc_le_bond_delete_by_idx called");
  return le_bond_delete_by_idx(idx);
}

RPC_T_GAP_CAUSE rpc_le_bond_delete_by_bd(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type)
{
  log_d("rpc_le_bond_delete_by_bd called");
  return le_bond_delete_by_bd(bd_addr, bd_type);
}

RPC_T_GAP_CAUSE rpc_le_bond_get_sec_level(uint8_t conn_id, RPC_T_GAP_SEC_LEVEL *sec_type)
{
  log_d("rpc_le_bond_get_sec_level called");
  return le_bond_get_sec_level(conn_id, sec_type);
}
//@}

//! @name rpc_gap_conn
//@{
RPC_T_GAP_CAUSE rpc_le_get_conn_param(RPC_T_LE_CONN_PARAM_TYPE param, binary_t *value, uint8_t conn_id)
{
  log_d("rpc_le_get_conn_param called\n\r");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  uint8_t *p_value = NULL;
  switch ((T_LE_CONN_PARAM_TYPE)param)
  {
  case GAP_PARAM_CONN_BD_ADDR:
  {
    FORMTION_BINARY_GET_CONN_PARAM(DEFAULT_BT_ADDR_SIZE, le_get_conn_param, GAP_PARAM_CONN_BD_ADDR, conn_id);
    break;
  }
  case GAP_PARAM_CONN_BD_ADDR_TYPE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(1, le_get_conn_param, GAP_PARAM_CONN_BD_ADDR_TYPE, conn_id);
    break;
  }
  case GAP_PARAM_CONN_INTERVAL:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_INTERVAL, conn_id);
    break;
  }
  case GAP_PARAM_CONN_LATENCY:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_LATENCY, conn_id);
    break;
  }
  case GAP_PARAM_CONN_TIMEOUT:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_TIMEOUT, conn_id);
    break;
  }
  case GAP_PARAM_CONN_MTU_SIZE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_MTU_SIZE, conn_id);
    break;
  }
  case GAP_PARAM_CONN_LOCAL_BD_TYPE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(1, le_get_conn_param, GAP_PARAM_CONN_LOCAL_BD_TYPE, conn_id);
    break;
  }
  case GAP_PARAM_CONN_RX_PHY_TYPE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(1, le_get_conn_param, GAP_PARAM_CONN_RX_PHY_TYPE, conn_id);
    break;
  }
  case GAP_PARAM_CONN_TX_PHY_TYPE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(1, le_get_conn_param, GAP_PARAM_CONN_TX_PHY_TYPE, conn_id);
    break;
  }
  case GAP_PARAM_CONN_REMOTE_FEATURES:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_REMOTE_FEATURES, conn_id);
    break;
  }
  case GAP_PARAM_CONN_HANDLE:
  {
    FORMTION_BINARY_GET_CONN_PARAM(2, le_get_conn_param, GAP_PARAM_CONN_HANDLE, conn_id);
    break;
  }
  default:
  {
    FORMTION_BINARY_GET_PARAM_INVALID;
    break;
  }
  }
  return ret;
}

bool rpc_le_get_conn_info(uint8_t conn_id, RPC_T_GAP_CONN_INFO *p_conn_info)
{
  log_d("rpc_le_get_conn_info called");
  return le_get_conn_info(conn_id, p_conn_info);
}

bool rpc_le_get_conn_addr(uint8_t conn_id, uint8_t bd_addr[6], uint8_t *bd_type)
{
  log_d("rpc_le_get_conn_info called");
  return le_get_conn_addr(conn_id, bd_addr, bd_type);
}

bool rpc_le_get_conn_id(const uint8_t bd_addr[6], uint8_t bd_type, uint8_t *p_conn_id)
{
  log_d("rpc_le_get_conn_id called");
  return le_get_conn_id(bd_addr, bd_type, p_conn_id);
}

uint8_t rpc_le_get_active_link_num(void)
{
  log_d("rpc_le_get_active_link_num called");
  return le_get_active_link_num();
}

uint8_t rpc_le_get_idle_link_num(void)
{
  log_d("rpc_le_get_idle_link_num called");
  return le_get_idle_link_num();
}

RPC_T_GAP_CAUSE rpc_le_disconnect(uint8_t conn_id)
{
  log_d("rpc_le_disconnect called");
  return le_disconnect(conn_id);
}

RPC_T_GAP_CAUSE rpc_le_read_rssi(uint8_t conn_id)
{
  log_d("rpc_le_disconnect called");
  return le_read_rssi(conn_id);
}

RPC_T_GAP_CAUSE rpc_le_set_data_len(uint8_t conn_id, uint16_t tx_octets, uint16_t tx_time)
{
  log_d("le_set_data_len called");
  return le_set_data_len(conn_id, tx_octets, tx_time);
}

RPC_T_GAP_CAUSE rpc_le_set_phy(uint8_t conn_id, uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys, RPC_T_GAP_PHYS_OPTIONS phy_options)
{
  log_d("rpc_le_set_phy called");
  return le_set_phy(conn_id, all_phys, tx_phys, rx_phys, phy_options);
}

RPC_T_GAP_CAUSE rpc_le_set_conn_param(RPC_T_GAP_CONN_PARAM_TYPE conn_type, const RPC_T_GAP_LE_CONN_REQ_PARAM *p_conn_param)
{
  log_d("rpc_le_set_conn_param called");
  return le_set_conn_param(conn_type, p_conn_param);
}

RPC_T_GAP_CAUSE rpc_le_connect(uint8_t init_phys, const uint8_t remote_bd[6], RPC_T_GAP_REMOTE_ADDR_TYPE remote_bd_type, RPC_T_GAP_LOCAL_ADDR_TYPE local_bd_type, uint16_t scan_timeout)
{
  log_d("rpc_le_connect called");
  return le_connect(init_phys, remote_bd, remote_bd_type, local_bd_type, scan_timeout);
}

RPC_T_GAP_CAUSE rpc_le_update_conn_param(uint8_t conn_id, uint16_t conn_interval_min, uint16_t conn_interval_max, uint16_t conn_latency, uint16_t supervision_timeout, uint16_t ce_length_min, uint16_t ce_length_max)
{
  log_d("rpc_le_connect called");
  return le_update_conn_param(conn_id, conn_interval_min, conn_interval_max, conn_latency, supervision_timeout, ce_length_min, ce_length_max);
}
//@}

//! @name rpc_gatt_client
//@{
bool rpc_ble_client_init(uint8_t num)
{
  log_d("rpc_client_init called");
  return ble_client_init(num);
}

uint8_t rpc_ble_add_client(uint8_t app_id, uint8_t link_num)
{
  log_d("rpc_ble_add_client called");
  return ble_add_client(app_id, link_num);
}

void rpc_client_init(uint8_t client_num)
{
  log_d("rpc_client_init called");
  return client_init(client_num);
}

RPC_T_GAP_CAUSE rpc_client_all_primary_srv_discovery(uint8_t conn_id, uint8_t client_id)
{
  log_d("rpc_client_all_primary_srv_discovery called");
  return client_all_primary_srv_discovery(conn_id, client_id);
}

RPC_T_GAP_CAUSE rpc_client_by_uuid_srv_discovery(uint8_t conn_id, uint8_t client_id, uint16_t uuid16)
{
  log_d("rpc_client_all_primary_srv_discovery called");
  return client_by_uuid_srv_discovery(conn_id, client_id, uuid16);
}

RPC_T_GAP_CAUSE rpc_client_by_uuid128_srv_discovery(uint8_t conn_id, uint8_t client_id, const uint8_t p_uuid128[16])
{
  log_d("rpc_client_by_uuid128_srv_discovery called");
  return client_by_uuid_srv_discovery(conn_id, client_id, p_uuid128);
}

RPC_T_GAP_CAUSE rpc_client_relationship_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle)
{
  log_d("rpc_client_by_uuid128_srv_discovery called");
  return client_relationship_discovery(conn_id, client_id, start_handle, end_handle);
}

RPC_T_GAP_CAUSE rpc_client_all_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle)
{
  log_d("rpc_client_by_uuid128_srv_discovery called");
  return client_all_char_discovery(conn_id, client_id, start_handle, end_handle);
}

RPC_T_GAP_CAUSE rpc_client_by_uuid_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, uint16_t uuid16)
{
  log_d("rpc_client_by_uuid_char_discovery called");
  return client_by_uuid_char_discovery(conn_id, client_id, start_handle, end_handle, uuid16);
}

RPC_T_GAP_CAUSE rpc_client_by_uuid128_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, const uint8_t p_uuid128[16])
{
  log_d("rpc_client_by_uuid128_char_discovery called");
  return client_by_uuid128_char_discovery(conn_id, client_id, start_handle, end_handle, p_uuid128);
}

RPC_T_GAP_CAUSE rpc_client_all_char_descriptor_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle)
{
  log_d("rpc_client_all_char_descriptor_discovery called");
  return client_all_char_descriptor_discovery(conn_id, client_id, start_handle, end_handle);
}

RPC_T_GAP_CAUSE rpc_client_attr_read(uint8_t conn_id, uint8_t client_id, uint16_t handle)
{
  log_d("rpc_client_attr_read called");
  return client_attr_read(conn_id, client_id, handle);
}

RPC_T_GAP_CAUSE rpc_client_attr_read_using_uuid(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, uint16_t uuid16, const uint8_t p_uuid128[16])
{
  log_d("rpc_client_all_char_descriptor_discovery called");
  return client_attr_read_using_uuid(conn_id, client_id, start_handle, end_handle, uuid16, p_uuid128);
}

RPC_T_GAP_CAUSE rpc_client_attr_write(uint8_t conn_id, uint8_t client_id, RPC_T_GATT_WRITE_TYPE write_type, uint16_t handle, const binary_t *data)
{
  log_d("rpc_client_attr_write called");
  return client_attr_write(conn_id, client_id, write_type, handle, data->dataLength, data->data);
}

RPC_T_GAP_CAUSE rpc_client_attr_ind_confirm(uint8_t conn_id)
{
  log_d("rpc_client_attr_ind_confirm called");
  return client_attr_ind_confirm(conn_id);
}
//@}

//! @name rpc_gatt_server
//@{
bool rpc_ble_server_init(uint8_t num)
{
  log_d("rpc_ble_server_init called");
  return ble_server_init(num);
}

uint8_t rpc_ble_create_service(const uint8_t uuid[16], uint8_t uuid_length, bool is_primary)
{
  log_d("rpc_ble_create_service called");
  ble_service_t srcv;
  srcv.is_primary = is_primary;
  srcv.uuid_length = uuid_length;
  memcpy(srcv.uuid, uuid, uuid_length);
  return ble_create_service(srcv);
}

bool rpc_ble_delete_service(uint8_t app_id)
{
  log_d("rpc_ble_delete_service called");
  return ble_delete_service(app_id);
}

uint8_t rpc_ble_service_start(uint8_t app_id)
{
  log_d("rpc_ble_service_start called");
  return ble_service_start(app_id);
}

uint8_t rpc_ble_get_servie_handle(uint8_t app_id)
{
  log_d("rpc_ble_get_servie_handle called");
  return ble_get_servie_handle(app_id);
}

uint16_t rpc_ble_create_char(uint8_t app_id, const uint8_t uuid[16], uint8_t uuid_length, uint8_t properties, uint32_t permissions)
{
  log_d("rpc_ble_create_char called");
  ble_char_t CHAR;
  CHAR.properties = properties;
  CHAR.permissions = permissions;
  CHAR.uuid_length = uuid_length;
  memcpy(CHAR.uuid, uuid, uuid_length);
  return ble_create_char(app_id, CHAR);
}

uint16_t rpc_ble_create_desc(uint8_t app_id, uint16_t char_handle, const uint8_t uuid[16], uint8_t uuid_length, uint8_t flags, uint32_t permissions, uint16_t value_length, const binary_t *p_value)
{
  log_d("rpc_ble_create_desc called");
  ble_desc_t desc;
  desc.flags = flags;
  desc.permissions = permissions;
  desc.uuid_length = uuid_length;
  memcpy(desc.uuid, uuid, uuid_length);
  desc.vlaue_length = value_length;
  if (p_value != NULL)
  {
    desc.p_value = p_value->data;
  }
  else
  {
    desc.p_value = NULL;
  }
  return ble_create_desc(app_id, char_handle, desc);
}

binary_t *rpc_ble_server_get_attr_value(uint8_t app_id, uint16_t attr_handle)
{
  log_d("rpc_ble_server_get_attr_value called\n\r");
  binary_t *result = NULL;
  uint8_t *p_value = NULL;
  uint16_t value_length = 0;
  value_length = ble_server_get_attr_value(app_id, attr_handle, &p_value);
  if (p_value != NULL)
  {
    log_d("value_length: %d", value_length);
    result = (binary_t *)erpc_malloc(sizeof(binary_t));
    uint8_t *value = (uint8_t *)erpc_malloc(sizeof(uint8_t) * value_length);
    memcpy(value, p_value, value_length);
    result->dataLength = value_length;
    result->data = value;
  }
  return result;
}

bool rpc_server_send_data(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, const binary_t *data, RPC_T_GATT_PDU_TYPE pdu_type)
{
  log_d("rpc_server_send_data called");
  return server_send_data(conn_id, service_id, attrib_index, data->data, data->dataLength, (T_GATT_PDU_TYPE)pdu_type);
}

bool rpc_server_exec_write_confirm(uint8_t conn_id, uint16_t cause, uint16_t handle)
{
  log_d("rpc_server_exec_write_confirm called");
  return server_exec_write_confirm(conn_id, cause, handle);
}

bool rpc_server_attr_write_confirm(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, RPC_T_APP_RESULT cause)
{
  log_d("rpc_server_attr_write_confirm called");
  return server_attr_write_confirm(conn_id, cause, attrib_index, (T_APP_RESULT)cause);
}

bool rpc_server_attr_read_confirm(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, const binary_t *data, RPC_T_APP_RESULT cause)
{
  log_d("rpc_server_attr_read_confirm called");
  return server_attr_read_confirm(conn_id, service_id, attrib_index, data->data, data->dataLength, (T_APP_RESULT)cause);
}
//@}

//! @name rpc_gap_storage
//@{
uint32_t rpc_flash_save_local_name(const RPC_T_LOCAL_NAME *p_data)
{
  log_d("rpc_flash_save_local_name called");
  return flash_save_local_name(p_data);
}

uint32_t rpc_flash_load_local_name(RPC_T_LOCAL_NAME *p_data)
{
  log_d("rpc_flash_load_local_name called");
  return flash_load_local_name(p_data);
}

uint32_t rpc_flash_save_local_appearance(const RPC_T_LOCAL_APPEARANCE *p_data)
{
  log_d("rpc_flash_load_local_name called");
  return flash_save_local_appearance(p_data);
}

uint32_t rpc_flash_load_local_appearance(const RPC_T_LOCAL_APPEARANCE *p_data)
{
  log_d("rpc_flash_load_local_appearance called");
  return flash_load_local_appearance(p_data);
}

RPC_T_LE_KEY_ENTRY *rpc_le_find_key_entry(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type)
{
  log_d("rpc_le_find_key_entry called");
  return le_find_key_entry(bd_addr, bd_type);
}

RPC_T_LE_KEY_ENTRY *rpc_le_find_key_entry_by_idx(uint8_t idx)
{
  log_d("rpc_le_find_key_entry called");
  return le_find_key_entry_by_idx(idx);
}

uint8_t rpc_le_get_bond_dev_num(void)
{
  log_d("rpc_le_get_bond_dev_num called");
  return le_get_bond_dev_num();
}

RPC_T_LE_KEY_ENTRY *rpc_le_get_low_priority_bond(void)
{
  log_d("rpc_le_get_low_priority_bond called");
  return le_get_low_priority_bond();
}

RPC_T_LE_KEY_ENTRY *rpc_le_get_high_priority_bond(void)
{
  log_d("rpc_le_get_high_priority_bond called");
  return le_get_high_priority_bond();
}

bool rpc_le_set_high_priority_bond(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type)
{
  log_d("rpc_le_set_high_priority_bond called");
  return le_set_high_priority_bond(bd_addr, bd_type);
}

bool rpc_le_resolve_random_address(const uint8_t unresolved_addr[6], uint8_t resolved_addr[6], RPC_T_GAP_IDENT_ADDR_TYPE *resolved_addr_type)
{
  log_d("rpc_le_resolve_random_address called");
  return le_resolve_random_address(unresolved_addr, resolved_addr, resolved_addr_type);
}

bool rpc_le_get_cccd_data(const RPC_T_LE_KEY_ENTRY *p_entry, RPC_T_LE_CCCD *p_data)
{
  log_d("rpc_le_get_cccd_data called");
  return le_get_cccd_data(p_entry, p_data);
}

bool rpc_le_gen_bond_dev(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type, RPC_T_GAP_LOCAL_ADDR_TYPE local_bd_type, const binary_t *local_ltk, RPC_T_LE_KEY_TYPE key_type, const RPC_T_LE_CCCD *p_cccd)
{
  log_d("le_gen_bond_dev called");
  return le_gen_bond_dev(bd_addr, bd_type, local_bd_type, local_ltk->dataLength, local_ltk->data, key_type, p_cccd);
}

uint16_t rpc_le_get_dev_bond_info_len(void)
{
  log_d("rpc_le_get_dev_bond_info_len called");
  return le_get_dev_bond_info_len();
}

RPC_T_LE_KEY_ENTRY *rpc_le_set_dev_bond_info(const binary_t *p_data, bool *exist)
{
  log_d("rpc_le_set_dev_bond_info called");
  return le_set_dev_bond_info(p_data->dataLength, p_data->data, exist);
}

bool rpc_le_get_dev_bond_info(const RPC_T_LE_KEY_ENTRY *p_entry, binary_t *p_data)
{
  log_d("rpc_le_set_dev_bond_info called");
  p_data->dataLength = 1;
  return le_get_dev_bond_info(p_entry, p_data->data);
}
//@}
