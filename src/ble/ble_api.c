
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


uint8_t _Ble_HostInitialize(void)
{
  //log_d("_Ble_HostInitialize called");
  return 0;
}

//! @name rpc_gap_adv
//@{

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
  uint8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = le_adv_get_param(param, p_value);
  value->data =(uint8_t *)p_value;
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
  log_d("rpc_le_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PARAM_SIZE;
  int8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = le_get_gap_param(param, p_value);
  value->data =(uint8_t *)p_value;
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

RPC_T_GAP_CAUSE rpc_le_set_host_chann_classif(const binary_t *p_channel_map)
{
  log_d("rpc_le_set_host_chann_classif called");
  return le_set_host_chann_classif(p_channel_map->data);
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
  value->dataLength = DEFAULT_PARAM_SIZE;
  uint8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = le_scan_get_param(param, p_value);
  value->data =(uint8_t *)p_value;
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

//! @name rpc_gap
//@{

RPC_T_GAP_CAUSE rpc_gap_set_param(RPC_T_GAP_PARAM_TYPE param, const binary_t *value)
{
  log_d("rpc_gap_set_param called");
  return gap_set_param(param, value->dataLength, value->data);
}

RPC_T_GAP_CAUSE rpc_gap_get_param(RPC_T_GAP_PARAM_TYPE param, binary_t *value)
{
  log_d("rpc_gap_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_BT_ADDR_SIZE;
  uint8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = rpc_gap_get_param(param, p_value);
  value->data =(uint8_t *)p_value;
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
  log_d("rpc_le_bond_get_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PARAM_SIZE;
  uint8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = le_bond_get_param(param, p_value);
  value->data =(uint8_t *)p_value;
  return ret;
}
RPC_T_GAP_CAUSE rpc_le_bond_pair(uint8_t conn_id)
{
  log_d("rpc_le_bond_pair called");
  return rpc_le_bond_pair(conn_id);
}

RPC_T_GAP_CAUSE rpc_le_bond_get_display_key(uint8_t conn_id, binary_t *key)
{
  log_d("rpc_le_bond_get_display_key called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  key->dataLength = DEFAULT_PKEY_SIZE;
  uint8_t *p_value = (uint8_t *)erpc_malloc(key->dataLength * sizeof(uint8_t));
  ret = le_bond_get_display_key(conn_id, p_value);
  key->data = p_value;
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
  log_d("rpc_le_get_conn_param called");
  RPC_T_GAP_CAUSE ret = RPC_GAP_CAUSE_SUCCESS;
  value->dataLength = DEFAULT_PKEY_SIZE;
  uint8_t *p_value = (uint8_t *)erpc_malloc(value->dataLength * sizeof(uint8_t));
  ret = le_get_conn_param(param, p_value, conn_id);
  value->data =(uint8_t *)p_value;
  return ret;
}

bool rpc_le_get_conn_info(uint8_t conn_id, RPC_T_GAP_CONN_INFO *p_conn_info)
{
  log_d("rpc_le_get_conn_info called");
  return rpc_le_get_conn_info(conn_id, p_conn_info);
}

bool rpc_le_get_conn_addr(uint8_t conn_id, uint8_t bd_addr[6], uint8_t *bd_type)
{
  log_d("rpc_le_get_conn_info called");
  return le_get_conn_addr(conn_id, bd_addr, bd_type);
}

bool rpc_le_get_conn_id(const uint8_t bd_addr[6], uint8_t bd_type, uint8_t *p_conn_id)
{
  log_d("rpc_le_get_conn_id called");
  return le_get_conn_addr(bd_addr, bd_type, p_conn_id);
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

RPC_T_GAP_CAUSE rpc_le_connect(uint8_t init_phys, const uint8_t remote_bd[6], RPC_T_GAP_CONN_PARAM_TYPE remote_bd_type, RPC_T_GAP_LOCAL_ADDR_TYPE local_bd_type, uint16_t scan_timeout)
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