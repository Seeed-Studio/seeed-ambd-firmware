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

void wifi_callback_init();
void wifi_callback_ind(system_event_id_t event);
#endif /* _PRC_WIFI_CALLBACK_H_ */