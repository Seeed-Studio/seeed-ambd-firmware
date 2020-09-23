
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      wifi_api.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-09-18
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#include "elog.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "rpc_system_header.h"

//! @name rpc_wifi_drv
//@{
int32_t rpc_wifi_connect(const binary_t *ssid, const binary_t *password, uint32_t security_type, int32_t key_id, uint32_t semaphore)
{
    log_d("called");
    return wifi_connect(ssid->data, security_type, password->data, ssid->dataLength, password->dataLength, key_id, NULL);
}

int32_t rpc_wifi_connect_bssid(const binary_t * bssid, const binary_t * ssid, const binary_t * password, uint32_t security_type, int32_t key_id, uint32_t semaphore)
{
    log_d("called");
    return wifi_connect_bssid(bssid->data, ssid->data, security_type, password->data, bssid->dataLength, ssid->dataLength, password->dataLength, key_id, NULL);
    return 0;
}

int32_t rpc_wifi_disconnect(void)
{
    log_d("called");
    return wifi_disconnect();
}

int32_t rpc_wifi_is_connected_to_ap(void)
{
    log_d("called");
    return wifi_is_connected_to_ap();
}

int32_t rpc_wifi_is_up(uint32_t itf)
{
    log_d("called");
    return wifi_is_up(itf);
}

int32_t rpc_wifi_is_ready_to_transceive(uint32_t itf)
{
    log_d("called");
    return wifi_is_ready_to_transceive(itf);
}

int32_t rpc_wifi_set_mac_address(const binary_t *mac)
{
    log_d("called");
    return rpc_wifi_set_mac_address(mac->data);
}

int32_t rpc_wifi_get_mac_address(uint8_t mac[18])
{
    log_d("called");
    return wifi_get_mac_address(mac);
}

int32_t rpc_wifi_enable_powersave(void)
{
    log_d("called");
    return wifi_enable_powersave();
}

int32_t rpc_wifi_resume_powersave(void)
{
    log_d("called");
    return wifi_resume_powersave();
}

int32_t rpc_wifi_disable_powersave(void)
{
    log_d("called");
    return wifi_disable_powersave();
}

void rpc_wifi_btcoex_set_bt_on(void)
{
    log_d("called");
    wifi_btcoex_set_bt_on();
    return;
}

void rpc_wifi_btcoex_set_bt_off(void)
{
    log_d("called");
    wifi_btcoex_set_bt_off();
    return;
}

int32_t rpc_wifi_get_associated_client_list(binary_t *client_list_buffer, uint16_t buffer_length)
{
    log_d("called");
    int32_t ret = 0;
    uint8_t *data = (uint8_t *)erpc_malloc(buffer_length * sizeof(uint8_t));
    ret = wifi_get_associated_client_list(data, buffer_length);
    client_list_buffer->dataLength = buffer_length;
    client_list_buffer->data = data;
    return ret;
}

int32_t rpc_wifi_get_ap_bssid(uint8_t bssid[6])
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_ap_bssid(bssid);
    return ret;
}

int32_t rpc_wifi_get_ap_info(binary_t *ap_info, uint32_t *security)
{
    log_d("called");
    int32_t ret = 0;
    rtw_bss_info_t *p_ap_info = (rtw_bss_info_t *)erpc_malloc(sizeof(rtw_bss_info_t));
    ret = wifi_get_ap_info(p_ap_info, security);
    ap_info->data = (uint8_t *)p_ap_info;
    ap_info->dataLength = sizeof(rtw_bss_info_t);
    return ret;
}

int32_t rpc_wifi_set_country(uint32_t country_code)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_country(country_code);
    return ret;
}
int32_t rpc_wifi_get_sta_max_data_rate(uint8_t *inidata_rate)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_sta_max_data_rate(inidata_rate);
    return ret;
}
int32_t rpc_wifi_get_rssi(int32_t *pRSSI)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_rssi(pRSSI);
    return ret;
}

int32_t rpc_wifi_set_channel(int32_t channel)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_channel(channel);
    return ret;
}

int32_t rpc_wifi_get_channel(int32_t *channel)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_channel(channel);
    return ret;
}

int32_t rpc_wifi_change_channel_plan(uint8_t channel_plan)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_change_channel_plan(channel_plan);
    return ret;
}

int32_t rpc_wifi_register_multicast_address(const uint8_t mac[6])
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_register_multicast_address(mac);
    return ret;
}

int32_t rpc_wifi_unregister_multicast_address(const uint8_t mac[6])
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_unregister_multicast_address(mac);
    return ret;
}

int32_t rpc_wifi_rf_on(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_rf_on();
    return ret;
}

int32_t rpc_wifi_rf_off(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_rf_off();
    return ret;
}

int32_t rpc_wifi_on(uint32_t mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_on(mode);
    return ret;
}

int32_t rpc_wifi_off(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_off();
    return ret;
}

int32_t rpc_wifi_set_mode(uint32_t mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_mode(mode);
    return ret;
}

int32_t rpc_wifi_off_fastly(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_off_fastly();
    return ret;
}

int32_t rpc_wifi_set_power_mode(uint8_t ips_mode, uint8_t lps_mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_power_mode(ips_mode, lps_mode);
    return ret;
}

int32_t rpc_wifi_set_tdma_param(uint8_t slot_period, uint8_t rfon_period_len_1, uint8_t rfon_period_len_2, uint8_t rfon_period_len_3)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_tdma_param(slot_period, rfon_period_len_1, rfon_period_len_2, rfon_period_len_3);
    return ret;
}

int32_t rpc_wifi_set_lps_dtim(uint8_t dtim)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_lps_dtim(dtim);
    return ret;
}

int32_t rpc_wifi_get_lps_dtim(uint8_t *dtim)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_lps_dtim(dtim);
    return ret;
}

int32_t rpc_wifi_set_lps_thresh(uint8_t mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_lps_thresh(mode);
    return ret;
}

int32_t rpc_wifi_set_lps_level(uint8_t lps_level)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_lps_level(lps_level);
    return ret;
}

int32_t rpc_wifi_set_mfp_support(uint8_t value)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_mfp_support(value);
    return ret;
}

int32_t rpc_wifi_start_ap(const binary_t *ssid, const binary_t *password, uint32_t security_type, int32_t channel)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_start_ap(ssid->data, security_type, password->data, ssid->dataLength, password->dataLength, channel);
    return ret;
}

int32_t rpc_wifi_start_ap_with_hidden_ssid(const binary_t *ssid, const binary_t *password, uint32_t security_type, int32_t channel)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_start_ap_with_hidden_ssid(ssid->data, security_type, password->data, ssid->dataLength, password->dataLength, channel);
    return ret;
}

int32_t rpc_wifi_set_pscan_chan(const binary_t *channel_list, uint8_t pscan_config)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_pscan_chan(channel_list->data, pscan_config, channel_list->dataLength);
    return ret;
}

int32_t rpc_wifi_get_setting(const char *ifname, binary_t *pSetting)
{
    log_d("called");
    int32_t ret = 0;
    rtw_wifi_setting_t *p_pSetting = (rtw_wifi_setting_t *)erpc_malloc(sizeof(rtw_wifi_setting_t));
    ret = wifi_get_setting(ifname, p_pSetting);
    pSetting->data = (uint8_t *)p_pSetting;
    pSetting->dataLength = sizeof(rtw_wifi_setting_t);
    return ret;
}

int32_t rpc_wifi_set_network_mode(uint32_t mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_network_mode(mode);
    return ret;
}

int32_t rpc_wifi_get_network_mode(uint32_t *pmode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_network_mode(pmode);
    return ret;
}

int32_t rpc_wifi_set_wps_phase(uint8_t is_trigger_wps)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_wps_phase(is_trigger_wps);
    return ret;
}

int32_t rpc_wifi_restart_ap(const binary_t *ssid, const binary_t *password, uint32_t security_type, int32_t channel)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_restart_ap(ssid->data, security_type, password->data, ssid->dataLength, password->dataLength, channel);
    return ret;
}

int32_t rpc_wifi_config_autoreconnect(uint8_t mode, uint8_t retry_times, uint16_t timeout)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_config_autoreconnect(mode, retry_times, timeout);
    return ret;
}

int32_t rpc_wifi_set_autoreconnect(uint8_t mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_autoreconnect(mode);
    return ret;
}

int32_t rpc_wifi_get_autoreconnect(uint8_t *mode)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_autoreconnect(mode);
    return ret;
}

int32_t rpc_wifi_get_last_error(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_last_error();
    return ret;
}

int32_t rpc_wifi_add_custom_ie(const binary_t *cus_ie)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_add_custom_ie(cus_ie->data, cus_ie->dataLength / sizeof(rtw_custom_ie_t));
    return ret;
}

int32_t rpc_wifi_update_custom_ie(const binary_t *cus_ie, int32_t ie_index)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_update_custom_ie(cus_ie->data, ie_index);
    return ret;
}

int32_t rpc_wifi_del_custom_ie(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_del_custom_ie();
    return ret;
}

void rpc_wifi_set_indicate_mgnt(int32_t enable)
{
    log_d("called");
    wifi_set_indicate_mgnt(enable);
    return;
}

int32_t rpc_wifi_get_drv_ability(uint32_t *ability)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_drv_ability(ability);
    return ret;
}

int32_t rpc_wifi_set_channel_plan(uint8_t channel_plan)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_channel_plan(channel_plan);
    return ret;
}

int32_t rpc_wifi_get_channel_plan(uint8_t *channel_plan)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_get_channel_plan(channel_plan);
    return ret;
}

int32_t rpc_wifi_enable_forwarding(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_enable_forwarding();
    return ret;
}

int32_t rpc_wifi_disable_forwarding(void)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_disable_forwarding();
    return ret;
}

int32_t rpc_wifi_set_ch_deauth(uint8_t enable)
{
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_ch_deauth(enable);
    return ret;
}

uint8_t rpc_wifi_get_band_type(void)
{
    log_d("called");
    uint8_t ret = 0;
    ret = wifi_get_band_type();
    return ret;
}

int32_t rpc_wifi_set_tx_pause_data(int32_t NewState)
{
    
    log_d("called");
    int32_t ret = 0;
    ret = wifi_set_tx_pause_data(NewState);
    return ret;
}
//@}