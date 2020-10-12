
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
#include "wifi_ind.h"
#include "wifi_callback.h"
#include "elog.h"

static void callback_wifi_event_connect(char *buf, int buf_len, int flags, void *userdata)
{
    wifi_callback_ind(SYSTEM_EVENT_STA_CONNECTED);
}
static void callback_wifi_event_disconnect(char *buf, int buf_len, int flags, void *userdata)
{
    wifi_callback_ind(SYSTEM_EVENT_STA_DISCONNECTED);
}
static void callback_wifi_event_scan_done(char *buf, int buf_len, int flags, void *userdata)
{
    wifi_callback_ind(SYSTEM_EVENT_SCAN_DONE);
}
static void callback_wifi_sta_assoc(char *buf, int buf_len, int flags, void *userdata)
{
    wifi_callback_ind(SYSTEM_EVENT_AP_STACONNECTED);
}
static void callback_wifi_sta_disassoc(char *buf, int buf_len, int flags, void *userdata)
{
    wifi_callback_ind(SYSTEM_EVENT_AP_STADISCONNECTED);
}

typedef int (*wlan_init_done_ptr)(void);
typedef int (*write_reconnect_ptr)(uint8_t *data, uint32_t len);
wlan_init_done_ptr p_wlan_init_done_callback;
write_reconnect_ptr p_write_reconnect_ptr;
int wifi_init_done_callback()
{
    wifi_callback_ind(SYSTEM_EVENT_WIFI_READY);
    return 1;
}

int wifi_write_reconnect_data_to_flash(uint8_t *data, uint32_t len)
{
    wifi_callback_ind(SYSTEM_EVENT_STA_GOT_IP);
    return 1;
}
void wifi_callback_ind(system_event_id_t event)
{
    printf("[WiFi-event] event: %d\n\r", event);

    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        printf("WiFi interface ready\n\r");
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        printf("Completed scan for access points\n\r");
        break;
    case SYSTEM_EVENT_STA_START:
        printf("WiFi client started\n\r");
        break;
    case SYSTEM_EVENT_STA_STOP:
        printf("WiFi clients stopped\n\r");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        printf("Connected to access point\n\r");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        printf("Disconnected from WiFi access point\n\r");
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        printf("Authentication mode of access point has changed\n\r");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        printf("Obtained IP address: \n\r");
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        printf("Lost IP address and IP address is reset to 0\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        printf("WiFi Protected Setup (WPS): succeeded in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        printf("WiFi Protected Setup (WPS): failed in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        printf("WiFi Protected Setup (WPS): timeout in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        printf("WiFi Protected Setup (WPS): pin code in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_AP_START:
        printf("WiFi access point started\n\r");
        break;
    case SYSTEM_EVENT_AP_STOP:
        printf("WiFi access point  stopped\n\r");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        printf("Client connected\n\r");
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        printf("Client disconnected\n\r");
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        printf("Assigned IP address to client\n\r");
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        printf("Received probe request\n\r");
        break;
    case SYSTEM_EVENT_GOT_IP6:
        printf("IPv6 is preferred\n\r");
        break;
    case SYSTEM_EVENT_ETH_START:
        printf("Ethernet started\n\r");
        break;
    case SYSTEM_EVENT_ETH_STOP:
        printf("Ethernet stopped\n\r");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        printf("Ethernet connected\n\r");
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        printf("Ethernet disconnected\n\r");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        printf("Obtained IP address\n\r");
        break;
    default:
        break;
    }
}
void wifi_callback_init()
{
    wifi_reg_event_handler(WIFI_EVENT_CONNECT, callback_wifi_event_connect, NULL);
    wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, callback_wifi_event_disconnect, NULL);
    wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, callback_wifi_event_scan_done, NULL);
    wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, callback_wifi_sta_assoc, NULL);
    wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, callback_wifi_sta_disassoc, NULL);

    p_wlan_init_done_callback = wifi_init_done_callback;
    p_write_reconnect_ptr = wifi_write_reconnect_data_to_flash;
}
