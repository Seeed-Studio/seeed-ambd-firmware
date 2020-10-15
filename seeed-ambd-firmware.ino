#include "elog.h"
#include "ble_task.h"
#include "ble_callback.h"
#include "ble_common.h"
#include "ble_main.h"
#include "gcs_client.h"
#include "gap_config.h"
#include "bas_client.h"
#include "gatt.h"

#include "rpc_system_header.h"
#include "rpc_wifi_api.h"
#include "wifi_main.h"
#include "wifi_callback.h"
#include "wifi_tcpip_adapter.h"

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
#define MAC_ARG(x) ((u8 *)(x))[0], ((u8 *)(x))[1], ((u8 *)(x))[2], ((u8 *)(x))[3], ((u8 *)(x))[4], ((u8 *)(x))[5]
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
static void print_scan_result(rtw_scan_result_t *record)
{

	printf("%s\t ", (record->bss_type == RTW_BSS_TYPE_ADHOC) ? "Adhoc" : "Infra");
	printf(MAC_FMT, MAC_ARG(record->BSSID.octet));
	printf(" %d\t ", record->signal_strength);
	printf(" %d\t  ", record->channel);
	printf(" %d\t  ", record->wps_type);
	printf("%s\t\t ", (record->security == RTW_SECURITY_OPEN) ? "Open" : (record->security == RTW_SECURITY_WEP_PSK) ? "WEP" : (record->security == RTW_SECURITY_WPA_TKIP_PSK) ? "WPA TKIP" : (record->security == RTW_SECURITY_WPA_AES_PSK) ? "WPA AES" : (record->security == RTW_SECURITY_WPA2_AES_PSK) ? "WPA2 AES" : (record->security == RTW_SECURITY_WPA2_TKIP_PSK) ? "WPA2 TKIP" : (record->security == RTW_SECURITY_WPA2_MIXED_PSK) ? "WPA2 Mixed" : (record->security == RTW_SECURITY_WPA_WPA2_MIXED) ? "WPA/WPA2 AES" : "Unknown");

	printf(" %s ", record->SSID.val);
	printf("\r\n");
}

static rtw_result_t app_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result)
{
	static int ApNum = 0;

	if (malloced_scan_result->scan_complete != RTW_TRUE)
	{
		rtw_scan_result_t *record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
		printf("%d\t ", ++ApNum);
		print_scan_result(record);
	}
	else
	{
		ApNum = 0;
	}
}
// Set these to your desired credentials.
const char *ssid = "TE_TEST";
const char *password = "123456789";

extern void erpc_system_init();

void setup()
{
	
	app_elog_init();
	wifi_init();
	// delay(1000);

	// // wifi_scan_networks(app_scan_result_handler, NULL);
	// // delay(5000);
	// // rpc_wifi_on(RTW_MODE_STA);
	tcpip_adapter_init();
	binary_t b_ssid;
	binary_t b_password;
	b_ssid.data =  (uint8_t *)ssid;
	b_ssid.dataLength = strlen(ssid)+1;
	b_password.data = (uint8_t *)password;
	b_password.dataLength = strlen(password)+1;
	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
	wifi_off();
    vTaskDelay(20);
	if (wifi_on(RTW_MODE_STA) < 0){
		printf("\n\rERROR: Wifi on STA failed!");
	}
	int ret = rpc_wifi_connect(&b_ssid, &b_password, RTW_SECURITY_WPA2_AES_PSK, -1, NULL);
	if(ret == RTW_ERROR)
	{
		printf("Error!!\n\r");
	}else
	{
		tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
	}
	
    // if (wifi_on(RTW_MODE_AP) < 0){
	// 	printf("\n\rERROR: Wifi on AP failed!");
	// }
	// tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
	// tcpip_adapter_ip_info_t info;
	// info.ip.addr = 16885952;
	// info.gw.addr = 16885952;
	// info.netmask.addr = 16777215;
	// tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info);
	// rpc_wifi_start_ap(&b_ssid, &b_password, RTW_SECURITY_WPA2_AES_PSK, 11);
	
	// tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
	//erpc_system_init();
}

int client_number;
struct
{
	int count;
	rtw_mac_t mac_list[3];
} client_info;
void loop()
{
	delay(2000);
	printf(".");
	static wlan_fast_reconnect_profile_t wifi_info = {0};
	if(wifi_get_reconnect_data(&wifi_info)!=0)
	{
		printf("SSID: %s\n\r", wifi_info.psk_essid);
		printf("PASSWORD: %s\n\r", wifi_info.psk_passphrase);
	}
	// client_info.count = 3;
	// wifi_get_associated_client_list(&client_info, sizeof(client_info));
	// printf("\n\rAssociated Client List:");
	// printf("\n\r==============================");

	// if (client_info.count == 0)
	// 	printf("\n\rClient Num: 0\n\r");
	// else
	// {
	// 	printf("\n\rClient Num: %d", client_info.count);
	// 	for (client_number = 0; client_number < client_info.count; client_number++)
	// 	{
	// 		printf("\n\rClient [%d]:", client_number);
	// 		printf("\n\r\tMAC => " MAC_FMT "",
	// 			   MAC_ARG(client_info.mac_list[client_number].octet));
	// 	}
	// 	printf("\n\r");
	// }
	// rpc_wifi_connect(&b_ssid, &b_password, RTW_SECURITY_WPA2_AES_PSK, 0, NULL);
	// vTaskDelay(30000);
	// printf("I am here\n\r");
	// rpc_wifi_disconnect();
	// vTaskDelay(5000);
}
// extern void erpc_ble_init();

// uint8_t conn_id = 0xff;
// #if DEBUG_LOCAL
// /** @brief  GAP - scan response data (max size = 31 bytes) */
// static const uint8_t scan_rsp_data[] =
// 	{
// 		0x03,				   /* length */
// 		GAP_ADTYPE_APPEARANCE, /* type="Appearance" */
// 		LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
// 		HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
// };

// /** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
// static const uint8_t adv_data[] =
// 	{
// 		/* Flags */
// 		0x02,			  /* length */
// 		GAP_ADTYPE_FLAGS, /* type="Flags" */
// 		GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
// 		/* Service */
// 		0x03, /* length */
// 		GAP_ADTYPE_16BIT_COMPLETE,
// 		LO_WORD(0x180F),
// 		HI_WORD(0x180F),
// 		/* Local name */
// 		0x0F, /* length */
// 		GAP_ADTYPE_LOCAL_NAME_COMPLETE,
// 		'B',
// 		'L',
// 		'E',
// 		'_',
// 		'P',
// 		'E',
// 		'R',
// 		'I',
// 		'P',
// 		'H',
// 		'E',
// 		'R',
// 		'A',
// 		'L',
// };

// char v1_user_descr[] = "V1 read characteristic";
// uint8_t srcv_app_id = 0;
// uint8_t srcv_handle = 0;
// uint8_t char_handle = 0;
// uint8_t desc_handle1 = 0;
// uint8_t desc_handle2 = 0;
// #endif

// void setup()
// {
// 	//app_elog_init();
// #if DEBUG_LOCAL
// 	ble_init();
// 	ble_server_init(BLE_SERVER_MAX_APPS);

// 	le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
// 	le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

// 	ble_service_t srcv;

// 	srcv.uuid_length = UUID_16BIT_SIZE;
// 	uint16_t srcv_uuid = 0x180F;
// 	memcpy(&(srcv.uuid), &srcv_uuid, 2);
// 	srcv.is_primary = true;

// 	srcv_app_id = ble_create_service(srcv);
// 	printf("srcv_app_id: %d\n\r", srcv_app_id);

// 	ble_char_t CHAR;
// 	CHAR.uuid_length = UUID_16BIT_SIZE;
// 	uint16_t CHAR_uuid = 0x2A19;
// 	memcpy(&(CHAR.uuid), &CHAR_uuid, 2);
// 	CHAR.properties = (GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY);
// 	CHAR.permissions = GATT_PERM_READ | GATT_PERM_WRITE;
// 	char_handle = ble_create_char(srcv_app_id, CHAR);
// 	printf("char_handle: %d\n\r", char_handle);
// 	ble_desc_t desc;

// 	desc.flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;
// 	desc.uuid_length = UUID_16BIT_SIZE;
// 	uint16_t desc_uuid = 0x2902;
// 	uint16_t default_vlaue = 0x0000;
// 	memcpy(&(desc.uuid), &desc_uuid, 2);
// 	memcpy(&(desc.uuid[2]), &default_vlaue, 2);
// 	desc.p_value = NULL;
// 	desc.vlaue_length = 2;
// 	desc.permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
// 	desc_handle1 = ble_create_desc(srcv_app_id, char_handle, desc);
// 	printf("desc_handle1: %d\n\r", desc_handle1);

// 	ble_desc_t desc2;
// 	desc2.flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
// 	desc2.uuid_length = UUID_16BIT_SIZE;
// 	uint16_t desc2_uuid = 0x2901;
// 	memcpy(&(desc2.uuid), &desc2_uuid, 2);
// 	desc2.p_value = (uint8_t *)v1_user_descr;
// 	desc2.vlaue_length = (sizeof(v1_user_descr) - 1);
// 	desc2.permissions = (GATT_PERM_READ);
// 	desc_handle2 = ble_create_desc(srcv_app_id, char_handle, desc2);
// 	printf("desc_handle2: %d\n\r", desc_handle2);

// 	srcv_handle = ble_service_start(srcv_app_id);

// 	ble_start();

// 	delay(2000);
// 	le_adv_start();
// #else
// 	erpc_ble_init();
// #endif
// }

// void loop()
// {
// 	delay(2000);
// }