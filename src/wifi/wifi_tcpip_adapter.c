
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      wifi_tcpip_adapter.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-10-9
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

#include "main.h"
#include "wifi_tcpip_adapter.h"
#include <platform/platform_stdlib.h>
#include "netif.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "lwip_netconf.h"
#include "lwip/err.h"
#include <dhcp/dhcps.h>
#include "elog.h"

extern struct netif xnetif[NET_IF_NUM];

void tcpip_adapter_init(void)
{
    LwIP_Init();
}

esp_err_t tcpip_adapter_eth_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    return ESP_OK;
}

esp_err_t tcpip_adapter_sta_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    esp_err_t ret = ESP_OK;
    uint8_t dhcp_result;

    if (RTW_SUCCESS != wifi_set_mac_address(mac))
    {
        ret = ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
        goto exit;
    }

    dhcp_result = LwIP_DHCP(0, DHCP_START);

    if (dhcp_result != DHCP_ADDRESS_ASSIGNED)
    {
        ret = ESP_ERR_TCPIP_ADAPTER_DHCPC_START_FAILED;
        goto exit;
    }

exit:
    return ret;
}

esp_err_t tcpip_adapter_ap_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    esp_err_t ret = ESP_OK;

    if (RTW_SUCCESS != wifi_set_mac_address(mac))
    {
        ret = ESP_ERR_TCPIP_ADAPTER_IF_NOT_READY;
    }

    uint8_t dhcp_result;
    struct netif *pnetif = &xnetif[0];

    dhcps_deinit();
    wifi_off();
    vTaskDelay(20);
    if (wifi_on(RTW_MODE_AP) < 0)
    {
        ret = ESP_ERR_TCPIP_ADAPTER_IF_NOT_READY;
        goto exit;
    }

    netif_set_addr(pnetif, &ip_info->ip, &ip_info->netmask, &ip_info->gw);

    dhcps_init(pnetif);

exit:
    return ret;
}

esp_err_t tcpip_adapter_stop(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    uint8_t dhcp_result = LwIP_DHCP(idx, DHCP_STOP);
    if (DHCP_STOP != dhcp_result)
        return ESP_FAIL;
    dhcps_deinit();
    return ret;
}

esp_err_t tcpip_adapter_up(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    struct netif *pnetif = &xnetif[idx];
    if (idx <= NET_IF_NUM)
    {
        dhcps_init(pnetif);
    }
    else
    {
        return ESP_ERR_TCPIP_ADAPTER_IF_NOT_READY;
    }

    return ret;
}

esp_err_t tcpip_adapter_down(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    dhcps_deinit();
    wifi_off();
    return ret;
}

esp_err_t tcpip_adapter_get_ip_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_ip_info_t *ip_info)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    struct netif *pnetif = &xnetif[idx];
    memcpy(&ip_info->ip, &pnetif->ip_addr, sizeof(ip_addr_t));
    memcpy(&ip_info->netmask, &pnetif->netmask, sizeof(ip_addr_t));
    memcpy(&ip_info->gw, &pnetif->gw, sizeof(ip_addr_t));
    log_v("tcpip_if:%d ip_addr:%d netmask:%d, gw:%d", tcpip_if, ip_info->ip,ip_info->netmask, ip_info->gw);
    return ret;
}

esp_err_t tcpip_adapter_set_ip_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_ip_info_t *ip_info)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    struct netif *pnetif = &xnetif[idx];

    // memcpy(&pnetif->ip_addr, &ip_info->ip, sizeof(ip_addr_t));
    // memcpy(&pnetif->netmask, &ip_info->netmask, sizeof(ip_addr_t));
    // memcpy(&pnetif->gw, &ip_info->gw, sizeof(ip_addr_t));
    // log_v("tcpip_if:%d ip_addr:%d netmask:%d, gw:%d", tcpip_if, pnetif->ip_addr, pnetif->netmask, pnetif->gw);
    netif_set_addr(pnetif, &ip_info->ip, &ip_info->netmask, &ip_info->gw);
    log_v("tcpip_if:%d ip_addr:%d netmask:%d, gw:%d", tcpip_if, pnetif->ip_addr, pnetif->netmask, pnetif->gw);
    return ret;
}
esp_err_t tcpip_adapter_set_dns_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_dns_type_t type, tcpip_adapter_dns_info_t *dns)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    dns_setserver(idx, &dns->ip);

    return ret;
}

esp_err_t tcpip_adapter_get_dns_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_dns_type_t type, tcpip_adapter_dns_info_t *dns)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    ip_addr_t *ip = dns_getserver(idx);

    memcpy(&dns->ip, ip, sizeof(ip_addr_t));

    return ret;
}

esp_err_t tcpip_adapter_dhcps_start(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    uint8_t idx = tcpip_if<=1?0:1;
    struct netif *pnetif = &xnetif[idx];
    //log_v("tcpip_adapter_dhcps_start tcpip_if:%d idx:%d", tcpip_if, idx);
    dhcps_init(pnetif);
    return ret;
}

esp_err_t tcpip_adapter_dhcps_stop(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    dhcps_deinit();
    vTaskDelay(10);
    //log_v("tcpip_adapter_dhcps_stop tcpip_if:%d", tcpip_if);
    return ret;
}

esp_err_t tcpip_adapter_dhcpc_start(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    uint8_t dhcp_result;
    uint8_t idx = tcpip_if<=1?0:1;
   
    dhcp_result = LwIP_DHCP(idx, DHCP_START);

    if (dhcp_result != DHCP_ADDRESS_ASSIGNED)
    {
        ret = ESP_FAIL;
    }

    return ret;
}

esp_err_t tcpip_adapter_dhcpc_stop(tcpip_adapter_if_t tcpip_if)
{
    esp_err_t ret = ESP_OK;
    uint8_t dhcp_result;
    uint8_t idx = tcpip_if<=1?0:1;
    dhcp_result = LwIP_DHCP(idx, DHCP_STOP);

    if (dhcp_result != DHCP_STOP)
    {
        ret = ESP_FAIL;
    }

    return ret;
}

esp_err_t tcpip_adapter_set_hostname(tcpip_adapter_if_t tcpip_if, const char *hostname)
{
    //netif_set_hostname(&xnetif[idx], hostname);
    return ESP_OK;
}

esp_err_t tcpip_adapter_get_hostname(tcpip_adapter_if_t tcpip_if, const char **hostname)
{

    //*hostname = netif_get_hostname(&xnetif[idx]);
    return ESP_OK;
}

esp_err_t tcpip_adapter_get_mac(tcpip_adapter_if_t tcpip_if, uint8_t *mac)
{
    wifi_get_mac_address(mac);
    return ESP_OK;
}

esp_err_t tcpip_adapter_set_mac(tcpip_adapter_if_t tcpip_if, uint8_t *mac)
{
    wifi_set_mac_address(mac);
    return ESP_OK;
}