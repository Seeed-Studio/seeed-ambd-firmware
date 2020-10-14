/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      wifi_callback.h
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-9-24
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */
#ifndef _PRC_WIFI_CALLBACK_H_
#define _PRC_WIFI_CALLBACK_H_
#include "wifi_tcpip_adapter.h"

typedef enum {
    WIFI_REASON_UNSPECIFIED              = 1,
    WIFI_REASON_AUTH_EXPIRE              = 2,
    WIFI_REASON_AUTH_LEAVE               = 3,
    WIFI_REASON_ASSOC_EXPIRE             = 4,
    WIFI_REASON_ASSOC_TOOMANY            = 5,
    WIFI_REASON_NOT_AUTHED               = 6,
    WIFI_REASON_NOT_ASSOCED              = 7,
    WIFI_REASON_ASSOC_LEAVE              = 8,
    WIFI_REASON_ASSOC_NOT_AUTHED         = 9,
    WIFI_REASON_DISASSOC_PWRCAP_BAD      = 10,
    WIFI_REASON_DISASSOC_SUPCHAN_BAD     = 11,
    WIFI_REASON_IE_INVALID               = 13,
    WIFI_REASON_MIC_FAILURE              = 14,
    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT   = 15,
    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    WIFI_REASON_IE_IN_4WAY_DIFFERS       = 17,
    WIFI_REASON_GROUP_CIPHER_INVALID     = 18,
    WIFI_REASON_PAIRWISE_CIPHER_INVALID  = 19,
    WIFI_REASON_AKMP_INVALID             = 20,
    WIFI_REASON_UNSUPP_RSN_IE_VERSION    = 21,
    WIFI_REASON_INVALID_RSN_IE_CAP       = 22,
    WIFI_REASON_802_1X_AUTH_FAILED       = 23,
    WIFI_REASON_CIPHER_SUITE_REJECTED    = 24,

    WIFI_REASON_BEACON_TIMEOUT           = 200,
    WIFI_REASON_NO_AP_FOUND              = 201,
    WIFI_REASON_AUTH_FAIL                = 202,
    WIFI_REASON_ASSOC_FAIL               = 203,
    WIFI_REASON_HANDSHAKE_TIMEOUT        = 204,
    WIFI_REASON_CONNECTION_FAIL          = 205,
} wifi_err_reason_t;

typedef enum {
    SYSTEM_EVENT_WIFI_READY = 0,           /**< WiFi ready */
    SYSTEM_EVENT_SCAN_DONE,                /**< finish scanning AP */
    SYSTEM_EVENT_STA_START,                /**< station start */
    SYSTEM_EVENT_STA_STOP,                 /**< station stop */
    SYSTEM_EVENT_STA_CONNECTED,            /**< station connected to AP */
    SYSTEM_EVENT_STA_DISCONNECTED,         /**< station disconnected from AP */
    SYSTEM_EVENT_STA_AUTHMODE_CHANGE,      /**< the auth mode of AP connected by station changed */
    SYSTEM_EVENT_STA_GOT_IP,               /**< station got IP from connected AP */
    SYSTEM_EVENT_STA_LOST_IP,              /**< station lost IP and the IP is reset to 0 */
    SYSTEM_EVENT_STA_WPS_ER_SUCCESS,       /**< station wps succeeds in enrollee mode */
    SYSTEM_EVENT_STA_WPS_ER_FAILED,        /**< station wps fails in enrollee mode */
    SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,       /**< station wps timeout in enrollee mode */
    SYSTEM_EVENT_STA_WPS_ER_PIN,           /**< station wps pin code in enrollee mode */
    SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP,   /*!< station wps overlap in enrollee mode */
    SYSTEM_EVENT_AP_START,                 /**< soft-AP start */
    SYSTEM_EVENT_AP_STOP,                  /**< soft-AP stop */
    SYSTEM_EVENT_AP_STACONNECTED,          /**< a station connected to soft-AP */
    SYSTEM_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from soft-AP */
    SYSTEM_EVENT_AP_STAIPASSIGNED,         /**< soft-AP assign an IP to a connected station */
    SYSTEM_EVENT_AP_PROBEREQRECVED,        /**< Receive probe request packet in soft-AP interface */
    SYSTEM_EVENT_GOT_IP6,                  /**< station or ap or ethernet interface v6IP addr is preferred */
    SYSTEM_EVENT_ETH_START,                /**< ethernet start */
    SYSTEM_EVENT_ETH_STOP,                 /**< ethernet stop */
    SYSTEM_EVENT_ETH_CONNECTED,            /**< ethernet phy link up */
    SYSTEM_EVENT_ETH_DISCONNECTED,         /**< ethernet phy link down */
    SYSTEM_EVENT_ETH_GOT_IP,               /**< ethernet got IP from connected AP */
    SYSTEM_EVENT_MAX
} system_event_id_t;

/* add this macro define for compatible with old IDF version */
#ifndef SYSTEM_EVENT_AP_STA_GOT_IP6
#define SYSTEM_EVENT_AP_STA_GOT_IP6 SYSTEM_EVENT_GOT_IP6
#endif

typedef enum {
    WPS_FAIL_REASON_NORMAL = 0,                   /**< ESP32 WPS normal fail reason */
    WPS_FAIL_REASON_RECV_M2D,                       /**< ESP32 WPS receive M2D frame */
    WPS_FAIL_REASON_MAX
}system_event_sta_wps_fail_reason_t;
typedef struct {
    uint32_t status;          /**< status of scanning APs */
    uint8_t  number;
    uint8_t  scan_id;
} system_event_sta_scan_done_t;

typedef struct {
    uint8_t ssid[32];         /**< SSID of connected AP */
    uint8_t ssid_len;         /**< SSID length of connected AP */
    uint8_t bssid[6];         /**< BSSID of connected AP*/
    uint8_t channel;          /**< channel of connected AP*/
    uint8_t authmode;
} system_event_sta_connected_t;

typedef struct {
    uint8_t ssid[32];         /**< SSID of disconnected AP */
    uint8_t ssid_len;         /**< SSID length of disconnected AP */
    uint8_t bssid[6];         /**< BSSID of disconnected AP */
    uint8_t reason;           /**< reason of disconnection */
} system_event_sta_disconnected_t;

typedef struct {
    uint8_t old_mode;         /**< the old auth mode of AP */
    uint8_t new_mode;         /**< the new auth mode of AP */
} system_event_sta_authmode_change_t;

typedef struct {
    tcpip_adapter_ip_info_t ip_info;
    bool ip_changed;
} system_event_sta_got_ip_t;

typedef struct {
    uint8_t pin_code[8];         /**< PIN code of station in enrollee mode */
} system_event_sta_wps_er_pin_t;

typedef struct {
    uint8_t mac[6];           /**< MAC address of the station connected to ESP32 soft-AP */
    uint8_t aid;              /**< the aid that ESP32 soft-AP gives to the station connected to  */
} system_event_ap_staconnected_t;

typedef struct {
    uint8_t mac[6];           /**< MAC address of the station disconnects to ESP32 soft-AP */
    uint8_t aid;              /**< the aid that ESP32 soft-AP gave to the station disconnects to  */
} system_event_ap_stadisconnected_t;

typedef struct {
    int rssi;                 /**< Received probe request signal strength */
    uint8_t mac[6];           /**< MAC address of the station which send probe request */
} system_event_ap_probe_req_rx_t;

typedef union {
    system_event_sta_connected_t               connected;          /**< ESP32 station connected to AP */
    system_event_sta_disconnected_t            disconnected;       /**< ESP32 station disconnected to AP */
    system_event_sta_scan_done_t               scan_done;          /**< ESP32 station scan (APs) done */
    system_event_sta_authmode_change_t         auth_change;        /**< the auth mode of AP ESP32 station connected to changed */
    system_event_sta_got_ip_t                  got_ip;             /**< ESP32 station got IP, first time got IP or when IP is changed */
    system_event_sta_wps_er_pin_t              sta_er_pin;         /**< ESP32 station WPS enrollee mode PIN code received */
    system_event_sta_wps_fail_reason_t         sta_er_fail_reason;/**< ESP32 station WPS enrollee mode failed reason code received */
    system_event_ap_staconnected_t             sta_connected;      /**< a station connected to ESP32 soft-AP */
    system_event_ap_stadisconnected_t          sta_disconnected;   /**< a station disconnected to ESP32 soft-AP */
    system_event_ap_probe_req_rx_t             ap_probereqrecved;  /**< ESP32 soft-AP receive probe request packet */
    //system_event_got_ip6_t                     got_ip6;            /**< ESP32 station　or ap or ethernet ipv6 addr state change to preferred */
} system_event_info_t;

typedef struct {
    system_event_id_t     event_id;      /**< event ID */
    system_event_info_t   event_info;    /**< event information */
} system_event_t;

typedef esp_err_t (*system_event_handler_t)(system_event_t *event);

void wifi_callback_init();
void wifi_event_reg_init();
void wifi_callback_ind(system_event_id_t event, uint8_t *data, uint32_t len);

#endif /* _PRC_WIFI_CALLBACK_H_ */