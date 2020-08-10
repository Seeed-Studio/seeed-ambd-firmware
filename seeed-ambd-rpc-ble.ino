#include "elog.h"
#include "rpc_ble_task.h"
#include "rpc_ble_callback.h"
#include "rpc_ble_flags.h"


/*init the easylogger moudle*/
void app_elog_init(void)
{

	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL  | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL  | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL  | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);

	/* allow color */
	elog_set_text_color_enabled(true);
	
	/* start EasyLogger */
	elog_start();
}

void setup()
{
    app_elog_init();
}	

void loop()
{
    //log_v("hello world");
    delay(2000);
}