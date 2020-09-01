#include "elog.h"
#include "ble_task.h"
#include "ble_callback.h"
#include "ble_common.h"
#include "ble_main.h"
#include "gcs_client.h"
#include "gap_config.h"
#include "bas_client.h"
#include "gatt.h"

#define GATT_UUID_SIMPLE_PROFILE 0xA00A
#define GATT_UUID_CHAR_SIMPLE_V1_READ 0xB001
#define GATT_UUID_CHAR_SIMPLE_V2_WRITE 0xB002
#define GATT_UUID_CHAR_SIMPLE_V3_NOTIFY 0xB003
#define GATT_UUID_CHAR_SIMPLE_V4_INDICATE 0xB004
#define SIMPLE_BLE_SERVICE_CHAR_V1_READ_INDEX 0x02
#define SIMPLE_BLE_SERVICE_CHAR_V2_WRITE_INDEX 0x05
#define SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX 0x07
#define SIMPLE_BLE_SERVICE_CHAR_V4_INDICATE_INDEX 0x0a
#define SIMPLE_BLE_SERVICE_CHAR_NOTIFY_CCCD_INDEX (SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX + 1)
#define SIMPLE_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX (SIMPLE_BLE_SERVICE_CHAR_V4_INDICATE_INDEX + 1)

char v1_user_descr[] = "V1 read characteristic";

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL simple_ble_service_tbl[] =
	{
		/* <<Primary Service>>, .. */
		{
			(ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE), /* flags     */
			{										   /* type_value */
			 LO_WORD(GATT_UUID_PRIMARY_SERVICE),
			 HI_WORD(GATT_UUID_PRIMARY_SERVICE),
			 LO_WORD(GATT_UUID_SIMPLE_PROFILE), /* service UUID */
			 HI_WORD(GATT_UUID_SIMPLE_PROFILE)},
			UUID_16BIT_SIZE, /* bValueLen     */
			NULL,			 /* p_value_context */
			GATT_PERM_READ	 /* permissions  */
		},
		/* <<Characteristic>> demo for read */
		{
			ATTRIB_FLAG_VALUE_INCL, /* flags */
			{
				/* type_value */
				LO_WORD(GATT_UUID_CHARACTERISTIC),
				HI_WORD(GATT_UUID_CHARACTERISTIC),
				GATT_CHAR_PROP_READ /* characteristic properties */
				/* characteristic UUID not needed here, is UUID of next attrib. */
			},
			1, /* bValueLen */
			NULL,
			GATT_PERM_READ /* permissions */
		},
		{
			ATTRIB_FLAG_VALUE_APPL, /* flags */
			{						/* type_value */
			 LO_WORD(GATT_UUID_CHAR_SIMPLE_V1_READ),
			 HI_WORD(GATT_UUID_CHAR_SIMPLE_V1_READ)},
			0, /* bValueLen */
			NULL,
			GATT_PERM_READ /* permissions */
		},
		{
			ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z, /* flags */
			{
				/* type_value */
				LO_WORD(GATT_UUID_CHAR_USER_DESCR),
				HI_WORD(GATT_UUID_CHAR_USER_DESCR),
			},
			(sizeof(v1_user_descr) - 1), /* bValueLen */
			(void *)v1_user_descr,
			GATT_PERM_READ /* permissions */
		},
		/* <<Characteristic>> demo for write */
		{
			ATTRIB_FLAG_VALUE_INCL, /* flags */
			{
				/* type_value */
				LO_WORD(GATT_UUID_CHARACTERISTIC),
				HI_WORD(GATT_UUID_CHARACTERISTIC),
				(GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP) /* characteristic properties */
				/* characteristic UUID not needed here, is UUID of next attrib. */
			},
			1, /* bValueLen */
			NULL,
			GATT_PERM_READ /* permissions */
		},
		{
			ATTRIB_FLAG_VALUE_APPL, /* flags */
			{						/* type_value */
			 LO_WORD(GATT_UUID_CHAR_SIMPLE_V2_WRITE),
			 HI_WORD(GATT_UUID_CHAR_SIMPLE_V2_WRITE)},
			0, /* bValueLen */
			NULL,
			GATT_PERM_WRITE /* permissions */
		},
		/* <<Characteristic>>, demo for notify */
		{
			ATTRIB_FLAG_VALUE_INCL, /* flags */
			{
				/* type_value */
				LO_WORD(GATT_UUID_CHARACTERISTIC),
				HI_WORD(GATT_UUID_CHARACTERISTIC),
				(GATT_CHAR_PROP_NOTIFY) /* characteristic properties */
				/* characteristic UUID not needed here, is UUID of next attrib. */
			},
			1, /* bValueLen */
			NULL,
			GATT_PERM_READ /* permissions */
		},
		{
			ATTRIB_FLAG_VALUE_APPL, /* flags */
			{						/* type_value */
			 LO_WORD(GATT_UUID_CHAR_SIMPLE_V3_NOTIFY),
			 HI_WORD(GATT_UUID_CHAR_SIMPLE_V3_NOTIFY)},
			0, /* bValueLen */
			NULL,
			GATT_PERM_NONE /* permissions */
		},
		/* client characteristic configuration */
		{
			ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL, /* flags */
			{												/* type_value */
			 LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			 HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			 /* NOTE: this value has an instantiation for each client, a write to */
			 /* this attribute does not modify this default value:                */
			 LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
			 HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)},
			2, /* bValueLen */
			NULL,
			(GATT_PERM_READ | GATT_PERM_WRITE) /* permissions */
		},
		/* <<Characteristic>> demo for indicate */
		{
			ATTRIB_FLAG_VALUE_INCL, /* flags */
			{
				/* type_value */
				LO_WORD(GATT_UUID_CHARACTERISTIC),
				HI_WORD(GATT_UUID_CHARACTERISTIC),
				(GATT_CHAR_PROP_INDICATE) /* characteristic properties */
				/* characteristic UUID not needed here, is UUID of next attrib. */
			},
			1, /* bValueLen */
			NULL,
			GATT_PERM_READ /* permissions */
		},
		{
			ATTRIB_FLAG_VALUE_APPL, /* flags */
			{						/* type_value */
			 LO_WORD(GATT_UUID_CHAR_SIMPLE_V4_INDICATE),
			 HI_WORD(GATT_UUID_CHAR_SIMPLE_V4_INDICATE)},
			0, /* bValueLen */
			NULL,
			GATT_PERM_NONE /* permissions */
		},
		/* client characteristic configuration */
		{
			ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL, /* flags */
			{												/* type_value */
			 LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			 HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			 /* NOTE: this value has an instantiation for each client, a write to */
			 /* this attribute does not modify this default value:                */
			 LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
			 HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)},
			2, /* bValueLen */
			NULL,
			(GATT_PERM_READ | GATT_PERM_WRITE) /* permissions */
		},
};

/** @brief  GAP - scan response data (max size = 31 bytes) */
static const uint8_t scan_rsp_data[] =
	{
		0x03,				   /* length */
		GAP_ADTYPE_APPEARANCE, /* type="Appearance" */
		LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
		HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
};

/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
static const uint8_t adv_data[] =
	{
		/* Flags */
		0x02,			  /* length */
		GAP_ADTYPE_FLAGS, /* type="Flags" */
		GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
		/* Service */
		0x03, /* length */
		GAP_ADTYPE_16BIT_COMPLETE,
		LO_WORD(GATT_UUID_SIMPLE_PROFILE),
		HI_WORD(GATT_UUID_SIMPLE_PROFILE),
		/* Local name */
		0x0F, /* length */
		GAP_ADTYPE_LOCAL_NAME_COMPLETE,
		'B',
		'L',
		'E',
		'_',
		'P',
		'E',
		'R',
		'I',
		'P',
		'H',
		'E',
		'R',
		'A',
		'L',
};

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
	ble_server_init(BLE_SERVER_MAX_APPS);

	// //delay(2000);
	T_SERVER_ID server_id0 = ble_add_service(simple_ble_service_tbl, sizeof(simple_ble_service_tbl));
	T_SERVER_ID server_id1 = ble_add_service(simple_ble_service_tbl, sizeof(simple_ble_service_tbl));

	le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
	le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);
	// T_CLIENT_ID client_id = ble_add_client(0, BLE_LE_MAX_LINKS);
	// printf("client id: %d\n\r", client_id);

	ble_start();

	//delay(2000);
	le_adv_start();

	// printf("scanning...\n\r");
	// int16_t _scanInterval = 0x600;
	// le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(_scanInterval), &_scanInterval);
	// le_scan_start();
	// delay(2000);
	// le_scan_stop();
	// delay(2000);
	// printf("connecting...\n\r");
	// uint8_t bd_addr[6] = {0x7d, 0x18, 0x1b, 0xf1, 0xf7, 0x2c};

	// T_GAP_LE_CONN_REQ_PARAM conn_req_param;
	// conn_req_param.scan_interval = 0x10;
	// conn_req_param.scan_window = 0x10;
	// conn_req_param.conn_interval_min = 80;
	// conn_req_param.conn_interval_max = 80;
	// conn_req_param.conn_latency = 0;
	// conn_req_param.supv_tout = 1000;
	// conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
	// conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);
	// le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);

	// printf("cmd_con, DestAddr: 0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X\r\n",
	// 	   bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	// T_GAP_CAUSE result = le_connect(0, bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, GAP_LOCAL_ADDR_LE_PUBLIC, 1000);

	// if (result == GAP_CAUSE_SUCCESS)
	// {

	// 	printf("Connect successful\r\n");
	// }
	// else
	// {
	// 	printf("Connect failed:%d\r\n", result);
	// 	while (1)
	// 	{
	// 	}

	// }
	// le_get_conn_id(bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id);
	// printf("conn_id: %d\r\n", result);
	// delay(3000);
	// //T_GAP_CONN_INFO pConnInfo;
	// //le_get_conn_info(conn_id, &pConnInfo);
	// printf("start client_all_primary_srv_discovery\n\r");
	// client_all_primary_srv_discovery(conn_id, client_id);
#else
	erpc_ble_init();
#endif
}

void loop()
{
	delay(2000);
}