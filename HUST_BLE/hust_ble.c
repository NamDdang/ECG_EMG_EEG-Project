#include "hust_ble.h"
#include "nrf_log.h"

// ham update so sample can truyen theo sensor_type
sample_transfer_t set_sample_transfer(ble_packet_t ble_packet_m)
{
    sample_transfer_t sample_transfer_m;
    switch(ble_packet_m.sensor_type)
    {
        case ECG_SENSOR_TYPE:
            sample_transfer_m.ecg_sample = ECG_SAMPLE_ECG_SENSOR_TYPE;
            sample_transfer_m.imu_sample = IMU_SAMPLE_ECG_SENSOR_TYPE;
            break;
        case IMU_SENSOR_TYPE:
            sample_transfer_m.ecg_sample = ECG_SAMPLE_IMU_SENSOR_TYPE;
            sample_transfer_m.imu_sample = IMU_SAMPLE_IMU_SENSOR_TYPE;
            break;
        case ALL_SENSOR_TYPE:
            sample_transfer_m.ecg_sample = ECG_SAMPLE_ALL_SENSOR_TYPE;
            sample_transfer_m.imu_sample = IMU_SAMPLE_ALL_SENSOR_TYPE;
            break;
    }
    return sample_transfer_m;
}

void convert_data_to_ble_packet(ble_packet_t ble_packet_m, uint8_t ** ble_packet)
{
    int count_ble_data = 0;
    int ble_packet_size = 8 + 1 + 1 + ble_packet_m.data_size;     // sizeof(timestamp) + sizeof(sensor_type) + sizeof(data_size) + sizeof(data)
    *ble_packet = malloc(sizeof(uint8_t)*ble_packet_size);
    for(int i = 0; i < 8; i++)
    {
            *(*ble_packet + i) = ble_packet_m.timestamp.byte[i];
    }
    count_ble_data+=8;
    *(*ble_packet + count_ble_data) = ble_packet_m.sensor_type;
    count_ble_data++;
    *(*ble_packet + count_ble_data) = ble_packet_m.data_size;
    count_ble_data++;
    *(*ble_packet + count_ble_data) = ble_packet_m.count_packet;
    count_ble_data++;
   
    
    sample_transfer_t sample_transfer_m;
    sample_transfer_m = set_sample_transfer(ble_packet_m);
    for(int j = 0; j < sample_transfer_m.ecg_sample; j++)				//update cho tung sample
    {
        for(int i = count_ble_data; i < count_ble_data + ECG_DATA_LENGTH; i++)		//update cho tung channel cua 1 lan lay sample
        {
                *(*ble_packet + i) = (ble_packet_m.ecg_data+j)->ecg_channel1.byte[i - count_ble_data];
                *(*ble_packet + i + ECG_DATA_LENGTH) = (ble_packet_m.ecg_data+j)->ecg_channel2.byte[i - count_ble_data];
                *(*ble_packet + i + 2 * ECG_DATA_LENGTH) = (ble_packet_m.ecg_data+j)->ecg_channel3.byte[i - count_ble_data];
                *(*ble_packet + i + 3 * ECG_DATA_LENGTH) = (ble_packet_m.ecg_data+j)->ecg_channel4.byte[i - count_ble_data];
        }
        count_ble_data+=(ECG_DATA_LENGTH*ECG_CHANNEL);
    }
    for(int j = 0; j < sample_transfer_m.imu_sample; j++)				//update cho tung sample
    {
        for(int i = count_ble_data; i < count_ble_data + IMU_DATA_LENGTH; i++)
        {
                *(*ble_packet + i) = (ble_packet_m.imu_data+j)->imu_channel1.byte[i - count_ble_data];
                *(*ble_packet + i + IMU_DATA_LENGTH) = (ble_packet_m.imu_data+j)->imu_channel2.byte[i - count_ble_data];
                *(*ble_packet + i + 2*IMU_DATA_LENGTH) = (ble_packet_m.imu_data+j)->imu_channel3.byte[i - count_ble_data];
        }
        count_ble_data+=(IMU_DATA_LENGTH*IMU_CHANNEL);
    }
}
// ham in ra tung byte cua ble packet
void print_ble_packet_data(uint8_t **ble_packet, int ble_packet_size)
{
    for(int i = 0; i < ble_packet_size; i++)
    {
            printf("%d ", *(*ble_packet + i));
    }
    printf("\n");
}