/*
 * Generated by erpcgen 1.7.4 on Tue Nov  3 16:25:43 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_wifi_callback_server_h_)
#define _rpc_wifi_callback_server_h_

#ifdef __cplusplus
#include "erpc_server.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_wifi_callback.h"
#include <stdint.h>
#include <stdbool.h>
}

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


/*!
 * @brief Service subclass for rpc_wifi_callback.
 */
class rpc_wifi_callback_service : public erpc::Service
{
public:
    rpc_wifi_callback_service() : Service(krpc_wifi_callback_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_wifi_event_callback of rpc_wifi_callback interface. */
    erpc_status_t rpc_wifi_event_callback_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_wifi_dns_found of rpc_wifi_callback interface. */
    erpc_status_t rpc_wifi_dns_found_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#else
#include "rpc_wifi_callback.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_rpc_wifi_callback_service(void);

void destroy_rpc_wifi_callback_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _rpc_wifi_callback_server_h_
