#include "elog.h"
#include "ble_task.h"
#include "ble_callback.h"
#include "ble_common.h"
#include "ble_main.h"
#include "gcs_client.h"
#include "gap_config.h"
#include "bas_client.h"
#include "gatt.h"

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
		LO_WORD(0x180F),
		HI_WORD(0x180F),
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

char v1_user_descr[] = "V1 read characteristic";
uint8_t srcv_app_id = 0;
uint8_t srcv_handle = 0;
uint8_t char_handle = 0;
uint8_t desc_handle1 = 0;
uint8_t desc_handle2 = 0;

void setup()
{
	app_elog_init();
#if DEBUG_LOCAL
	ble_init();
	ble_server_init(BLE_SERVER_MAX_APPS);

	le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
	le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

	ble_service_t srcv;

	srcv.uuid_length = UUID_16BIT_SIZE;
	uint16_t srcv_uuid = 0x180F;
	memcpy(&(srcv.uuid), &srcv_uuid, 2);
	srcv.is_primary = true;

	srcv_app_id = ble_create_service(srcv);
	printf("srcv_app_id: %d\n\r", srcv_app_id);

	ble_char_t CHAR;
	CHAR.uuid_length = UUID_16BIT_SIZE;
	uint16_t CHAR_uuid = 0x2A19;
	memcpy(&(CHAR.uuid), &CHAR_uuid, 2);
	CHAR.properties = (GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY);
	CHAR.permissions = GATT_PERM_READ | GATT_PERM_WRITE;
	char_handle = ble_create_char(srcv_app_id, CHAR);
	printf("char_handle: %d\n\r", char_handle);
	ble_desc_t desc;

	desc.flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;
	desc.uuid_length = UUID_16BIT_SIZE;
	uint16_t desc_uuid = 0x2902;
	uint16_t default_vlaue = 0x0000;
	memcpy(&(desc.uuid), &desc_uuid, 2);
	memcpy(&(desc.uuid[2]), &default_vlaue, 2);
	desc.p_value = NULL;
	desc.vlaue_length = 2;
	desc.permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
	desc_handle1 = ble_create_desc(srcv_app_id, char_handle, desc);
	printf("desc_handle1: %d\n\r", desc_handle1);

	ble_desc_t desc2;
	desc2.flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
	desc2.uuid_length = UUID_16BIT_SIZE;
	uint16_t desc2_uuid = 0x2901;
	memcpy(&(desc2.uuid), &desc2_uuid, 2);
	desc2.p_value = (uint8_t *)v1_user_descr;
	desc2.vlaue_length = (sizeof(v1_user_descr) - 1);
	desc2.permissions = (GATT_PERM_READ);
	desc_handle2 = ble_create_desc(srcv_app_id, char_handle, desc2);
	printf("desc_handle2: %d\n\r", desc_handle2);

	srcv_handle = ble_service_start(srcv_app_id);

	ble_start();

	delay(2000);
	le_adv_start();
#else
	erpc_ble_init();
#endif
}

void loop()
{
	printf(".");
	delay(2000);
}