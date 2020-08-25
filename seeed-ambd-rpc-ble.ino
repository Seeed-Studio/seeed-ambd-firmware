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
/** @brief Default scan interval (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_INTERVAL 0x520
/** @brief Default scan window (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_WINDOW 0x520

uint8_t conn_id = 0xff;

void setup()
{
	app_elog_init();
#ifdef DEBUG_LOCAL
	ble_init();

	gap_config_max_le_link_num(BLE_CLIENT_MAX_LINKS);
	le_gap_init(BLE_CLIENT_MAX_LINKS);

	/* Device name and device appearance */
	uint8_t device_name[GAP_DEVICE_NAME_LEN] = "BLE_CENTRAL_CLIENT";
	uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

	uint8_t scan_mode = GAP_SCAN_MODE_ACTIVE;
	uint16_t scan_interval = DEFAULT_SCAN_INTERVAL;
	uint16_t scan_window = DEFAULT_SCAN_WINDOW;
	uint8_t scan_filter_policy = GAP_SCAN_FILTER_ANY;
	uint8_t scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;

	/* GAP Bond Manager parameters */
	uint8_t auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
	uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
	uint8_t auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
	uint8_t auth_oob = false;
	uint8_t auth_use_fix_passkey = false;
	uint32_t auth_fix_passkey = 0;
	uint8_t auth_sec_req_enable = false;
	uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

	/* Set device name and device appearance */
	le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
	le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);

	/* Set scan parameters */
	le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(scan_mode), &scan_mode);
	le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(scan_interval), &scan_interval);
	le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(scan_window), &scan_window);
	le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
					  &scan_filter_policy);
	le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
					  &scan_filter_duplicate);

	/* Setup the GAP Bond Manager */
	gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
	gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
	gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
	gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
	le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
	le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
					  &auth_use_fix_passkey);
	le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
	le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
					  &auth_sec_req_flags);

	uint8_t phys_prefer = GAP_PHYS_PREFER_ALL;
	uint8_t tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							 GAP_PHYS_PREFER_CODED_BIT;
	uint8_t rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							 GAP_PHYS_PREFER_CODED_BIT;
	le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);

	le_register_app_cb(ble_gap_callback);
	client_register_general_client_cb(ble_gatt_client_callback);
	ble_start();
	delay(2000);

	ble_client_init(BLE_CLIENT_MAX_APPS);
	T_CLIENT_ID client_id = ble_add_client(0, BLE_CLIENT_MAX_LINKS, ble_gatt_client_callback);

	printf("client id: %d\n\r", client_id);

	printf("scanning...\n\r");
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
	printf("start client_all_primary_srv_discovery");
	client_all_primary_srv_discovery(conn_id, client_id);
#else
	erpc_ble_init();
#endif
}

void loop()
{
	printf(".");
    delay(2000);
}