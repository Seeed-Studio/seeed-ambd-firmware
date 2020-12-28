
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      wifi_callback.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-9-24
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "flash_api.h"
#include "device_lock.h"
#include "wifi_ind.h"
#include "wifi_callback.h"
#include "elog.h"
#include "rpc_wifi_callback.h"

extern rtw_mode_t wifi_mode;

// Free space allocated inside struct binary_t function implementation
static void free_binary_t_struct(binary_t *data)
{
    if (data->data)
    {
        erpc_free(data->data);
    }
}

static void print_callback_data(uint8_t *data, uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02X, ", data[i]);
    }
    printf("\n\r");
}

static void callback_wifi_event_connect(char *buf, int buf_len, int flags, void *userdata)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    if (wifi_mode == RTW_MODE_STA)
    {
        wifi_callback_ind(SYSTEM_EVENT_STA_CONNECTED, buf, buf_len);
    }
}
static void callback_wifi_event_disconnect(char *buf, int buf_len, int flags, void *userdata)
{

    log_v("FUN:%s \n\r", __FUNCTION__);
    if (wifi_mode == RTW_MODE_STA)
    {
        wifi_callback_ind(SYSTEM_EVENT_STA_DISCONNECTED, buf, buf_len);
    }
}
static void callback_wifi_event_scan_done(char *buf, int buf_len, int flags, void *userdata)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
}
static void callback_wifi_sta_assoc(char *buf, int buf_len, int flags, void *userdata)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    wifi_callback_ind(SYSTEM_EVENT_AP_STACONNECTED, buf, buf_len);
}
static void callback_wifi_sta_disassoc(char *buf, int buf_len, int flags, void *userdata)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    wifi_callback_ind(SYSTEM_EVENT_AP_STADISCONNECTED, buf, buf_len);
}

static void callback_wifi_ip_changed(char *buf, int buf_len, int flags, void *userdata)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    wifi_callback_ind(SYSTEM_EVENT_STA_LOST_IP, buf, buf_len);
}

typedef int (*wlan_init_done_ptr)(void);
typedef int (*write_reconnect_ptr)(uint8_t *data, uint32_t len);
extern wlan_init_done_ptr p_wlan_init_done_callback;
extern write_reconnect_ptr p_write_reconnect_ptr;

int wifi_init_done_callback()
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    wifi_callback_ind(SYSTEM_EVENT_WIFI_READY, NULL, 0);
    return 1;
}

int wifi_clear_reconnect_data()
{
    flash_t flash;
    struct wlan_fast_reconnect wifi_info = {0};
    uint8_t * tmp = (uint8_t *)&wifi_info;
    uint8_t count;
    bool need_erase = false;

    log_v("FUN:%s \n\r", __FUNCTION__);

    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_stream_read(&flash, FAST_RECONNECT_DATA, sizeof(struct wlan_fast_reconnect), (u8 *)&wifi_info);
    for(count = 0 ; count < sizeof(wifi_info) ; count++){
        if(tmp[count] != 0xFF){
            need_erase = true;
            break;
        }
    }
    if(need_erase == true){
        flash_erase_sector(&flash, FAST_RECONNECT_DATA);
    }    
    device_mutex_unlock(RT_DEV_LOCK_FLASH);

    if(need_erase == true){
        log_v("wifi_clear_reconnect_data done");
    }

    return 1;
}

int wifi_get_reconnect_data(wlan_fast_reconnect_profile_t *wifi_info)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    flash_t flash;

    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_stream_read(&flash, FAST_RECONNECT_DATA, sizeof(struct wlan_fast_reconnect), (uint8_t *)wifi_info);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);

    /* Check whether stored flash profile is empty */
    struct wlan_fast_reconnect *empty_data;
    empty_data = (struct wlan_fast_reconnect *)malloc(sizeof(struct wlan_fast_reconnect));
    if (empty_data)
    {
        memset(empty_data, 0xff, sizeof(struct wlan_fast_reconnect));
        if (memcmp(empty_data, wifi_info, sizeof(struct wlan_fast_reconnect)) == 0)
        {
            printf("[FAST_CONNECT] Fast connect profile is empty, abort fast connection\n");
            free(empty_data);
            return 0;
        }
        free(empty_data);
    }

    return 1;
}

int wifi_write_reconnect_data_to_flash(uint8_t *data, uint32_t len)
{
    log_v("FUN:%s \n\r", __FUNCTION__);
    flash_t flash;
    struct wlan_fast_reconnect read_data = {0};
    if (!data)
        return -1;

    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_stream_read(&flash, FAST_RECONNECT_DATA, sizeof(struct wlan_fast_reconnect), (u8 *)&read_data);

    //wirte it to flash if different content: SSID, Passphrase, Channel, Security type
    if (memcmp(data, (u8 *)&read_data, sizeof(struct wlan_fast_reconnect)) != 0)
    {
        flash_erase_sector(&flash, FAST_RECONNECT_DATA);
        flash_stream_write(&flash, FAST_RECONNECT_DATA, len, (uint8_t *)data);
    }
    device_mutex_unlock(RT_DEV_LOCK_FLASH);

    wifi_callback_ind(SYSTEM_EVENT_STA_GOT_IP, data, len);

    return 0;
}

void wifi_callback_ind(system_event_id_t event, uint8_t *data, uint32_t len)
{
    log_v("[WiFi-event] event: %d\n\r", event);
    system_event_t event_data;
    memset(&(event_data.event_info), 0, sizeof(system_event_info_t));
    event_data.event_id = event;
    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        log_v("WiFi interface ready\n\r");
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        log_v("Completed scan for access points\n\r");
        break;
    case SYSTEM_EVENT_STA_START:
        log_v("WiFi client started\n\r");
        break;
    case SYSTEM_EVENT_STA_STOP:
        log_v("WiFi clients stopped\n\r");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
    {
        log_v("Connected to access point\n\r");
        rtw_bss_info_t ap_info;
        rtw_security_t sec;
        if (wifi_get_ap_info(&ap_info, &sec) == RTW_SUCCESS)
        {
            system_event_sta_connected_t *sta_connected = &(event_data.event_info.connected);
            strcpy(sta_connected->ssid, ap_info.SSID);
            sta_connected->ssid_len = ap_info.SSID_len;
            memcpy(sta_connected->bssid, ap_info.BSSID.octet, 6);
            sta_connected->channel = ap_info.channel;
            sta_connected->authmode = sec;
        }
        break;
    }
    case SYSTEM_EVENT_STA_DISCONNECTED:
        log_v("Disconnected from WiFi access point\n\r");
        rtw_bss_info_t ap_info;
        rtw_security_t sec;
        if (wifi_get_ap_info(&ap_info, &sec) == RTW_SUCCESS)
        {
            system_event_sta_disconnected_t *sta_disconnected = &(event_data.event_info.disconnected);
            strcpy(sta_disconnected->ssid, ap_info.SSID);
            sta_disconnected->ssid_len = ap_info.SSID_len;
            memcpy(sta_disconnected->bssid, ap_info.BSSID.octet, 6);

            int error = wifi_get_last_error();
            switch (error)
            {
            case RTW_NO_ERROR:
                log_v("\n\r\tNo Error");
                break;
            case RTW_NONE_NETWORK:
                log_v("\n\r\tTarget AP Not Found");
                sta_disconnected->reason = WIFI_REASON_NO_AP_FOUND;
                break;
            case RTW_CONNECT_FAIL:
                log_v("\n\r\tAssociation Failed");
                sta_disconnected->reason = WIFI_REASON_CONNECTION_FAIL;
                break;
            case RTW_WRONG_PASSWORD:
                log_v("\n\r\tWrong Password");
                sta_disconnected->reason = WIFI_REASON_AUTH_FAIL;
                break;
            case RTW_DHCP_FAIL:
                log_v("\n\r\tDHCP Failed");
                sta_disconnected->reason = WIFI_REASON_CONNECTION_FAIL;
                break;
            case RTW_4WAY_HANDSHAKE_TIMEOUT:
                log_v("\n\r\Time out");
                sta_disconnected->reason = WIFI_REASON_HANDSHAKE_TIMEOUT;
                break;
            default:
                log_v("\n\r\tUnknown Error(%d)", error);
            }
        }
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        log_v("Authentication mode of access point has changed\n\r");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        log_v("Obtained IP address: \n\r");
        system_event_sta_got_ip_t *sta_got_ip = &(event_data.event_info.got_ip);
        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &(sta_got_ip->ip_info));
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        log_v("Lost IP address and IP address is reset to 0\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        log_v("WiFi Protected Setup (WPS): succeeded in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        log_v("WiFi Protected Setup (WPS): failed in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        log_v("WiFi Protected Setup (WPS): timeout in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        log_v("WiFi Protected Setup (WPS): pin code in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_AP_START:
        log_v("WiFi access point started\n\r");
        break;
    case SYSTEM_EVENT_AP_STOP:
        log_v("WiFi access point  stopped\n\r");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        log_v("Client connected\n\r");
        system_event_ap_staconnected_t *ap_staconnected_t = &(event_data.event_info.sta_connected);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        log_v("Client disconnected\n\r");
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        log_v("Assigned IP address to client\n\r");
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        log_v("Received probe request\n\r");
        break;
    case SYSTEM_EVENT_GOT_IP6:
        log_v("IPv6 is preferred\n\r");
        break;
    case SYSTEM_EVENT_ETH_START:
        log_v("Ethernet started\n\r");
        break;
    case SYSTEM_EVENT_ETH_STOP:
        log_v("Ethernet stopped\n\r");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        log_v("Ethernet connected\n\r");
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        log_v("Ethernet disconnected\n\r");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        log_v("Obtained IP address\n\r");
        break;
    default:
        break;
    }
    //print_callback_data(data, len);
    binary_t cb_data;
    cb_data.dataLength = sizeof(system_event_t);
    cb_data.data = &event_data;
    rpc_wifi_event_callback(&cb_data);
}
void wifi_event_reg_init()
{
    wifi_reg_event_handler(WIFI_EVENT_CONNECT, callback_wifi_event_connect, NULL);
    wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, callback_wifi_event_disconnect, NULL);
    wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, callback_wifi_event_scan_done, NULL);
    wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, callback_wifi_sta_assoc, NULL);
    wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, callback_wifi_sta_disassoc, NULL);
}

void wifi_callback_init()
{
    p_wlan_init_done_callback = wifi_init_done_callback;
    p_write_reconnect_ptr = wifi_write_reconnect_data_to_flash;
}

void wifi_dns_found(const char *name, ip_addr_t *ipaddr, void *callback_arg)
{
    log_d("called");
    binary_t b_ipaddr;
    b_ipaddr.data = (uint8_t *)ipaddr;
    b_ipaddr.dataLength = sizeof(ipaddr);
    binary_t *b_callback_arg = (binary_t *)callback_arg;

    rpc_wifi_dns_found(name, &b_ipaddr, b_callback_arg);

    if (callback_arg)
    {
        free_binary_t_struct(callback_arg);
    }
    if (callback_arg)
    {
        erpc_free(callback_arg);
    }
}
