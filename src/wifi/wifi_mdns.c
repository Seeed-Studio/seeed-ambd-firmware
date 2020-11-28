
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
#include "rpc_system_header.h"
#include "mdns_private.h"
#include "mdns_common.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "wifi_callback.h"
#include "wifi_main.h"
#include <errno.h>

typedef struct {
    const char key[MDNS_NAME_BUF_LEN];
    const char value[MDNS_NAME_BUF_LEN];
} rpc_mdns_txt_item_t;

typedef struct rpc_mdns_result_basic_s {
    tcpip_adapter_if_t tcpip_if;
    mdns_ip_protocol_t ip_protocol;
    // PTR
    char instance_name[MDNS_NAME_BUF_LEN];
    // SRV
    char hostname[MDNS_NAME_BUF_LEN];
    uint16_t port;
    // TXT
    uint16_t txt_count;
    // A and AAAA
    uint16_t addr_count;
} rpc_mdns_result_basic_s;

mdns_result_t * results = NULL;

int32_t rpc_mdns_init(void)
{
    log_d("called");

    int32_t ret = mdns_init();

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_free(void)
{
    log_d("called");

    mdns_free();

    log_d("exit");

    return ESP_OK;
}

int32_t rpc_mdns_service_add(const char * instance_name, const char * service_type, const char * proto, uint16_t port)
{
    esp_err_t ret = ESP_FAIL;

    log_d("called");

    if((strlen(instance_name) == 4) && (!strncmp(instance_name,"NULL",4))){
        ret = mdns_service_add(NULL,service_type,proto,port,NULL,0);
    }else{
        ret = mdns_service_add(instance_name,service_type,proto,port,NULL,0);
    }

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_service_remove(const char * service_type, const char * proto)
{
    log_d("called");

    int32_t ret = mdns_service_remove(service_type,proto);

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_service_txt_item_set(const char * service_type, const char * proto, const char * key, const char * value)
{
    log_d("called");

    int32_t ret = mdns_service_txt_item_set(service_type,proto,key,value);

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_instance_name_set(const char * instance_name)
{
    log_d("called");

    int32_t ret = mdns_instance_name_set(instance_name);

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_hostname_set(const char * hostname)
{
    log_d("called");

    int32_t ret = mdns_hostname_set(hostname);

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_service_instance_name_set(const char * service, const char * proto, const char * instance)
{
    log_d("called");

    int32_t ret = mdns_service_instance_name_set(service,proto,instance);

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_query_a(const char * host_name, uint32_t timeout, binary_t * addr)
{
    log_d("called");

    esp_err_t ret = ESP_FAIL;
    ip_addr_t * result_addr = (ip_addr_t *)erpc_malloc(sizeof(ip_addr_t));

    if(result_addr == NULL){
        printf("rpc_mdns_query_ptr_result malloc failed \r\n");
    }

    ret = mdns_query_a(host_name,timeout,result_addr);

    addr->data = result_addr;
    addr->dataLength = sizeof(ip_addr_t);

    log_d("exit");

    return ret; 
}

int32_t rpc_mdns_query_ptr(const char * service_type, const char * proto, uint32_t timeout, int32_t max_results, int32_t * result_total)
{
    log_d("called");

    esp_err_t ret = ESP_FAIL;
    int count = 0;

    mdns_query_results_free(results);
    ret = mdns_query_ptr(service_type,proto,timeout,max_results,&results);
    if(!ret && results){
        mdns_result_t * r = results;

        while(r){
            count++;
            r = r->next;
        }
        *result_total = count;
    }

    log_d("exit");

    return ret; 
}

int32_t rpc_mdns_query_ptr_result_txt(int32_t result_target, int32_t txt_target, binary_t * txt)
{
    log_d("called");

    esp_err_t ret = ESP_FAIL;
    int count_result = 0;

    mdns_result_t * r = results;
    while(r && count_result != result_target){
        count_result++;
        r = r->next;
    }

    if(r != NULL && count_result == result_target){
        if(txt_target < r->txt_count){
            if(r->txt[txt_target].key != NULL && r->txt[txt_target].value != NULL){
                int32_t key_size = strlen(r->txt[txt_target].key) + 1;
                int32_t value_size = strlen(r->txt[txt_target].value) + 1;
                int32_t size = key_size + value_size + sizeof(uint16_t) * 2;
                txt->data = erpc_malloc(size);
                txt->dataLength = size;
                if(txt->data == NULL){
                    printf("rpc_mdns_query_ptr_result_txt malloc failed \r\n");
                }
                uint16_t * key_size_offset = &txt->data[0];
                uint16_t * value_size_offset = &txt->data[sizeof(uint16_t)];
                uint8_t * key_offset = &txt->data[sizeof(uint16_t) * 2];
                uint8_t * value_offset = &txt->data[sizeof(uint16_t) * 2 + key_size];
                *key_size_offset = key_size;
                *value_size_offset = value_size;
                memcpy(key_offset,r->txt[txt_target].key,key_size);
                memcpy(value_offset,r->txt[txt_target].value,value_size);
                txt->data[sizeof(uint16_t) * 2 + key_size - 1] = '\0';
                txt->data[size - 1] = '\0';

                ret = ESP_OK;
            }
        }
    }

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_query_ptr_result_addr(int32_t result_target, int32_t addr_target, binary_t * addr)
{
    log_d("called");

    esp_err_t ret = ESP_FAIL;
    int count_result = 0;
    int count_addr = 0;
    mdns_ip_addr_t * mdns_addr;
    mdns_result_t * r = results;

    while(r && count_result != result_target){
        count_result++;
        r = r->next;
    }

    if(r != NULL && count_result == result_target){
        mdns_addr = r->addr;
        while(mdns_addr && count_addr != addr_target){
            count_addr++;
            mdns_addr = mdns_addr->next;
        }

        if(mdns_addr != NULL && count_addr == addr_target){
            ip_addr_t * result = (ip_addr_t *)erpc_malloc(sizeof(ip_addr_t));
            if(result == NULL){
                printf("rpc_mdns_query_ptr_result_addr malloc failed \r\n");
            }
            result->addr = mdns_addr->addr.addr;
            addr->data = result;
            addr->dataLength = sizeof(ip_addr_t);
            ret = ESP_OK;
        }
    }

    log_d("exit");

    return ret;
}

int32_t rpc_mdns_query_ptr_result_basic(int32_t result_target, binary_t * scan_result)
{
    log_d("called");

    esp_err_t ret = ESP_FAIL;
    int count_result = 0;
    int count_addr = 0;
    mdns_ip_addr_t * mdns_addr;

    rpc_mdns_result_basic_s * result = (rpc_mdns_result_basic_s *)erpc_malloc(sizeof(rpc_mdns_result_basic_s));
    if(result == NULL){
        printf("rpc_mdns_query_ptr_result malloc failed \r\n");
    }

    mdns_result_t * r = results;
    while(r && count_result != result_target){
        count_result++;
        r = r->next;
    }

    scan_result->data = result;
    scan_result->dataLength = sizeof(rpc_mdns_result_basic_s);

    if(r != NULL && count_result == result_target){
        result->tcpip_if = r->tcpip_if;
        result->ip_protocol = r->ip_protocol;
        result->port = r->port;
        result->txt_count = r->txt_count;
        strncpy(result->instance_name,r->instance_name,MDNS_NAME_BUF_LEN);
        strncpy(result->hostname,r->hostname,MDNS_NAME_BUF_LEN);
        mdns_addr = r->addr;
        while(mdns_addr != NULL){
            count_addr++;
            mdns_addr = mdns_addr->next;
        }
        result->addr_count = count_addr;
        ret = ESP_OK;
    }

    log_d("exit");

    return ret; 
}

int32_t rpc_mdns_query_results_free(void)
{
    log_d("called");

    mdns_query_results_free(results);

    log_d("exit");

    return ESP_OK;    
}

