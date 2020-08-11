
#ifdef __cplusplus
extern "C"
{
#endif
#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_drv.h"
#include "trace_app.h"
#include "bte.h"
#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "os_task.h"
#include "os_msg.h"
#include "app_msg.h"
#include "rtk_coex.h"
#include "profile_server.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif

#include "elog.h"
#include "rpc_ble_common.h"
#include "rpc_ble_api.h"
#include "rpc_ble_callback.h"
#include "rpc_ble_task.h"
#include "rpc_ble_utils.h"

/** @brief Default scan interval (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_INTERVAL 0x520
/** @brief Default scan window (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_WINDOW 0x520

void ble_client_init()
{
    rpc_ble_dev_init(2);
    log_v("ble_client_init");
    gap_config_max_le_link_num(3);
    bte_init();
    le_gap_init(3);

    /* Device name and device appearance */
    uint8_t device_name[GAP_DEVICE_NAME_LEN] = "BLE_CENTRAL_CLIENT";
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

    /* Scan parameters */
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
#if F_BT_LE_SMP_OOB_SUPPORT
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    /* register gap message callback */
    le_register_app_cb(rpc_ble_gap_callback);

#if F_BT_LE_5_0_SET_PHY_SUPPORT
    uint8_t phys_prefer = GAP_PHYS_PREFER_ALL;
    uint8_t tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
                             GAP_PHYS_PREFER_CODED_BIT;
    uint8_t rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
                             GAP_PHYS_PREFER_CODED_BIT;
    le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
    le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
    le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);
#endif

    client_init(1);

    rpc_ble_app_task_init();
}

extern void wifi_btcoex_set_bt_on(void);
int ble_client_test_main()
{
    log_v("ble_client_test_main");

    //int bt_stack_already_on = 0;
    //(void) bt_stack_already_on;
    T_GAP_DEV_STATE new_state;

    if (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE)))
    {
        wiFiDrv.wifiDriverInit();
    }

    /*Wait WIFI init complete*/
    while (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE)))
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    //judge BLE central is already on
    le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
    if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
    {
        //bt_stack_already_on = 1;
        log_d("[BLE Central]BT Stack already on");
        return 0;
    }
    else
        ble_client_init();

    bt_coex_init();

    /*Wait BT init complete*/
    do
    {
        vTaskDelay(100 / portTICK_RATE_MS);
        le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
    } while (new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

    /*Start BT WIFI coexistence*/
    wifi_btcoex_set_bt_on();

    le_scan_start();

    return 0;
}