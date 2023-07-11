#ifndef HUST_BLE_H__
#define HUST_BLE_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// doi thi phai update code
#define ECG_DATA_LENGTH 3
#define IMU_DATA_LENGTH 2
#define ECG_CHANNEL 4
#define IMU_CHANNEL 3

// doi duoc ma khong can update code
#define ECG_SAMPLE_ECG_SENSOR_TYPE 19  // number of ecg samples in ble packet when sensor_type = ECG_SENSOR_TYPE
#define ECG_SAMPLE_IMU_SENSOR_TYPE 0
#define ECG_SAMPLE_ALL_SENSOR_TYPE 3
#define IMU_SAMPLE_ECG_SENSOR_TYPE 0
#define IMU_SAMPLE_IMU_SENSOR_TYPE 4
#define IMU_SAMPLE_ALL_SENSOR_TYPE 2

typedef struct
{   
    uint8_t ecg_sample;
    uint8_t imu_sample;
} sample_transfer_t;

typedef enum
{   
    ECG_SENSOR_TYPE = 2,
    IMU_SENSOR_TYPE,
    ALL_SENSOR_TYPE
} sensor_type_t;

typedef struct
{
    uint8_t byte[8];
} timestamp_t;
typedef struct
{
    uint8_t byte[3];
} ecg_sample_data_t;
typedef struct
{
    uint8_t byte[2];
} imu_sample_data_t;
typedef struct 
{
    ecg_sample_data_t ecg_channel1, ecg_channel2, ecg_channel3, ecg_channel4;
} ecg_data_t;
typedef struct 
{
    imu_sample_data_t imu_channel1, imu_channel2, imu_channel3;
} imu_data_t;

typedef struct 
{
    timestamp_t timestamp;
    sensor_type_t sensor_type;
    uint8_t data_size;
    uint8_t count_packet;
    ecg_data_t *ecg_data;
    imu_data_t *imu_data;
} ble_packet_t;

// ham update so sample can truyen theo sensor_type
sample_transfer_t set_sample_transfer(ble_packet_t ble_packet_m);

void convert_data_to_ble_packet(ble_packet_t ble_packet_m, uint8_t ** ble_packet);

// ham in ra tung byte cua ble packet
void print_ble_packet_data(uint8_t **ble_packet, int ble_packet_size);



#endif // HUST_BLE_H__