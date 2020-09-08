
/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      erpc_setup.cpp
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-12
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/

#include "rpc_ble_api.h"
#include "rpc_ble_api_server.h"
#include "erpc_arduino_uart_transport.h"
#include "erpc_basic_codec.h"
#include "erpc_simple_server.h"
#include "erpc_arbitrated_client_manager.h"
#include "erpc_threading.h"
#include "erpc_transport_arbitrator.h"
#include "erpc_port.h"
#include "elog.h"

using namespace erpc;

/*============================================================================*
 *                              Constants
 *============================================================================*/

UartTransport g_transport(&Serial2, 115200);
BasicCodecFactory g_basicCodecFactory;
ArbitratedClientManager *g_client;
TransportArbitrator g_arbitrator;
SimpleServer g_server;
Crc16 g_crc16;

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief  Initialize erpc server task
 * @return void
 */

class MyMessageBufferFactory : public MessageBufferFactory
{
public:
    virtual MessageBuffer create()
    {
        uint8_t *buf = new uint8_t[4096];
        return MessageBuffer(buf, 4096);
    }

    virtual void dispose(MessageBuffer *buf)
    {
        assert(buf);
        if (*buf)
        {
            delete[] buf->get();
        }
    }
};

MyMessageBufferFactory g_msgFactory;

/**
 * @brief  Initialize erpc server task
 * @return void
 */
void add_services(erpc::SimpleServer *server)
{
    server->addService(static_cast<erpc::Service *>(create_host_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_bone_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_adv_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_le_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_config_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_conn_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gap_scan_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gatt_client_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_gatt_server_service()));
}

/**
 * @brief  Initialize erpc server task
 * @return void
 */
void run_erpc_server(void *arg)
{
    (void)arg;
    log_i("start erpc server\n\r");
    /* run server */
    for (;;)
    {
        g_server.poll();
        delay(10);
    }
}

/**
 * @brief  Initialize erpc server task
 * @return void
 */
void erpc_ble_init()
{
    g_transport.init();

    g_arbitrator.setSharedTransport(&g_transport);
    g_arbitrator.setCodec(g_basicCodecFactory.create());

    g_client = new ArbitratedClientManager();
    g_client->setArbitrator(&g_arbitrator);
    g_client->setCodecFactory(&g_basicCodecFactory);
    g_client->setMessageBufferFactory(&g_msgFactory);

    g_arbitrator.setCrc16(&g_crc16);

    g_server.setTransport(&g_arbitrator);
    g_server.setCodecFactory(&g_basicCodecFactory);
    g_server.setMessageBufferFactory(&g_msgFactory);
    Thread serverThread(&run_erpc_server, 8, 4096, "erpc server");
    serverThread.start();

    add_services(&g_server);
    g_client->setServer(&g_server);
    g_client->setServerThreadId(serverThread.getThreadId());
}