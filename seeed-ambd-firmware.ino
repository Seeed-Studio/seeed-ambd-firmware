#include "elog.h"
#include "wifi_main.h"

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

extern void erpc_system_init();
void setup()
{
	wifi_init();
	app_elog_init();
#if DEBUG_LOCAL
	ble_init();
	//ble_server_init(BLE_SERVER_MAX_APPS);

	// le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
	// le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

	// ble_service_t srcv;

	// srcv.uuid_length = UUID_16BIT_SIZE;
	// uint16_t srcv_uuid = 0x180F;
	// memcpy(&(srcv.uuid), &srcv_uuid, 2);
	// srcv.is_primary = true;

	// srcv_app_id = ble_create_service(srcv);
	// printf("srcv_app_id: %d\n\r", srcv_app_id);

	// ble_char_t CHAR;
	// CHAR.uuid_length = UUID_16BIT_SIZE;
	// uint16_t CHAR_uuid = 0x2A19;
	// memcpy(&(CHAR.uuid), &CHAR_uuid, 2);
	// CHAR.properties = (GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY);
	// CHAR.permissions = GATT_PERM_READ | GATT_PERM_WRITE;
	// char_handle = ble_create_char(srcv_app_id, CHAR);
	// printf("char_handle: %d\n\r", char_handle);
	// ble_desc_t desc;

	// desc.flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;
	// desc.uuid_length = UUID_16BIT_SIZE;
	// uint16_t desc_uuid = 0x2902;
	// uint16_t default_vlaue = 0x0000;
	// memcpy(&(desc.uuid), &desc_uuid, 2);
	// memcpy(&(desc.uuid[2]), &default_vlaue, 2);
	// desc.p_value = NULL;
	// desc.vlaue_length = 2;
	// desc.permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
	// desc_handle1 = ble_create_desc(srcv_app_id, char_handle, desc);
	// printf("desc_handle1: %d\n\r", desc_handle1);

	// ble_desc_t desc2;
	// desc2.flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
	// desc2.uuid_length = UUID_16BIT_SIZE;
	// uint16_t desc2_uuid = 0x2901;
	// memcpy(&(desc2.uuid), &desc2_uuid, 2);
	// desc2.p_value = (uint8_t *)v1_user_descr;
	// desc2.vlaue_length = (sizeof(v1_user_descr) - 1);
	// desc2.permissions = (GATT_PERM_READ);
	// desc_handle2 = ble_create_desc(srcv_app_id, char_handle, desc2);
	// printf("desc_handle2: %d\n\r", desc_handle2);

	// srcv_handle = ble_service_start(srcv_app_id);
	ble_start();
	ble_client_init(1);

	delay(2000);
	//le_adv_start();
	// le_scan_start();
	// delay(10000);
	// le_scan_stop();
	// delay(2000);

	T_CLIENT_ID client_id = ble_add_client(0, BLE_LE_MAX_LINKS);

	printf("connecting...\n\r");
	uint8_t bd_addr[6] = {0xd7, 0x1d, 0x12, 0xdc, 0x64, 0xf0};

	T_GAP_LE_CONN_REQ_PARAM _connReqParam;
	_connReqParam.scan_interval = 0x40;
	_connReqParam.scan_window = 0x30;
	_connReqParam.conn_interval_min = 80;
	_connReqParam.conn_interval_max = 80;
	_connReqParam.conn_latency = 0;
	_connReqParam.supv_tout = 1000;
	_connReqParam.ce_len_min = 2 * (_connReqParam.conn_interval_min - 1);
	_connReqParam.ce_len_max = 2 * (_connReqParam.conn_interval_max - 1);

	le_set_conn_param(GAP_CONN_PARAM_1M, &_connReqParam);

	while (1)
	{
		log_i("cmd_con, DestAddr: 0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X\r\n",
			  bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
		T_GAP_CAUSE result = le_connect(0, bd_addr, GAP_REMOTE_ADDR_LE_RANDOM, GAP_LOCAL_ADDR_LE_PUBLIC, 2000);

		le_get_conn_id(bd_addr, GAP_REMOTE_ADDR_LE_RANDOM, &conn_id);
		log_i("conn_id: %d\r\n", conn_id);
		delay(3000);
		T_GAP_CONN_INFO pConnInfo;
		le_get_conn_info(conn_id, &pConnInfo);
		if (pConnInfo.conn_state == GAP_CONN_STATE_CONNECTED)
		{
			log_i("Connect successful\r\n");
			client_all_primary_srv_discovery(conn_id, client_id);
			break;
		}
		else
		{
			log_i("Connect failed:%d\r\n", conn_id);
			delay(8000);
		}
	}

#else
	erpc_system_init();
#endif
}

void loop()
{
	delay(10000);
}