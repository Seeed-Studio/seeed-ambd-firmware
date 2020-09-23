/*
 * Generated by erpcgen 1.7.4 on Wed Sep 23 17:41:02 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_ble_api_h_)
#define _rpc_ble_api_h_

// Common types header file
#include "rpc_system_header.h"

/*! @brief rpc_ble_host identifiers */
enum _rpc_ble_host_ids
{
    krpc_ble_host_service_id = 1,
    krpc_ble_host_rpc_ble_init_id = 1,
    krpc_ble_host_rpc_ble_start_id = 2,
    krpc_ble_host_rpc_ble_deinit_id = 3,
};

/*! @brief rpc_gap identifiers */
enum _rpc_gap_ids
{
    krpc_gap_service_id = 2,
    krpc_gap_rpc_gap_set_param_id = 1,
    krpc_gap_rpc_gap_get_param_id = 2,
    krpc_gap_rpc_gap_set_pairable_mode_id = 3,
};

/*! @brief rpc_gap_bone identifiers */
enum _rpc_gap_bone_ids
{
    krpc_gap_bone_service_id = 3,
    krpc_gap_bone_rpc_le_bond_set_param_id = 1,
    krpc_gap_bone_rpc_le_bond_get_param_id = 2,
    krpc_gap_bone_rpc_le_bond_pair_id = 3,
    krpc_gap_bone_rpc_le_bond_get_display_key_id = 4,
    krpc_gap_bone_rpc_le_bond_passkey_input_confirm_id = 5,
    krpc_gap_bone_rpc_le_bond_oob_input_confirm_id = 6,
    krpc_gap_bone_rpc_le_bond_just_work_confirm_id = 7,
    krpc_gap_bone_rpc_le_bond_passkey_display_confirm_id = 8,
    krpc_gap_bone_rpc_le_bond_user_confirm_id = 9,
    krpc_gap_bone_rpc_le_bond_cfg_local_key_distribute_id = 10,
    krpc_gap_bone_rpc_le_bond_clear_all_keys_id = 11,
    krpc_gap_bone_rpc_le_bond_delete_by_idx_id = 12,
    krpc_gap_bone_rpc_le_bond_delete_by_bd_id = 13,
    krpc_gap_bone_rpc_le_bond_get_sec_level_id = 14,
};

/*! @brief rpc_gap_le identifiers */
enum _rpc_gap_le_ids
{
    krpc_gap_le_service_id = 4,
    krpc_gap_le_rpc_le_gap_init_id = 1,
    krpc_gap_le_rpc_le_gap_msg_info_way_id = 2,
    krpc_gap_le_rpc_le_get_max_link_num_id = 3,
    krpc_gap_le_rpc_le_set_gap_param_id = 4,
    krpc_gap_le_rpc_le_get_gap_param_id = 5,
    krpc_gap_le_rpc_le_modify_white_list_id = 6,
    krpc_gap_le_rpc_le_gen_rand_addr_id = 7,
    krpc_gap_le_rpc_le_set_rand_addr_id = 8,
    krpc_gap_le_rpc_le_cfg_local_identity_address_id = 9,
    krpc_gap_le_rpc_le_set_host_chann_classif_id = 10,
    krpc_gap_le_rpc_le_write_default_data_len_id = 11,
};

/*! @brief rpc_gap_config identifiers */
enum _rpc_gap_config_ids
{
    krpc_gap_config_service_id = 5,
    krpc_gap_config_rpc_gap_config_cccd_not_check_id = 1,
    krpc_gap_config_rpc_gap_config_ccc_bits_count_id = 2,
    krpc_gap_config_rpc_gap_config_max_attribute_table_count_id = 3,
    krpc_gap_config_rpc_gap_config_max_mtu_size_id = 4,
    krpc_gap_config_rpc_gap_config_bte_pool_size_id = 5,
    krpc_gap_config_rpc_gap_config_bt_report_buf_num_id = 6,
    krpc_gap_config_rpc_gap_config_le_key_storage_flag_id = 7,
    krpc_gap_config_rpc_gap_config_max_le_paired_device_id = 8,
    krpc_gap_config_rpc_gap_config_max_le_link_num_id = 9,
};

/*! @brief rpc_gap_adv identifiers */
enum _rpc_gap_adv_ids
{
    krpc_gap_adv_service_id = 6,
    krpc_gap_adv_rpc_le_adv_set_param_id = 1,
    krpc_gap_adv_rpc_le_adv_get_param_id = 2,
    krpc_gap_adv_rpc_le_adv_start_id = 3,
    krpc_gap_adv_rpc_le_adv_stop_id = 4,
    krpc_gap_adv_rpc_le_adv_update_param_id = 5,
};

/*! @brief rpc_gap_scan identifiers */
enum _rpc_gap_scan_ids
{
    krpc_gap_scan_service_id = 7,
    krpc_gap_scan_rpc_le_scan_set_param_id = 1,
    krpc_gap_scan_rpc_le_scan_get_param_id = 2,
    krpc_gap_scan_rpc_le_scan_start_id = 3,
    krpc_gap_scan_rpc_le_scan_timer_start_id = 4,
    krpc_gap_scan_rpc_le_scan_stop_id = 5,
    krpc_gap_scan_rpc_le_scan_info_filter_id = 6,
};

/*! @brief rpc_gap_conn identifiers */
enum _rpc_gap_conn_ids
{
    krpc_gap_conn_service_id = 8,
    krpc_gap_conn_rpc_le_get_conn_param_id = 1,
    krpc_gap_conn_rpc_le_get_conn_info_id = 2,
    krpc_gap_conn_rpc_le_get_conn_addr_id = 3,
    krpc_gap_conn_rpc_le_get_conn_id_id = 4,
    krpc_gap_conn_rpc_le_get_active_link_num_id = 5,
    krpc_gap_conn_rpc_le_get_idle_link_num_id = 6,
    krpc_gap_conn_rpc_le_disconnect_id = 7,
    krpc_gap_conn_rpc_le_read_rssi_id = 8,
    krpc_gap_conn_rpc_le_set_data_len_id = 9,
    krpc_gap_conn_rpc_le_set_phy_id = 10,
    krpc_gap_conn_rpc_le_set_conn_param_id = 11,
    krpc_gap_conn_rpc_le_connect_id = 12,
    krpc_gap_conn_rpc_le_update_conn_param_id = 13,
};

/*! @brief rpc_gap_storage identifiers */
enum _rpc_gap_storage_ids
{
    krpc_gap_storage_service_id = 9,
    krpc_gap_storage_rpc_flash_save_local_name_id = 1,
    krpc_gap_storage_rpc_flash_load_local_name_id = 2,
    krpc_gap_storage_rpc_flash_save_local_appearance_id = 3,
    krpc_gap_storage_rpc_flash_load_local_appearance_id = 4,
    krpc_gap_storage_rpc_le_find_key_entry_id = 5,
    krpc_gap_storage_rpc_le_find_key_entry_by_idx_id = 6,
    krpc_gap_storage_rpc_le_get_bond_dev_num_id = 7,
    krpc_gap_storage_rpc_le_get_low_priority_bond_id = 8,
    krpc_gap_storage_rpc_le_get_high_priority_bond_id = 9,
    krpc_gap_storage_rpc_le_set_high_priority_bond_id = 10,
    krpc_gap_storage_rpc_le_resolve_random_address_id = 11,
    krpc_gap_storage_rpc_le_get_cccd_data_id = 12,
    krpc_gap_storage_rpc_le_gen_bond_dev_id = 13,
    krpc_gap_storage_rpc_le_get_dev_bond_info_len_id = 14,
    krpc_gap_storage_rpc_le_set_dev_bond_info_id = 15,
    krpc_gap_storage_rpc_le_get_dev_bond_info_id = 16,
};

/*! @brief rpc_gatt_client identifiers */
enum _rpc_gatt_client_ids
{
    krpc_gatt_client_service_id = 10,
    krpc_gatt_client_rpc_ble_client_init_id = 1,
    krpc_gatt_client_rpc_ble_add_client_id = 2,
    krpc_gatt_client_rpc_client_init_id = 3,
    krpc_gatt_client_rpc_client_all_primary_srv_discovery_id = 4,
    krpc_gatt_client_rpc_client_by_uuid_srv_discovery_id = 5,
    krpc_gatt_client_rpc_client_by_uuid128_srv_discovery_id = 6,
    krpc_gatt_client_rpc_client_relationship_discovery_id = 7,
    krpc_gatt_client_rpc_client_all_char_discovery_id = 8,
    krpc_gatt_client_rpc_client_by_uuid_char_discovery_id = 9,
    krpc_gatt_client_rpc_client_by_uuid128_char_discovery_id = 10,
    krpc_gatt_client_rpc_client_all_char_descriptor_discovery_id = 11,
    krpc_gatt_client_rpc_client_attr_read_id = 12,
    krpc_gatt_client_rpc_client_attr_read_using_uuid_id = 13,
    krpc_gatt_client_rpc_client_attr_write_id = 14,
    krpc_gatt_client_rpc_client_attr_ind_confirm_id = 15,
};

/*! @brief rpc_gatt_server identifiers */
enum _rpc_gatt_server_ids
{
    krpc_gatt_server_service_id = 11,
    krpc_gatt_server_rpc_ble_server_init_id = 1,
    krpc_gatt_server_rpc_ble_create_service_id = 2,
    krpc_gatt_server_rpc_ble_delete_service_id = 3,
    krpc_gatt_server_rpc_ble_service_start_id = 4,
    krpc_gatt_server_rpc_ble_get_servie_handle_id = 5,
    krpc_gatt_server_rpc_ble_create_char_id = 6,
    krpc_gatt_server_rpc_ble_create_desc_id = 7,
    krpc_gatt_server_rpc_server_send_data_id = 8,
    krpc_gatt_server_rpc_ble_server_get_attr_value_id = 9,
    krpc_gatt_server_rpc_server_exec_write_confirm_id = 10,
    krpc_gatt_server_rpc_server_attr_write_confirm_id = 11,
    krpc_gatt_server_rpc_server_attr_read_confirm_id = 12,
};

#if defined(__cplusplus)
extern "C" {
#endif

//! @name rpc_ble_host
//@{
bool rpc_ble_init(void);

void rpc_ble_start(void);

void rpc_ble_deinit(void);
//@}

//! @name rpc_gap
//@{
RPC_T_GAP_CAUSE rpc_gap_set_param(RPC_T_GAP_PARAM_TYPE param, const binary_t * value);

RPC_T_GAP_CAUSE rpc_gap_get_param(RPC_T_GAP_PARAM_TYPE param, binary_t * value);

RPC_T_GAP_CAUSE rpc_gap_set_pairable_mode(void);
//@}

//! @name rpc_gap_bone
//@{
RPC_T_GAP_CAUSE rpc_le_bond_set_param(RPC_T_LE_BOND_PARAM_TYPE param, const binary_t * value);

RPC_T_GAP_CAUSE rpc_le_bond_get_param(RPC_T_LE_BOND_PARAM_TYPE param, binary_t * value);

RPC_T_GAP_CAUSE rpc_le_bond_pair(uint8_t conn_id);

RPC_T_GAP_CAUSE rpc_le_bond_get_display_key(uint8_t conn_id, uint32_t * key);

RPC_T_GAP_CAUSE rpc_le_bond_passkey_input_confirm(uint8_t conn_id, uint32_t passcode, RPC_T_GAP_CFM_CAUSE cause);

RPC_T_GAP_CAUSE rpc_le_bond_oob_input_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause);

RPC_T_GAP_CAUSE rpc_le_bond_just_work_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause);

RPC_T_GAP_CAUSE rpc_le_bond_passkey_display_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause);

RPC_T_GAP_CAUSE rpc_le_bond_user_confirm(uint8_t conn_id, RPC_T_GAP_CFM_CAUSE cause);

RPC_T_GAP_CAUSE rpc_le_bond_cfg_local_key_distribute(uint8_t init_dist, uint8_t rsp_dist);

void rpc_le_bond_clear_all_keys(void);

RPC_T_GAP_CAUSE rpc_le_bond_delete_by_idx(uint8_t idx);

RPC_T_GAP_CAUSE rpc_le_bond_delete_by_bd(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type);

RPC_T_GAP_CAUSE rpc_le_bond_get_sec_level(uint8_t conn_id, RPC_T_GAP_SEC_LEVEL * sec_type);
//@}

//! @name rpc_gap_le
//@{
bool rpc_le_gap_init(uint8_t link_num);

void rpc_le_gap_msg_info_way(bool use_msg);

uint8_t rpc_le_get_max_link_num(void);

RPC_T_GAP_CAUSE rpc_le_set_gap_param(RPC_T_GAP_LE_PARAM_TYPE param, const binary_t * value);

RPC_T_GAP_CAUSE rpc_le_get_gap_param(RPC_T_GAP_LE_PARAM_TYPE param, binary_t * value);

RPC_T_GAP_CAUSE rpc_le_modify_white_list(RPC_T_GAP_WHITE_LIST_OP operation, const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type);

RPC_T_GAP_CAUSE rpc_le_gen_rand_addr(RPC_T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t random_bd[6]);

RPC_T_GAP_CAUSE rpc_le_set_rand_addr(const uint8_t random_bd[6]);

RPC_T_GAP_CAUSE rpc_le_cfg_local_identity_address(const uint8_t addr[6], RPC_T_GAP_IDENT_ADDR_TYPE ident_addr_type);

RPC_T_GAP_CAUSE rpc_le_set_host_chann_classif(uint8_t p_channel_map);

RPC_T_GAP_CAUSE rpc_le_write_default_data_len(uint16_t tx_octets, uint16_t tx_time);
//@}

//! @name rpc_gap_config
//@{
void rpc_gap_config_cccd_not_check(RPC_T_GAP_CONFIG_GATT_CCCD_NOT_CHECK cccd_not_check_flag);

void rpc_gap_config_ccc_bits_count(uint8_t gatt_server_ccc_bits_count, uint8_t gatt_storage_ccc_bits_count);

void rpc_gap_config_max_attribute_table_count(uint8_t gatt_max_attribute_table_count);

void rpc_gap_config_max_mtu_size(uint16_t att_max_mtu_size);

void rpc_gap_config_bte_pool_size(uint8_t bte_pool_size);

void rpc_gap_config_bt_report_buf_num(uint8_t bt_report_buf_num);

void rpc_gap_config_le_key_storage_flag(uint16_t le_key_storage_flag);

void rpc_gap_config_max_le_paired_device(uint8_t max_le_paired_device);

void rpc_gap_config_max_le_link_num(uint8_t le_link_num);
//@}

//! @name rpc_gap_adv
//@{
RPC_T_GAP_CAUSE rpc_le_adv_set_param(RPC_T_LE_ADV_PARAM_TYPE param, const binary_t * value);

RPC_T_GAP_CAUSE rpc_le_adv_get_param(RPC_T_LE_ADV_PARAM_TYPE param, binary_t * value);

RPC_T_GAP_CAUSE rpc_le_adv_start(void);

RPC_T_GAP_CAUSE rpc_le_adv_stop(void);

RPC_T_GAP_CAUSE rpc_le_adv_update_param(void);
//@}

//! @name rpc_gap_scan
//@{
RPC_T_GAP_CAUSE rpc_le_scan_set_param(RPC_T_LE_SCAN_PARAM_TYPE param, const binary_t * value);

RPC_T_GAP_CAUSE rpc_le_scan_get_param(RPC_T_LE_SCAN_PARAM_TYPE param, binary_t * value);

RPC_T_GAP_CAUSE rpc_le_scan_start(void);

RPC_T_GAP_CAUSE rpc_le_scan_timer_start(uint32_t tick);

RPC_T_GAP_CAUSE rpc_le_scan_stop(void);

bool rpc_le_scan_info_filter(bool enable, uint8_t offset, uint8_t len, const uint8_t p_filter[31]);
//@}

//! @name rpc_gap_conn
//@{
RPC_T_GAP_CAUSE rpc_le_get_conn_param(RPC_T_LE_CONN_PARAM_TYPE param, binary_t * value, uint8_t conn_id);

bool rpc_le_get_conn_info(uint8_t conn_id, RPC_T_GAP_CONN_INFO * p_conn_info);

bool rpc_le_get_conn_addr(uint8_t conn_id, uint8_t bd_addr[6], uint8_t * bd_type);

bool rpc_le_get_conn_id(const uint8_t bd_addr[6], uint8_t bd_type, uint8_t * p_conn_id);

uint8_t rpc_le_get_active_link_num(void);

uint8_t rpc_le_get_idle_link_num(void);

RPC_T_GAP_CAUSE rpc_le_disconnect(uint8_t conn_id);

RPC_T_GAP_CAUSE rpc_le_read_rssi(uint8_t conn_id);

RPC_T_GAP_CAUSE rpc_le_set_data_len(uint8_t conn_id, uint16_t tx_octets, uint16_t tx_time);

RPC_T_GAP_CAUSE rpc_le_set_phy(uint8_t conn_id, uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys, RPC_T_GAP_PHYS_OPTIONS phy_options);

RPC_T_GAP_CAUSE rpc_le_set_conn_param(RPC_T_GAP_CONN_PARAM_TYPE conn_type, const RPC_T_GAP_LE_CONN_REQ_PARAM * p_conn_param);

RPC_T_GAP_CAUSE rpc_le_connect(uint8_t init_phys, const uint8_t remote_bd[6], RPC_T_GAP_REMOTE_ADDR_TYPE remote_bd_type, RPC_T_GAP_LOCAL_ADDR_TYPE local_bd_type, uint16_t scan_timeout);

RPC_T_GAP_CAUSE rpc_le_update_conn_param(uint8_t conn_id, uint16_t conn_interval_min, uint16_t conn_interval_max, uint16_t conn_latency, uint16_t supervision_timeout, uint16_t ce_length_min, uint16_t ce_length_max);
//@}

//! @name rpc_gap_storage
//@{
uint32_t rpc_flash_save_local_name(const RPC_T_LOCAL_NAME * p_data);

uint32_t rpc_flash_load_local_name(RPC_T_LOCAL_NAME * p_data);

uint32_t rpc_flash_save_local_appearance(const RPC_T_LOCAL_APPEARANCE * p_data);

uint32_t rpc_flash_load_local_appearance(RPC_T_LOCAL_APPEARANCE * p_data);

RPC_T_LE_KEY_ENTRY * rpc_le_find_key_entry(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type);

RPC_T_LE_KEY_ENTRY * rpc_le_find_key_entry_by_idx(uint8_t idx);

uint8_t rpc_le_get_bond_dev_num(void);

RPC_T_LE_KEY_ENTRY * rpc_le_get_low_priority_bond(void);

RPC_T_LE_KEY_ENTRY * rpc_le_get_high_priority_bond(void);

bool rpc_le_set_high_priority_bond(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type);

bool rpc_le_resolve_random_address(const uint8_t unresolved_addr[6], uint8_t resolved_addr[6], RPC_T_GAP_IDENT_ADDR_TYPE * resolved_addr_type);

bool rpc_le_get_cccd_data(const RPC_T_LE_KEY_ENTRY * p_entry, RPC_T_LE_CCCD * p_data);

bool rpc_le_gen_bond_dev(const uint8_t bd_addr[6], RPC_T_GAP_REMOTE_ADDR_TYPE bd_type, RPC_T_GAP_LOCAL_ADDR_TYPE local_bd_type, const binary_t * local_ltk, RPC_T_LE_KEY_TYPE key_type, const RPC_T_LE_CCCD * p_cccd);

uint16_t rpc_le_get_dev_bond_info_len(void);

RPC_T_LE_KEY_ENTRY * rpc_le_set_dev_bond_info(const binary_t * p_data, bool * exist);

bool rpc_le_get_dev_bond_info(const RPC_T_LE_KEY_ENTRY * p_entry, binary_t * p_data);
//@}

//! @name rpc_gatt_client
//@{
bool rpc_ble_client_init(uint8_t num);

uint8_t rpc_ble_add_client(uint8_t app_id, uint8_t link_num);

void rpc_client_init(uint8_t client_num);

RPC_T_GAP_CAUSE rpc_client_all_primary_srv_discovery(uint8_t conn_id, uint8_t client_id);

RPC_T_GAP_CAUSE rpc_client_by_uuid_srv_discovery(uint8_t conn_id, uint8_t client_id, uint16_t uuid16);

RPC_T_GAP_CAUSE rpc_client_by_uuid128_srv_discovery(uint8_t conn_id, uint8_t client_id, const uint8_t p_uuid128[16]);

RPC_T_GAP_CAUSE rpc_client_relationship_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle);

RPC_T_GAP_CAUSE rpc_client_all_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle);

RPC_T_GAP_CAUSE rpc_client_by_uuid_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, uint16_t uuid16);

RPC_T_GAP_CAUSE rpc_client_by_uuid128_char_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, const uint8_t p_uuid128[16]);

RPC_T_GAP_CAUSE rpc_client_all_char_descriptor_discovery(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle);

RPC_T_GAP_CAUSE rpc_client_attr_read(uint8_t conn_id, uint8_t client_id, uint16_t handle);

RPC_T_GAP_CAUSE rpc_client_attr_read_using_uuid(uint8_t conn_id, uint8_t client_id, uint16_t start_handle, uint16_t end_handle, uint16_t uuid16, const uint8_t p_uuid128[16]);

RPC_T_GAP_CAUSE rpc_client_attr_write(uint8_t conn_id, uint8_t client_id, RPC_T_GATT_WRITE_TYPE write_type, uint16_t handle, const binary_t * data);

RPC_T_GAP_CAUSE rpc_client_attr_ind_confirm(uint8_t conn_id);
//@}

//! @name rpc_gatt_server
//@{
bool rpc_ble_server_init(uint8_t num);

uint8_t rpc_ble_create_service(const uint8_t uuid[16], uint8_t uuid_length, bool is_primary);

bool rpc_ble_delete_service(uint8_t app_id);

uint8_t rpc_ble_service_start(uint8_t app_id);

uint8_t rpc_ble_get_servie_handle(uint8_t app_id);

uint16_t rpc_ble_create_char(uint8_t app_id, const uint8_t uuid[16], uint8_t uuid_length, uint8_t properties, uint32_t permissions);

uint16_t rpc_ble_create_desc(uint8_t app_id, uint16_t char_handle, const uint8_t uuid[16], uint8_t uuid_length, uint8_t flags, uint32_t permissions, uint16_t value_length, const binary_t * p_value);

bool rpc_server_send_data(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, const binary_t * data, RPC_T_GATT_PDU_TYPE pdu_type);

binary_t * rpc_ble_server_get_attr_value(uint8_t app_id, uint16_t attr_handle);

bool rpc_server_exec_write_confirm(uint8_t conn_id, uint16_t cause, uint16_t handle);

bool rpc_server_attr_write_confirm(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, RPC_T_APP_RESULT cause);

bool rpc_server_attr_read_confirm(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index, const binary_t * data, RPC_T_APP_RESULT cause);
//@}

#if defined(__cplusplus)
}
#endif

#endif // _rpc_ble_api_h_
