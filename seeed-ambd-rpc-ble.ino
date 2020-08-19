#include "elog.h"
#include "ble_task.h"
#include "ble_callback.h"
#include "ble_common.h"
#include "ble_main.h"
#include "gap_scan.h"


/*init the easylogger moudle*/
void app_elog_init(void)
{

	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL  | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL  | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL  | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL  | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL  | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);

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

void setup()
{
    app_elog_init();
	erpc_ble_init();

	// ble_init();
	// le_gap_init(3);
	// uint8_t scan_mode = GAP_SCAN_MODE_ACTIVE;
    // uint16_t scan_interval = DEFAULT_SCAN_INTERVAL;
    // uint16_t scan_window = DEFAULT_SCAN_WINDOW;
    // uint8_t scan_filter_policy = GAP_SCAN_FILTER_ANY;
    // uint8_t scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;

	//     /* Set scan parameters */
    // le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(scan_mode), &scan_mode);
    // le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(scan_interval), &scan_interval);
    // le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(scan_window), &scan_window);
    // le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
    //                   &scan_filter_policy);
    // le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
    //                   &scan_filter_duplicate);

	// le_register_app_cb(ble_gap_callback);
	
	// ble_start();

}	

void loop()
{
	delay(1000);
    // // //log_v("hello world");
    // // delay(2000);
	// delay(5000);
	// log_i("scan start");
	// le_scan_start();
	// delay(10000);
	// log_i("scan stop");
	// le_scan_stop();
	
}