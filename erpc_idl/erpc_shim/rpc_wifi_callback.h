/*
 * Generated by erpcgen 1.7.4 on Wed Nov  4 11:07:47 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_wifi_callback_h_)
#define _rpc_wifi_callback_h_

// Common types header file
#include "rpc_system_header.h"

/*! @brief rpc_wifi_callback identifiers */
enum _rpc_wifi_callback_ids
{
    krpc_wifi_callback_service_id = 18,
    krpc_wifi_callback_rpc_wifi_event_callback_id = 1,
    krpc_wifi_callback_rpc_wifi_dns_found_id = 2,
};

#if defined(__cplusplus)
extern "C" {
#endif

//! @name rpc_wifi_callback
//@{
void rpc_wifi_event_callback(const binary_t * event);

void rpc_wifi_dns_found(const char * hostname, const binary_t * ipaddr, const binary_t * arg);
//@}

#if defined(__cplusplus)
}
#endif

#endif // _rpc_wifi_callback_h_
