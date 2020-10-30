
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_main.c
   * @brief     Source file for WiFi project, mainly used for initialize modules
   * @author    Hongtai.Liu
   * @date      2020-09-24
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/

#include "elog.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "rpc_system_header.h"
#include "wifi_callback.h"
#include "wifi_main.h"
#include "mbedtls/threading.h"
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha1.h"
#include "mbedtls/pk.h"
#include "mbedtls/x509_crt.h"

static uint16_t _networkCount = 0;
static bool isScaning = false;
static rtw_scan_result_t _scan_networks[WL_NETWORKS_LIST_MAXNUM] = {0};

// /**
//  * @brief Implementation of mbedtls_mutex_init for thread-safety.
//  *
//  */
// void my_mbedtls_mutex_init( mbedtls_threading_mutex_t * mutex )
// {
//     if( mutex->is_valid == 0 )
//     {
//         mutex->mutex = xSemaphoreCreateMutex();

//         if( mutex->mutex != NULL )
//         {
//             mutex->is_valid = 1;
//         }
//         else
//         {
//             log_e( ( "Failed to initialize mbedTLS mutex.\r\n" ) );
//         }
//     }
// }

// /**
//  * @brief Implementation of mbedtls_mutex_free for thread-safety.
//  *
//  */
// void my_mbedtls_mutex_free( mbedtls_threading_mutex_t * mutex )
// {
//     if( mutex->is_valid == 1 )
//     {
//         vSemaphoreDelete( mutex->mutex );
//         mutex->is_valid = 0;
//     }
// }

// /**
//  * @brief Implementation of mbedtls_mutex_lock for thread-safety.
//  *
//  * @return 0 if successful, MBEDTLS_ERR_THREADING_MUTEX_ERROR if timeout,
//  * MBEDTLS_ERR_THREADING_BAD_INPUT_DATA if the mutex is not valid.
//  */
// int my_mbedtls_mutex_lock( mbedtls_threading_mutex_t * mutex )
// {
//     int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

//     if( mutex->is_valid == 1 )
//     {
//         if( xSemaphoreTake( mutex->mutex, portMAX_DELAY ) )
//         {
//             ret = 0;
//         }
//         else
//         {
//             ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
//             log_e( ( "Failed to obtain mbedTLS mutex.\r\n" ) );
//         }
//     }

//     return ret;
// }

// /**
//  * @brief Implementation of mbedtls_mutex_unlock for thread-safety.
//  *
//  * @return 0 if successful, MBEDTLS_ERR_THREADING_MUTEX_ERROR if timeout,
//  * MBEDTLS_ERR_THREADING_BAD_INPUT_DATA if the mutex is not valid.
//  */
// int my_mbedtls_mutex_unlock( mbedtls_threading_mutex_t * mutex )
// {
//     int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

//     if( mutex->is_valid == 1 )
//     {
//         if( xSemaphoreGive( mutex->mutex ) )
//         {
//             ret = 0;
//         }
//         else
//         {
//             ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
//             log_e( ( "Failed to unlock mbedTLS mutex.\r\n" ) );
//         }
//     }

//     return ret;
// }

// /**
//  * @brief Implements libc calloc semantics using the FreeRTOS heap
//  */
// static void * myPrvCalloc( size_t xNmemb,
//                          size_t xSize )
// {
//     void * pvNew = pvPortMalloc( xNmemb * xSize );

//     if( NULL != pvNew )
//     {
//         memset( pvNew, 0, xNmemb * xSize );
//     }

//     return pvNew;
// }


bool wifi_init()
{

  wifi_callback_init();

  wifi_on(RTW_MODE_NONE);

  wifi_manager_init();

  wifi_event_reg_init();

  // mbedtls_threading_set_alt( my_mbedtls_mutex_init,
  //                                  my_mbedtls_mutex_free,
  //                                  my_mbedtls_mutex_lock,
  //                                  my_mbedtls_mutex_unlock );
                                  

  return true;
}

rtw_result_t wifi_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result)
{
  rtw_scan_result_t *record = &malloced_scan_result->ap_details;
  record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

  if (malloced_scan_result->scan_complete != RTW_TRUE)
  {
    if (_networkCount < WL_NETWORKS_LIST_MAXNUM)
    {
      memcpy(&(_scan_networks[_networkCount]), record, sizeof(rtw_scan_result_t));
      _networkCount++;
    }
  }
  else
  {
    isScaning = false;
    wifi_callback_ind(SYSTEM_EVENT_SCAN_DONE, NULL, 0);
  }
  return RTW_SUCCESS;
}

uint16_t wifi_scan_get_ap_num()
{
  log_d("called");
  return _networkCount;
}

int32_t wifi_scan_get_ap_records(uint16_t number, rtw_scan_result_t *_scanResult)
{
  log_d("called");
  if (number > WL_NETWORKS_LIST_MAXNUM || _scanResult == NULL)
  {
    return RTW_ERROR;
  }

  memcpy(_scanResult, _scan_networks, sizeof(rtw_scan_result_t) * number);
  return RTW_SUCCESS;
}

bool wifi_is_scaning()
{
  return isScaning;
}

int32_t wifi_scan_start()
{
  log_d("called");
  _networkCount = 0;
  int32_t ret = wifi_scan_networks(wifi_scan_result_handler, NULL);
  if (ret == RTW_SUCCESS)
  {
    isScaning = true;
  }
  return ret;
}
