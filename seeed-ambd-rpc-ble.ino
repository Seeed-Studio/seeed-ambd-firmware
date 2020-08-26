#include "elog.h"
#include "ble_task.h"
#include "ble_callback.h"
#include "ble_common.h"
#include "ble_main.h"
#include "gcs_client.h"
#include "gap_config.h"
#include "bas_client.h"

/*init the easylogger moudle*/
void app_elog_init(void)
{

	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);

	/* allow color */
	elog_set_text_color_enabled(true);

	/* start EasyLogger */
	elog_start();
}
extern void erpc_ble_init();

uint8_t conn_id = 0xff;

void setup()
{
	app_elog_init();
#if DEBUG_LOCAL
	ble_init();
	ble_client_init(BLE_CLIENT_MAX_APPS);
	ble_start();
	delay(2000);

	
	T_CLIENT_ID client_id = ble_add_client(0, BLE_CLIENT_MAX_LINKS);

	printf("client id: %d\n\r", client_id);

	printf("scanning...\n\r");
	int16_t _scanInterval = 0x600;          
    le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(_scanInterval), &_scanInterval);
	le_scan_start();
	delay(2000);
	le_scan_stop();
	delay(2000);
	printf("connecting...\n\r");
	uint8_t bd_addr[6] = {0x7d, 0x18, 0x1b, 0xf1, 0xf7, 0x2c};

	T_GAP_LE_CONN_REQ_PARAM conn_req_param;
	conn_req_param.scan_interval = 0x10;
	conn_req_param.scan_window = 0x10;
	conn_req_param.conn_interval_min = 80;
	conn_req_param.conn_interval_max = 80;
	conn_req_param.conn_latency = 0;
	conn_req_param.supv_tout = 1000;
	conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
	conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);
	le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);

	printf("cmd_con, DestAddr: 0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X\r\n",
		   bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	T_GAP_CAUSE result = le_connect(0, bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, GAP_LOCAL_ADDR_LE_PUBLIC, 1000);

	if (result == GAP_CAUSE_SUCCESS)
	{

		printf("Connect successful\r\n");
	}
	else
	{
		printf("Connect failed:%d\r\n", result);
		while (1)
		{
		}
		
	}
	le_get_conn_id(bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id);
	printf("conn_id: %d\r\n", result);
	delay(3000);
	//T_GAP_CONN_INFO pConnInfo;
	//le_get_conn_info(conn_id, &pConnInfo);
	printf("start client_all_primary_srv_discovery\n\r");
	client_all_primary_srv_discovery(conn_id, client_id);
#else
	erpc_ble_init();
#endif
}

void loop()
{
    delay(2000);
}