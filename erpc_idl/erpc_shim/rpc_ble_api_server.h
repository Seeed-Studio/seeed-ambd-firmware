/*
 * Generated by erpcgen 1.7.4 on Wed Aug 26 10:21:10 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_ble_api_server_h_)
#define _rpc_ble_api_server_h_

#ifdef __cplusplus
#include "erpc_server.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_ble_api.h"
#include <stdint.h>
#include <stdbool.h>
}

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


/*!
 * @brief Service subclass for host.
 */
class host_service : public erpc::Service
{
public:
    host_service() : Service(khost_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_ble_init of host interface. */
    erpc_status_t rpc_ble_init_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_ble_start of host interface. */
    erpc_status_t rpc_ble_start_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_ble_deinit of host interface. */
    erpc_status_t rpc_ble_deinit_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap.
 */
class rpc_gap_service : public erpc::Service
{
public:
    rpc_gap_service() : Service(krpc_gap_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_gap_set_param of rpc_gap interface. */
    erpc_status_t rpc_gap_set_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_get_param of rpc_gap interface. */
    erpc_status_t rpc_gap_get_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_set_pairable_mode of rpc_gap interface. */
    erpc_status_t rpc_gap_set_pairable_mode_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_bone.
 */
class rpc_gap_bone_service : public erpc::Service
{
public:
    rpc_gap_bone_service() : Service(krpc_gap_bone_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_le_bond_set_param of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_set_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_get_param of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_get_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_pair of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_pair_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_get_display_key of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_get_display_key_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_passkey_input_confirm of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_passkey_input_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_oob_input_confirm of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_oob_input_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_just_work_confirm of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_just_work_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_passkey_display_confirm of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_passkey_display_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_user_confirm of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_user_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_cfg_local_key_distribute of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_cfg_local_key_distribute_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_clear_all_keys of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_clear_all_keys_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_delete_by_idx of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_delete_by_idx_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_delete_by_bd of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_delete_by_bd_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_bond_get_sec_level of rpc_gap_bone interface. */
    erpc_status_t rpc_le_bond_get_sec_level_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_le.
 */
class rpc_gap_le_service : public erpc::Service
{
public:
    rpc_gap_le_service() : Service(krpc_gap_le_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_le_gap_init of rpc_gap_le interface. */
    erpc_status_t rpc_le_gap_init_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_gap_msg_info_way of rpc_gap_le interface. */
    erpc_status_t rpc_le_gap_msg_info_way_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_max_link_num of rpc_gap_le interface. */
    erpc_status_t rpc_le_get_max_link_num_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_gap_param of rpc_gap_le interface. */
    erpc_status_t rpc_le_set_gap_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_gap_param of rpc_gap_le interface. */
    erpc_status_t rpc_le_get_gap_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_modify_white_list of rpc_gap_le interface. */
    erpc_status_t rpc_le_modify_white_list_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_gen_rand_addr of rpc_gap_le interface. */
    erpc_status_t rpc_le_gen_rand_addr_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_rand_addr of rpc_gap_le interface. */
    erpc_status_t rpc_le_set_rand_addr_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_cfg_local_identity_address of rpc_gap_le interface. */
    erpc_status_t rpc_le_cfg_local_identity_address_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_host_chann_classif of rpc_gap_le interface. */
    erpc_status_t rpc_le_set_host_chann_classif_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_write_default_data_len of rpc_gap_le interface. */
    erpc_status_t rpc_le_write_default_data_len_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_config.
 */
class rpc_gap_config_service : public erpc::Service
{
public:
    rpc_gap_config_service() : Service(krpc_gap_config_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_gap_config_cccd_not_check of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_cccd_not_check_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_ccc_bits_count of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_ccc_bits_count_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_max_attribute_table_count of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_max_attribute_table_count_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_max_mtu_size of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_max_mtu_size_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_bte_pool_size of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_bte_pool_size_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_bt_report_buf_num of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_bt_report_buf_num_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_le_key_storage_flag of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_le_key_storage_flag_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_max_le_paired_device of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_max_le_paired_device_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_gap_config_max_le_link_num of rpc_gap_config interface. */
    erpc_status_t rpc_gap_config_max_le_link_num_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_adv.
 */
class rpc_gap_adv_service : public erpc::Service
{
public:
    rpc_gap_adv_service() : Service(krpc_gap_adv_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_le_adv_set_param of rpc_gap_adv interface. */
    erpc_status_t rpc_le_adv_set_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_adv_get_param of rpc_gap_adv interface. */
    erpc_status_t rpc_le_adv_get_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_adv_start of rpc_gap_adv interface. */
    erpc_status_t rpc_le_adv_start_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_adv_stop of rpc_gap_adv interface. */
    erpc_status_t rpc_le_adv_stop_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_adv_update_param of rpc_gap_adv interface. */
    erpc_status_t rpc_le_adv_update_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_scan.
 */
class rpc_gap_scan_service : public erpc::Service
{
public:
    rpc_gap_scan_service() : Service(krpc_gap_scan_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_le_scan_set_param of rpc_gap_scan interface. */
    erpc_status_t rpc_le_scan_set_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_scan_get_param of rpc_gap_scan interface. */
    erpc_status_t rpc_le_scan_get_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_scan_start of rpc_gap_scan interface. */
    erpc_status_t rpc_le_scan_start_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_scan_stop of rpc_gap_scan interface. */
    erpc_status_t rpc_le_scan_stop_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_scan_info_filter of rpc_gap_scan interface. */
    erpc_status_t rpc_le_scan_info_filter_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gap_conn.
 */
class rpc_gap_conn_service : public erpc::Service
{
public:
    rpc_gap_conn_service() : Service(krpc_gap_conn_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_le_get_conn_param of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_conn_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_conn_info of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_conn_info_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_conn_addr of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_conn_addr_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_conn_id of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_conn_id_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_active_link_num of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_active_link_num_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_get_idle_link_num of rpc_gap_conn interface. */
    erpc_status_t rpc_le_get_idle_link_num_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_disconnect of rpc_gap_conn interface. */
    erpc_status_t rpc_le_disconnect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_read_rssi of rpc_gap_conn interface. */
    erpc_status_t rpc_le_read_rssi_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_data_len of rpc_gap_conn interface. */
    erpc_status_t rpc_le_set_data_len_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_phy of rpc_gap_conn interface. */
    erpc_status_t rpc_le_set_phy_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_set_conn_param of rpc_gap_conn interface. */
    erpc_status_t rpc_le_set_conn_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_connect of rpc_gap_conn interface. */
    erpc_status_t rpc_le_connect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_le_update_conn_param of rpc_gap_conn interface. */
    erpc_status_t rpc_le_update_conn_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

/*!
 * @brief Service subclass for rpc_gatt_client.
 */
class rpc_gatt_client_service : public erpc::Service
{
public:
    rpc_gatt_client_service() : Service(krpc_gatt_client_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_ble_client_init of rpc_gatt_client interface. */
    erpc_status_t rpc_ble_client_init_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_ble_add_client of rpc_gatt_client interface. */
    erpc_status_t rpc_ble_add_client_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_init of rpc_gatt_client interface. */
    erpc_status_t rpc_client_init_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_all_primary_srv_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_all_primary_srv_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_by_uuid_srv_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_by_uuid_srv_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_by_uuid128_srv_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_by_uuid128_srv_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_relationship_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_relationship_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_all_char_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_all_char_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_by_uuid_char_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_by_uuid_char_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_by_uuid128_char_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_by_uuid128_char_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_all_char_descriptor_discovery of rpc_gatt_client interface. */
    erpc_status_t rpc_client_all_char_descriptor_discovery_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_attr_read of rpc_gatt_client interface. */
    erpc_status_t rpc_client_attr_read_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_attr_read_using_uuid of rpc_gatt_client interface. */
    erpc_status_t rpc_client_attr_read_using_uuid_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_attr_write of rpc_gatt_client interface. */
    erpc_status_t rpc_client_attr_write_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_client_attr_ind_confirm of rpc_gatt_client interface. */
    erpc_status_t rpc_client_attr_ind_confirm_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#else
#include "rpc_ble_api.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_host_service(void);

void destroy_host_service(void);

erpc_service_t create_rpc_gap_service(void);

void destroy_rpc_gap_service(void);

erpc_service_t create_rpc_gap_bone_service(void);

void destroy_rpc_gap_bone_service(void);

erpc_service_t create_rpc_gap_le_service(void);

void destroy_rpc_gap_le_service(void);

erpc_service_t create_rpc_gap_config_service(void);

void destroy_rpc_gap_config_service(void);

erpc_service_t create_rpc_gap_adv_service(void);

void destroy_rpc_gap_adv_service(void);

erpc_service_t create_rpc_gap_scan_service(void);

void destroy_rpc_gap_scan_service(void);

erpc_service_t create_rpc_gap_conn_service(void);

void destroy_rpc_gap_conn_service(void);

erpc_service_t create_rpc_gatt_client_service(void);

void destroy_rpc_gatt_client_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _rpc_ble_api_server_h_
