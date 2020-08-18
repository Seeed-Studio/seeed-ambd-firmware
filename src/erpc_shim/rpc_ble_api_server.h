/*
 * Generated by erpcgen 1.7.4 on Tue Aug 18 10:43:04 2020.
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
    /*! @brief Server shim for _Ble_HostInitialize of host interface. */
    erpc_status_t _Ble_HostInitialize_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
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

    /*! @brief Server shim for rpc_modify_white_list of rpc_gap_le interface. */
    erpc_status_t rpc_modify_white_list_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

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
};

extern "C" {
#else
#include "rpc_ble_api.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_host_service(void);

void destroy_host_service(void);

erpc_service_t create_rpc_gap_le_service(void);

void destroy_rpc_gap_le_service(void);

erpc_service_t create_rpc_gap_config_service(void);

void destroy_rpc_gap_config_service(void);

erpc_service_t create_rpc_gap_adv_service(void);

void destroy_rpc_gap_adv_service(void);

erpc_service_t create_rpc_gap_scan_service(void);

void destroy_rpc_gap_scan_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _rpc_ble_api_server_h_
