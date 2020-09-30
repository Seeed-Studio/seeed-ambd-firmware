/*
 * Generated by erpcgen 1.7.4 on Wed Sep 30 14:38:25 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_wifi_api_server_h_)
#define _rpc_wifi_api_server_h_

#ifdef __cplusplus
#include "erpc_server.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_wifi_api.h"
#include <stdint.h>
#include <stdbool.h>
}

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


/*!
 * @brief Service subclass for rpc_wifi_drv.
 */
class rpc_wifi_drv_service : public erpc::Service
{
public:
    rpc_wifi_drv_service() : Service(krpc_wifi_drv_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_wifi_connect of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_connect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_connect_bssid of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_connect_bssid_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_disconnect of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_disconnect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_is_connected_to_ap of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_is_connected_to_ap_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_is_up of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_is_up_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_is_ready_to_transceive of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_is_ready_to_transceive_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_mac_address of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_mac_address_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_mac_address of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_mac_address_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_enable_powersave of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_enable_powersave_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_resume_powersave of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_resume_powersave_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_disable_powersave of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_disable_powersave_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_btcoex_set_bt_on of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_btcoex_set_bt_on_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_btcoex_set_bt_off of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_btcoex_set_bt_off_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_associated_client_list of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_associated_client_list_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_ap_bssid of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_ap_bssid_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_ap_info of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_ap_info_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_country of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_country_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_sta_max_data_rate of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_sta_max_data_rate_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_rssi of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_rssi_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_channel of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_channel_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_channel of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_channel_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_change_channel_plan of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_change_channel_plan_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_register_multicast_address of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_register_multicast_address_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_unregister_multicast_address of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_unregister_multicast_address_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_rf_on of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_rf_on_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_rf_off of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_rf_off_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_on of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_on_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_off of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_off_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_mode of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_mode_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_off_fastly of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_off_fastly_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_power_mode of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_power_mode_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_tdma_param of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_tdma_param_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_lps_dtim of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_lps_dtim_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_lps_dtim of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_lps_dtim_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_lps_thresh of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_lps_thresh_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_lps_level of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_lps_level_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_mfp_support of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_mfp_support_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_start_ap of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_start_ap_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_start_ap_with_hidden_ssid of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_start_ap_with_hidden_ssid_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_pscan_chan of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_pscan_chan_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_setting of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_setting_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_network_mode of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_network_mode_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_network_mode of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_network_mode_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_wps_phase of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_wps_phase_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_restart_ap of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_restart_ap_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_config_autoreconnect of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_config_autoreconnect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_autoreconnect of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_autoreconnect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_autoreconnect of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_autoreconnect_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_last_error of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_last_error_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_add_custom_ie of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_add_custom_ie_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_update_custom_ie of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_update_custom_ie_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_del_custom_ie of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_del_custom_ie_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_indicate_mgnt of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_indicate_mgnt_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_drv_ability of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_drv_ability_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_channel_plan of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_channel_plan_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_channel_plan of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_channel_plan_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_enable_forwarding of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_enable_forwarding_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_disable_forwarding of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_disable_forwarding_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_ch_deauth of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_ch_deauth_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_get_band_type of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_get_band_type_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_set_tx_pause_data of rpc_wifi_drv interface. */
    erpc_status_t rpc_wifi_set_tx_pause_data_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#else
#include "rpc_wifi_api.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_rpc_wifi_drv_service(void);

void destroy_rpc_wifi_drv_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _rpc_wifi_api_server_h_
