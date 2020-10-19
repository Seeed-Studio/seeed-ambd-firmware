/*
 * Generated by erpcgen 1.7.4 on Mon Oct 19 14:52:05 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_wifi_api_h_)
#define _rpc_wifi_api_h_

// Common types header file
#include "rpc_system_header.h"

/*! @brief rpc_wifi_drv identifiers */
enum _rpc_wifi_drv_ids
{
    krpc_wifi_drv_service_id = 13,
    krpc_wifi_drv_rpc_wifi_connect_id = 1,
    krpc_wifi_drv_rpc_wifi_connect_bssid_id = 2,
    krpc_wifi_drv_rpc_wifi_disconnect_id = 3,
    krpc_wifi_drv_rpc_wifi_is_connected_to_ap_id = 4,
    krpc_wifi_drv_rpc_wifi_is_up_id = 5,
    krpc_wifi_drv_rpc_wifi_is_ready_to_transceive_id = 6,
    krpc_wifi_drv_rpc_wifi_set_mac_address_id = 7,
    krpc_wifi_drv_rpc_wifi_get_mac_address_id = 8,
    krpc_wifi_drv_rpc_wifi_enable_powersave_id = 9,
    krpc_wifi_drv_rpc_wifi_resume_powersave_id = 10,
    krpc_wifi_drv_rpc_wifi_disable_powersave_id = 11,
    krpc_wifi_drv_rpc_wifi_btcoex_set_bt_on_id = 12,
    krpc_wifi_drv_rpc_wifi_btcoex_set_bt_off_id = 13,
    krpc_wifi_drv_rpc_wifi_get_associated_client_list_id = 14,
    krpc_wifi_drv_rpc_wifi_get_ap_bssid_id = 15,
    krpc_wifi_drv_rpc_wifi_get_ap_info_id = 16,
    krpc_wifi_drv_rpc_wifi_set_country_id = 17,
    krpc_wifi_drv_rpc_wifi_get_sta_max_data_rate_id = 18,
    krpc_wifi_drv_rpc_wifi_get_rssi_id = 19,
    krpc_wifi_drv_rpc_wifi_set_channel_id = 20,
    krpc_wifi_drv_rpc_wifi_get_channel_id = 21,
    krpc_wifi_drv_rpc_wifi_change_channel_plan_id = 22,
    krpc_wifi_drv_rpc_wifi_register_multicast_address_id = 23,
    krpc_wifi_drv_rpc_wifi_unregister_multicast_address_id = 24,
    krpc_wifi_drv_rpc_wifi_rf_on_id = 25,
    krpc_wifi_drv_rpc_wifi_rf_off_id = 26,
    krpc_wifi_drv_rpc_wifi_on_id = 27,
    krpc_wifi_drv_rpc_wifi_off_id = 28,
    krpc_wifi_drv_rpc_wifi_set_mode_id = 29,
    krpc_wifi_drv_rpc_wifi_off_fastly_id = 30,
    krpc_wifi_drv_rpc_wifi_set_power_mode_id = 31,
    krpc_wifi_drv_rpc_wifi_set_tdma_param_id = 32,
    krpc_wifi_drv_rpc_wifi_set_lps_dtim_id = 33,
    krpc_wifi_drv_rpc_wifi_get_lps_dtim_id = 34,
    krpc_wifi_drv_rpc_wifi_set_lps_thresh_id = 35,
    krpc_wifi_drv_rpc_wifi_set_lps_level_id = 36,
    krpc_wifi_drv_rpc_wifi_set_mfp_support_id = 37,
    krpc_wifi_drv_rpc_wifi_start_ap_id = 38,
    krpc_wifi_drv_rpc_wifi_start_ap_with_hidden_ssid_id = 39,
    krpc_wifi_drv_rpc_wifi_set_pscan_chan_id = 40,
    krpc_wifi_drv_rpc_wifi_get_setting_id = 41,
    krpc_wifi_drv_rpc_wifi_set_network_mode_id = 42,
    krpc_wifi_drv_rpc_wifi_get_network_mode_id = 43,
    krpc_wifi_drv_rpc_wifi_set_wps_phase_id = 44,
    krpc_wifi_drv_rpc_wifi_restart_ap_id = 45,
    krpc_wifi_drv_rpc_wifi_config_autoreconnect_id = 46,
    krpc_wifi_drv_rpc_wifi_set_autoreconnect_id = 47,
    krpc_wifi_drv_rpc_wifi_get_autoreconnect_id = 48,
    krpc_wifi_drv_rpc_wifi_get_last_error_id = 49,
    krpc_wifi_drv_rpc_wifi_add_custom_ie_id = 50,
    krpc_wifi_drv_rpc_wifi_update_custom_ie_id = 51,
    krpc_wifi_drv_rpc_wifi_del_custom_ie_id = 52,
    krpc_wifi_drv_rpc_wifi_set_indicate_mgnt_id = 53,
    krpc_wifi_drv_rpc_wifi_get_drv_ability_id = 54,
    krpc_wifi_drv_rpc_wifi_set_channel_plan_id = 55,
    krpc_wifi_drv_rpc_wifi_get_channel_plan_id = 56,
    krpc_wifi_drv_rpc_wifi_enable_forwarding_id = 57,
    krpc_wifi_drv_rpc_wifi_disable_forwarding_id = 58,
    krpc_wifi_drv_rpc_wifi_set_ch_deauth_id = 59,
    krpc_wifi_drv_rpc_wifi_get_band_type_id = 60,
    krpc_wifi_drv_rpc_wifi_set_tx_pause_data_id = 61,
    krpc_wifi_drv_rpc_wifi_get_reconnect_data_id = 62,
    krpc_wifi_drv_rpc_wifi_scan_start_id = 63,
    krpc_wifi_drv_rpc_wifi_is_scaning_id = 64,
    krpc_wifi_drv_rpc_wifi_scan_get_ap_records_id = 65,
    krpc_wifi_drv_rpc_wifi_scan_get_ap_num_id = 66,
};

/*! @brief rpc_wifi_tcpip identifiers */
enum _rpc_wifi_tcpip_ids
{
    krpc_wifi_tcpip_service_id = 14,
    krpc_wifi_tcpip_rpc_tcpip_adapter_init_id = 1,
    krpc_wifi_tcpip_rpc_tcpip_adapter_sta_start_id = 2,
    krpc_wifi_tcpip_rpc_tcpip_adapter_ap_start_id = 3,
    krpc_wifi_tcpip_rpc_tcpip_adapter_stop_id = 4,
    krpc_wifi_tcpip_rpc_tcpip_adapter_up_id = 5,
    krpc_wifi_tcpip_rpc_tcpip_adapter_down_id = 6,
    krpc_wifi_tcpip_rpc_tcpip_adapter_get_ip_info_id = 7,
    krpc_wifi_tcpip_rpc_tcpip_adapter_set_ip_info_id = 8,
    krpc_wifi_tcpip_rpc_tcpip_adapter_set_dns_info_id = 9,
    krpc_wifi_tcpip_rpc_tcpip_adapter_get_dns_info_id = 10,
    krpc_wifi_tcpip_rpc_tcpip_adapter_dhcps_start_id = 11,
    krpc_wifi_tcpip_rpc_tcpip_adapter_dhcps_stop_id = 12,
    krpc_wifi_tcpip_rpc_tcpip_adapter_dhcpc_start_id = 13,
    krpc_wifi_tcpip_rpc_tcpip_adapter_dhcpc_stop_id = 14,
    krpc_wifi_tcpip_rpc_tcpip_adapter_set_hostname_id = 15,
    krpc_wifi_tcpip_rpc_tcpip_adapter_get_hostname_id = 16,
    krpc_wifi_tcpip_rpc_tcpip_adapter_get_mac_id = 17,
    krpc_wifi_tcpip_rpc_tcpip_adapter_set_mac_id = 18,
};

/*! @brief rpc_wifi_lwip identifiers */
enum _rpc_wifi_lwip_ids
{
    krpc_wifi_lwip_service_id = 15,
    krpc_wifi_lwip_rpc_lwip_accept_id = 1,
    krpc_wifi_lwip_rpc_lwip_bind_id = 2,
    krpc_wifi_lwip_rpc_lwip_shutdown_id = 3,
    krpc_wifi_lwip_rpc_lwip_getpeername_id = 4,
    krpc_wifi_lwip_rpc_lwip_getsockname_id = 5,
    krpc_wifi_lwip_rpc_lwip_getsockopt_id = 6,
    krpc_wifi_lwip_rpc_lwip_setsockopt_id = 7,
    krpc_wifi_lwip_rpc_lwip_close_id = 8,
    krpc_wifi_lwip_rpc_lwip_connect_id = 9,
    krpc_wifi_lwip_rpc_lwip_listen_id = 10,
    krpc_wifi_lwip_rpc_lwip_recv_id = 11,
    krpc_wifi_lwip_rpc_lwip_read_id = 12,
    krpc_wifi_lwip_rpc_lwip_recvfrom_id = 13,
    krpc_wifi_lwip_rpc_lwip_send_id = 14,
    krpc_wifi_lwip_rpc_lwip_sendmsg_id = 15,
    krpc_wifi_lwip_rpc_lwip_sendto_id = 16,
    krpc_wifi_lwip_rpc_lwip_socket_id = 17,
    krpc_wifi_lwip_rpc_lwip_write_id = 18,
    krpc_wifi_lwip_rpc_lwip_writev_id = 19,
    krpc_wifi_lwip_rpc_lwip_select_id = 20,
    krpc_wifi_lwip_rpc_lwip_ioctl_id = 21,
    krpc_wifi_lwip_rpc_lwip_fcntl_id = 22,
};

#if defined(__cplusplus)
extern "C" {
#endif

//! @name rpc_wifi_drv
//@{
int32_t rpc_wifi_connect(const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t key_id, uint32_t semaphore);

int32_t rpc_wifi_connect_bssid(const binary_t * bssid, const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t key_id, uint32_t semaphore);

int32_t rpc_wifi_disconnect(void);

int32_t rpc_wifi_is_connected_to_ap(void);

int32_t rpc_wifi_is_up(uint32_t itf);

int32_t rpc_wifi_is_ready_to_transceive(uint32_t itf);

int32_t rpc_wifi_set_mac_address(const binary_t * mac);

int32_t rpc_wifi_get_mac_address(uint8_t mac[18]);

int32_t rpc_wifi_enable_powersave(void);

int32_t rpc_wifi_resume_powersave(void);

int32_t rpc_wifi_disable_powersave(void);

void rpc_wifi_btcoex_set_bt_on(void);

void rpc_wifi_btcoex_set_bt_off(void);

int32_t rpc_wifi_get_associated_client_list(binary_t * client_list_buffer, uint16_t buffer_length);

int32_t rpc_wifi_get_ap_bssid(uint8_t bssid[6]);

int32_t rpc_wifi_get_ap_info(binary_t * ap_info, uint32_t * security);

int32_t rpc_wifi_set_country(uint32_t country_code);

int32_t rpc_wifi_get_sta_max_data_rate(uint8_t * inidata_rate);

int32_t rpc_wifi_get_rssi(int32_t * pRSSI);

int32_t rpc_wifi_set_channel(int32_t channel);

int32_t rpc_wifi_get_channel(int32_t * channel);

int32_t rpc_wifi_change_channel_plan(uint8_t channel_plan);

int32_t rpc_wifi_register_multicast_address(const uint8_t mac[6]);

int32_t rpc_wifi_unregister_multicast_address(const uint8_t mac[6]);

int32_t rpc_wifi_rf_on(void);

int32_t rpc_wifi_rf_off(void);

int32_t rpc_wifi_on(uint32_t mode);

int32_t rpc_wifi_off(void);

int32_t rpc_wifi_set_mode(uint32_t mode);

int32_t rpc_wifi_off_fastly(void);

int32_t rpc_wifi_set_power_mode(uint8_t ips_mode, uint8_t lps_mode);

int32_t rpc_wifi_set_tdma_param(uint8_t slot_period, uint8_t rfon_period_len_1, uint8_t rfon_period_len_2, uint8_t rfon_period_len_3);

int32_t rpc_wifi_set_lps_dtim(uint8_t dtim);

int32_t rpc_wifi_get_lps_dtim(uint8_t * dtim);

int32_t rpc_wifi_set_lps_thresh(uint8_t mode);

int32_t rpc_wifi_set_lps_level(uint8_t lps_level);

int32_t rpc_wifi_set_mfp_support(uint8_t value);

int32_t rpc_wifi_start_ap(const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t channel);

int32_t rpc_wifi_start_ap_with_hidden_ssid(const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t channel);

int32_t rpc_wifi_set_pscan_chan(const binary_t * channel_list, uint8_t pscan_config);

int32_t rpc_wifi_get_setting(const char * ifname, binary_t * pSetting);

int32_t rpc_wifi_set_network_mode(uint32_t mode);

int32_t rpc_wifi_get_network_mode(uint32_t * pmode);

int32_t rpc_wifi_set_wps_phase(uint8_t is_trigger_wps);

int32_t rpc_wifi_restart_ap(const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t channel);

int32_t rpc_wifi_config_autoreconnect(uint8_t mode, uint8_t retry_times, uint16_t timeout);

int32_t rpc_wifi_set_autoreconnect(uint8_t mode);

int32_t rpc_wifi_get_autoreconnect(uint8_t * mode);

int32_t rpc_wifi_get_last_error(void);

int32_t rpc_wifi_add_custom_ie(const binary_t * cus_ie);

int32_t rpc_wifi_update_custom_ie(const binary_t * cus_ie, int32_t ie_index);

int32_t rpc_wifi_del_custom_ie(void);

void rpc_wifi_set_indicate_mgnt(int32_t enable);

int32_t rpc_wifi_get_drv_ability(uint32_t * ability);

int32_t rpc_wifi_set_channel_plan(uint8_t channel_plan);

int32_t rpc_wifi_get_channel_plan(uint8_t * channel_plan);

int32_t rpc_wifi_enable_forwarding(void);

int32_t rpc_wifi_disable_forwarding(void);

int32_t rpc_wifi_set_ch_deauth(uint8_t enable);

uint8_t rpc_wifi_get_band_type(void);

int32_t rpc_wifi_set_tx_pause_data(uint32_t NewState);

int32_t rpc_wifi_get_reconnect_data(binary_t * wifi_info);

int32_t rpc_wifi_scan_start(void);

bool rpc_wifi_is_scaning(void);

int32_t rpc_wifi_scan_get_ap_records(uint16_t number, binary_t * _scanResult);

uint16_t rpc_wifi_scan_get_ap_num(void);
//@}

//! @name rpc_wifi_tcpip
//@{
int32_t rpc_tcpip_adapter_init(void);

int32_t rpc_tcpip_adapter_sta_start(const binary_t * mac, const binary_t * ip_info);

int32_t rpc_tcpip_adapter_ap_start(const binary_t * mac, const binary_t * ip_info);

int32_t rpc_tcpip_adapter_stop(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_up(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_down(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_get_ip_info(uint32_t tcpip_if, binary_t * ip_info);

int32_t rpc_tcpip_adapter_set_ip_info(uint32_t tcpip_if, const binary_t * ip_info);

int32_t rpc_tcpip_adapter_set_dns_info(uint32_t tcpip_if, uint32_t dns_type, const binary_t * dns);

int32_t rpc_tcpip_adapter_get_dns_info(uint32_t tcpip_if, uint32_t dns_type, binary_t * dns);

int32_t rpc_tcpip_adapter_dhcps_start(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_dhcps_stop(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_dhcpc_start(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_dhcpc_stop(uint32_t tcpip_if);

int32_t rpc_tcpip_adapter_set_hostname(uint32_t tcpip_if, const char * hostname);

int32_t rpc_tcpip_adapter_get_hostname(uint32_t tcpip_if, char * hostname);

int32_t rpc_tcpip_adapter_get_mac(uint32_t tcpip_if, binary_t * mac);

int32_t rpc_tcpip_adapter_set_mac(uint32_t tcpip_if, const binary_t * mac);
//@}

//! @name rpc_wifi_lwip
//@{
int32_t rpc_lwip_accept(int32_t s, const binary_t * addr, uint32_t * addrlen);

int32_t rpc_lwip_bind(int32_t s, const binary_t * name, uint32_t namelen);

int32_t rpc_lwip_shutdown(int32_t s, int32_t how);

int32_t rpc_lwip_getpeername(int32_t s, binary_t * name, uint32_t * namelen);

int32_t rpc_lwip_getsockname(int32_t s, binary_t * name, uint32_t * namelen);

int32_t rpc_lwip_getsockopt(int32_t s, int32_t level, int32_t optname, const binary_t * in_optval, binary_t * out_optval, uint32_t * optlen);

int32_t rpc_lwip_setsockopt(int32_t s, int32_t level, int32_t optname, const binary_t * optval, uint32_t optlen);

int32_t rpc_lwip_close(int32_t s);

int32_t rpc_lwip_connect(int32_t s, const binary_t * name, uint32_t namelen);

int32_t rpc_lwip_listen(int32_t s, int32_t backlog);

int32_t rpc_lwip_recv(int32_t s, binary_t * mem, uint32_t len, int32_t flags);

int32_t rpc_lwip_read(int32_t s, binary_t * mem, uint32_t len);

int32_t rpc_lwip_recvfrom(int32_t s, binary_t * mem, uint32_t len, int32_t flags, const binary_t * from, uint32_t * fromlen);

int32_t rpc_lwip_send(int32_t s, const binary_t * dataptr, int32_t flags);

int32_t rpc_lwip_sendmsg(int32_t s, const binary_t * msg_name, const binary_t * msg_iov, const binary_t * msg_control, int32_t msg_flags, int32_t flags);

int32_t rpc_lwip_sendto(int32_t s, const binary_t * dataptr, int32_t flags, const binary_t * to, uint32_t tolen);

int32_t rpc_lwip_socket(int32_t domain, int32_t l_type, int32_t protocol);

int32_t rpc_lwip_write(int32_t s, const binary_t * dataptr, uint32_t size);

int32_t rpc_lwip_writev(int32_t s, const binary_t * iov, int32_t iovcnt);

int32_t rpc_lwip_select(int32_t maxfdp1, const binary_t * readset, const binary_t * writeset, const binary_t * exceptset, const binary_t * timeout);

int32_t rpc_lwip_ioctl(int32_t s, uint32_t cmd, const binary_t * argp);

int32_t rpc_lwip_fcntl(int32_t s, int32_t cmd, int32_t val);
//@}

#if defined(__cplusplus)
}
#endif

#endif // _rpc_wifi_api_h_
