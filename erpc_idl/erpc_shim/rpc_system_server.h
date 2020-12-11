/*
 * Generated by erpcgen 1.7.4 on Fri Dec 11 16:32:29 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_rpc_system_server_h_)
#define _rpc_system_server_h_

#ifdef __cplusplus
#include "erpc_server.h"
#include "erpc_codec.h"
extern "C"
{
#include "rpc_system.h"
#include <stdint.h>
#include <stdbool.h>
}

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


/*!
 * @brief Service subclass for rpc_system.
 */
class rpc_system_service : public erpc::Service
{
public:
    rpc_system_service() : Service(krpc_system_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for rpc_system_version of rpc_system interface. */
    erpc_status_t rpc_system_version_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);

    /*! @brief Server shim for rpc_system_ack of rpc_system interface. */
    erpc_status_t rpc_system_ack_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#else
#include "rpc_system.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_rpc_system_service(void);

void destroy_rpc_system_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _rpc_system_server_h_
