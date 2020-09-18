
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
int32_t rpc_wifi_connect(const binary_t *ssid, const binary_t *password, uint32_t ecurity_type, int32_t key_id, uint32_t semaphore)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_connect_bssid(const uint8_t bssid[6], const binary_t *ssid, const binary_t *password, uint32_t ecurity_type, int32_t key_id, uint32_t semaphore)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_disconnect(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_is_connected_to_ap(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_is_up(uint32_t itf)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_is_ready_to_transceive(uint32_t itf)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_mac_address(const binary_t *mac)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_mac_address(binary_t *mac)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_enable_powersave(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_resume_powersave(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_disable_powersave(void)
{
    log_d("called");
    return 0;
}

void rpc_wifi_btcoex_set_bt_on(void)
{
    log_d("called");
    return;
}

void rpc_wifi_btcoex_set_bt_off(void)
{
    log_d("called");
    return;
}

int32_t rpc_wifi_get_txpower(int32_t *poweridx)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_txpower(int32_t poweridx)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_associated_client_list(binary_t *client_list_buffer, uint16_t buffer_length)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_ap_bssid(binary_t *bssid)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_ap_info(binary_t *ap_info, uint32_t *security)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_country(uint32_t country_code)
{
    log_d("called");
    return 0;
}
int32_t rpc_wifi_get_sta_max_data_rate(uint8_t *inidata_rate)
{
    log_d("called");
    return 0;
}
int32_t rpc_wifi_get_rssi(int32_t *pRSSI)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_channel(int32_t channel)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_channel(int32_t *channel)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_change_channel_plan(uint8_t channel_plan)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_register_multicast_address(const uint8_t mac[6])
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_unregister_multicast_address(const uint8_t mac[6])
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_rf_on(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_rf_off(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_on(uint32_t mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_off(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_mode(uint32_t mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_off_fastly(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_power_mode(uint8_t ips_mode, uint8_t lps_mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_tdma_param(uint8_t slot_period, uint8_t rfon_period_len_1, uint8_t rfon_period_len_2, uint8_t rfon_period_len_3)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_lps_dtim(uint8_t dtim)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_lps_dtim(uint8_t *dtim)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_lps_thresh(uint8_t mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_lps_level(uint8_t lps_level)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_mfp_support(uint8_t value)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_start_ap(const binary_t *ssid, const binary_t *password, uint32_t ecurity_type, int32_t channel)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_start_ap_with_hidden_ssid(const binary_t *ssid, const binary_t *password, uint32_t ecurity_type, int32_t channel)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_pscan_chan(const binary_t *channel_list, uint8_t pscan_config)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_setting(const char *ifname, binary_t *pSetting)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_network_mode(uint32_t mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_network_mode(uint32_t *pmode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_wps_phase(uint8_t is_trigger_wps)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_restart_ap(const binary_t *ssid, const binary_t *password, uint32_t ecurity_type, int32_t channel)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_config_autoreconnect(uint8_t mode, uint8_t retry_times, uint16_t timeout)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_autoreconnect(uint8_t mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_autoreconnect(uint8_t *mode)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_last_error(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_add_custom_ie(const binary_t *cus_ie)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_update_custom_ie(const binary_t *cus_ie, int32_t ie_index)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_del_custom_ie(void)
{
    log_d("called");
    return 0;
}

void rpc_wifi_set_indicate_mgnt(int32_t enable)
{
    log_d("called");
    return;
}

int32_t rpc_wifi_get_drv_ability(uint32_t *ability)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_channel_plan(uint8_t channel_plan)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_get_channel_plan(uint8_t *channel_plan)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_enable_forwarding(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_disable_forwarding(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_ch_deauth(uint8_t enable)
{
    log_d("called");
    return 0;
}

void rpc_wifi_set_ap_polling_sta(uint8_t enabled)
{
    log_d("called");
    return;
}

uint8_t rpc_wifi_get_band_type(void)
{
    log_d("called");
    return 0;
}

int32_t rpc_wifi_set_tx_pause_data(int32_t NewState)
{
    log_d("called");
    return 0;
}
//@}