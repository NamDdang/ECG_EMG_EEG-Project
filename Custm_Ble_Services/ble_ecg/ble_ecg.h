/*
 * ble_ecg.h file for the ecg ble service.
 *
 * Author : Abdelali Boussetta  github/rmptxf
 * Date : 01/30/2019
 *
 */

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "sdk_common.h"
#include "app_error.h"

#define BLE_ECG_BLE_OBSERVER_PRIO  2

/**@brief   Macro for defining a ble_ecg instance.
 *
 * @param   _name   Name of the instance.
 */
#define BLE_ECG_DEF(_name)                                                            \
static ble_ecg_t _name;                                                               \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                   \
                     BLE_ECG_BLE_OBSERVER_PRIO,                                       \
                     ble_ecg_on_ble_evt, &_name)



#define ECG_SERVICE_UUID_BASE      { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, \
                                     0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }

#define ECG_SERVICE_UUID           0x3000

#define TIMESTAMP_CHAR_UUID        0x3001
#define ECG_CHANNEL1_CHAR_UUID     0x3002 
#define ECG_CHANNEL2_CHAR_UUID     0x3003	
#define ECG_CHANNEL3_CHAR_UUID     0x3004
#define ECG_CHANNEL4_CHAR_UUID     0x3005

/**@brief ECG service event types.
 *
 */
typedef enum
{   
    BLE_ECG_TIMESTAMP_CHAR_NOTIFICATIONS_ENABLED,
    BLE_ECG_TIMESTAMP_CHAR_NOTIFICATIONS_DISABLED,

    BLE_ECG_CHANNEL1_CHAR_NOTIFICATIONS_ENABLED,
    BLE_ECG_CHANNEL1_CHAR_NOTIFICATIONS_DISABLED,

    BLE_ECG_CHANNEL2_CHAR_NOTIFICATIONS_ENABLED,
    BLE_ECG_CHANNEL2_CHAR_NOTIFICATIONS_DISABLED,

    BLE_ECG_CHANNEL3_CHAR_NOTIFICATIONS_ENABLED,
    BLE_ECG_CHANNEL3_CHAR_NOTIFICATIONS_DISABLED,

    BLE_ECG_CHANNEL4_CHAR_NOTIFICATIONS_ENABLED,
    BLE_ECG_CHANNEL4_CHAR_NOTIFICATIONS_DISABLED,

} ble_ecg_evt_type_t;


typedef struct
{
   ble_ecg_evt_type_t evt_type;                     
} ble_ecg_evt_t;


/**@brief Forward declaration of the ble_ecg_t type. */
typedef struct ble_ecg_s ble_ecg_t;


/**@brief ECG service event handler type. */
typedef void (*ble_ecg_evt_handler_t) (ble_ecg_t * p_ecg, ble_ecg_evt_t * p_evt);


/**@brief ECG Service init structure. 
 * This contains all options needed for the initialization of the service.
 *
 */
typedef struct
{
    ble_ecg_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the ECG Service. */  
   
    ble_srv_cccd_security_mode_t  ecg_channel1_char_attr_md;    /**< Used to set the security mode of the cccd for the ecg channel 1 char. */
    ble_srv_cccd_security_mode_t  ecg_channel2_char_attr_md;    /**< Used to set the security mode of the cccd for the ecg channel 2 char. */
    ble_srv_cccd_security_mode_t  ecg_channel3_char_attr_md;    /**< Used to set the security mode of the cccd for the ecg channel 3 char. */
    ble_srv_cccd_security_mode_t  ecg_channel4_char_attr_md;    /**< Used to set the security mode of the cccd for the ecg channel 4 char. */

} ble_ecg_init_t;


/**@brief ECG Service structure.
 *        This contains various status information for the service.
 */

struct ble_ecg_s
{
    ble_ecg_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the ECG Service. */
    uint16_t                      service_handle;                 /**< Handle of ECG Service (as provided by the BLE stack). */
   
    ble_gatts_char_handles_t      timestamp_char_handles;       /**< Handles related to the timestamp characteristic. */
    ble_gatts_char_handles_t      ecg_channel1_char_handles;    /**< Handles related to the ecg channel 1 characteristic. */
    ble_gatts_char_handles_t      ecg_channel2_char_handles;    /**< Handles related to the ecg channel 2 characteristic. */
    ble_gatts_char_handles_t      ecg_channel3_char_handles;    /**< Handles related to the ecg channel 3 characteristic. */
    ble_gatts_char_handles_t      ecg_channel4_char_handles;    /**< Handles related to the ecg channel 4 characteristic. */
      
    uint16_t                      conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                       uuid_type;                      /**< Holds the service uuid type. */
};


/**@brief Function for handling the ECG service ble events.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_ecg_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);


/**@brief Function for initializing the ECG ble service.
 *
 * @param[in]   p_ecg       ECG service structure.
 * @param[in]   p_ecg_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ecg_init(ble_ecg_t * p_ecg, const ble_ecg_init_t * p_ecg_init);


/**@brief Function for updating the ecg channel 1 value on the ecg channel 1 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel1_value  ECG channel 1 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel1_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel1_value, uint16_t conn_handle);

/**@brief Function for updating the ecg channel 2 value on the ecg channel 2 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel2_value  ECG channel 2 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel2_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel2_value, uint16_t conn_handle);

/**@brief Function for updating the ecg channel 3 value on the ecg channel 3 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel3_value  ECG channel 3 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel3_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel3_value, uint16_t conn_handle);

/**@brief Function for updating the ecg channel 4 value on the ecg channel 4 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel4_value  ECG channel 4 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel4_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel4_value, uint16_t conn_handle);


/**@brief Function for updating the timestamp on the timestamp ble characteristic.
 *
 * @param[in]   p_ecg             ECG service structure.
 * @param[in]   p_timestamp       Timestamp.
 * @param[in]   conn_handle       Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ecg_timestamp_update(ble_ecg_t * p_ecg, uint8_t  * p_timestamp, uint16_t conn_handle);
