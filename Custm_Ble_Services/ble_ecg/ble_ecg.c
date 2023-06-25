/*
 * ble_ecg.c file for the ecg ble service
 *
 * Author : Abdelali Boussetta  github/rmptxf
 * Date : 01/30/2019
 *
 */

#include "ble_ecg.h"


/**@brief Function for handling the Write event.
 *
 * @param[in]   p_ecg       ECG service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_ecg_t * p_ecg, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    ble_ecg_evt_t                 evt;

    // writing to the ecg channel 1 characteristic (cccd) "client characteristic configuration descriptor"
   if (p_evt_write->handle == p_ecg->ecg_channel1_char_handles.cccd_handle)
   {
      if (ble_srv_is_notification_enabled(p_evt_write->data))
      {
          evt.evt_type = BLE_ECG_CHANNEL1_CHAR_NOTIFICATIONS_ENABLED;
      }
      else
      {
          evt.evt_type = BLE_ECG_CHANNEL1_CHAR_NOTIFICATIONS_DISABLED;
      }

      p_ecg->evt_handler(p_ecg, &evt);
   }

    // writing to the ecg channel 2 characteristic (cccd)
   else if (p_evt_write->handle == p_ecg->ecg_channel2_char_handles.cccd_handle)
   {
      if (ble_srv_is_notification_enabled(p_evt_write->data))
      {
          evt.evt_type = BLE_ECG_CHANNEL2_CHAR_NOTIFICATIONS_ENABLED;
      }
      else
      {
          evt.evt_type = BLE_ECG_CHANNEL2_CHAR_NOTIFICATIONS_DISABLED;
      }

      p_ecg->evt_handler(p_ecg, &evt);
   }

    // writing to the ecg channel 3 characteristic (cccd)
   else if (p_evt_write->handle == p_ecg->ecg_channel3_char_handles.cccd_handle)
   {
      if (ble_srv_is_notification_enabled(p_evt_write->data))
      {
          evt.evt_type = BLE_ECG_CHANNEL3_CHAR_NOTIFICATIONS_ENABLED;
      }
      else
      {
          evt.evt_type = BLE_ECG_CHANNEL3_CHAR_NOTIFICATIONS_DISABLED;
      }

      p_ecg->evt_handler(p_ecg, &evt);
   }

    // writing to the ecg channel 4 characteristic (cccd)
   else if (p_evt_write->handle == p_ecg->ecg_channel4_char_handles.cccd_handle)
   {
      if (ble_srv_is_notification_enabled(p_evt_write->data))
      {
          evt.evt_type = BLE_ECG_CHANNEL4_CHAR_NOTIFICATIONS_ENABLED;
      }
      else
      {
          evt.evt_type = BLE_ECG_CHANNEL4_CHAR_NOTIFICATIONS_DISABLED;
      }

      p_ecg->evt_handler(p_ecg, &evt);
   }
}


/**@brief Function for handling the ECG servie ble events.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_ecg_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_ecg_t * p_ecg = (ble_ecg_t *) p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_ecg, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for initializing the ECG ble service.
 *
 * @param[in]   p_ecg       ECG service structure.
 * @param[in]   p_ecg_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_init(ble_ecg_t * p_ecg, const ble_ecg_init_t * p_ecg_init)
{

    uint32_t                  err_code;
    ble_uuid_t                ble_uuid;
    ble_add_char_params_t     add_char_params;

/* Adding the service */

    // Initialize service structure.
    p_ecg->evt_handler               = p_ecg_init->evt_handler;
    p_ecg->conn_handle               = BLE_CONN_HANDLE_INVALID;

    // Add the ECG ble Service UUID
    ble_uuid128_t base_uuid =  ECG_SERVICE_UUID_BASE;
    err_code =  sd_ble_uuid_vs_add(&base_uuid, &p_ecg->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    ble_uuid.type = p_ecg->uuid_type;
    ble_uuid.uuid = ECG_SERVICE_UUID;

    // Add the service to the database
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ecg->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

/* Adding the service characteristics */
    // Add the timestamp characteristic.

    uint8_t timestamp_char_init_value [8] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = TIMESTAMP_CHAR_UUID;
    add_char_params.uuid_type         = p_ecg->uuid_type;

    add_char_params.init_len          = 8;
    add_char_params.max_len           = 8;
    add_char_params.p_init_value      = timestamp_char_init_value;

    add_char_params.char_props.read   = 1;
    //add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ecg->service_handle,
                                  &add_char_params,
                                  &p_ecg->timestamp_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add the ecg channel 1 characteristic.

    uint8_t ecg_channel1_char_init_value [3] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = ECG_CHANNEL1_CHAR_UUID;
    add_char_params.uuid_type         = p_ecg->uuid_type;

    add_char_params.init_len          = 3;
    add_char_params.max_len           = 3;
    add_char_params.p_init_value      = ecg_channel1_char_init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ecg->service_handle,
                                  &add_char_params,
                                  &p_ecg->ecg_channel1_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add the ecg channel 2 characteristic.

    uint8_t ecg_channel2_char_init_value [3] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = ECG_CHANNEL2_CHAR_UUID;
    add_char_params.uuid_type         = p_ecg->uuid_type;

    add_char_params.init_len          = 3;// (in bytes)
    add_char_params.max_len           = 3;
    add_char_params.p_init_value      = ecg_channel2_char_init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ecg->service_handle,
                                  &add_char_params,
                                  &p_ecg->ecg_channel2_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add the ecg channel 3 characteristic.

    uint8_t ecg_channel3_char_init_value [3] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = ECG_CHANNEL3_CHAR_UUID;
    add_char_params.uuid_type         = p_ecg->uuid_type;

    add_char_params.init_len          = 3;// (in bytes)
    add_char_params.max_len           = 3;
    add_char_params.p_init_value      = ecg_channel3_char_init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ecg->service_handle,
                                  &add_char_params,
                                  &p_ecg->ecg_channel3_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add the ecg channel 4 characteristic.

    uint8_t ecg_channel4_char_init_value [3] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = ECG_CHANNEL4_CHAR_UUID;
    add_char_params.uuid_type         = p_ecg->uuid_type;

    add_char_params.init_len          = 3;// (in bytes)
    add_char_params.max_len           = 3;
    add_char_params.p_init_value      = ecg_channel4_char_init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ecg->service_handle,
                                  &add_char_params,
                                  &p_ecg->ecg_channel4_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

/**@brief Function for updating the ecg channel 1 value on the ecg channel 1 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel1_value  ECG channel 1 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel1_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel1_value, uint16_t conn_handle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(ecg_channel1_value);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_ecg->ecg_channel1_char_handles.value_handle;
    params.p_data = ecg_channel1_value;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}

/**@brief Function for updating the ecg channel 2 value on the ecg channel 2 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel2_value  ECG channel 2 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel2_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel2_value, uint16_t conn_handle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(ecg_channel2_value);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_ecg->ecg_channel2_char_handles.value_handle;
    params.p_data = ecg_channel2_value;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}

/**@brief Function for updating the ecg channel 3 value on the ecg channel 3 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel3_value  ECG channel 3 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel3_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel3_value, uint16_t conn_handle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(ecg_channel3_value);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_ecg->ecg_channel3_char_handles.value_handle;
    params.p_data = ecg_channel3_value;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}

/**@brief Function for updating the ecg channel 4 value on the ecg channel 4 ble characteristic.
 *
 * @param[in]   p_ecg               ECG service structure.
 * @param[in]   ecg_channel4_value  ECG channel 4 value.
 * @param[in]   conn_handle         Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_channel4_value_update(ble_ecg_t * p_ecg, uint8_t * ecg_channel4_value, uint16_t conn_handle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(ecg_channel4_value);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_ecg->ecg_channel4_char_handles.value_handle;
    params.p_data = ecg_channel4_value;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}

/**@brief Function for updating the timestamp on the timestamp ble characteristic.
 *
 * @param[in]   p_ecg           ECG service structure.
 * @param[in]   ecg_timestamp   Timestamp.
 * @param[in]   conn_handle     Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ecg_timestamp_update(ble_ecg_t * p_ecg, uint8_t * ecg_timestamp, uint16_t conn_handle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(ecg_timestamp);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_ecg->timestamp_char_handles.value_handle;
    params.p_data = ecg_timestamp;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}