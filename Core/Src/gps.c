#include "include.h"

void init_gps(gps_sensor_t *gps_sensor, UART_HandleTypeDef *huart)
{
    if (!gps_sensor) return;

    gps_sensor->Interface.huart = huart;

    for (size_t i = 0; i < GPS_RAW_DATA_SIZE; i++)
        gps_sensor->raw_data[i] = 0;

    gps_sensor->pvtdata.year = 0;
    gps_sensor->pvtdata.yearBytes[0] = 0;
    gps_sensor->pvtdata.yearBytes[1] = 0;
    gps_sensor->pvtdata.month = 0;
    gps_sensor->pvtdata.day = 0;
    gps_sensor->pvtdata.hour = 0;
    gps_sensor->pvtdata.min = 0;
    gps_sensor->pvtdata.sec = 0;
    gps_sensor->pvtdata.fixType = 0;

    gps_sensor->pvtdata.lon = 0;
    gps_sensor->pvtdata.lonBytes[0] = 0;
    gps_sensor->pvtdata.lonBytes[1] = 0;
    gps_sensor->pvtdata.lonBytes[2] = 0;
    gps_sensor->pvtdata.lonBytes[3] = 0;
    gps_sensor->pvtdata.lat = 0;
    gps_sensor->pvtdata.latBytes[0] = 0;
    gps_sensor->pvtdata.latBytes[1] = 0;
    gps_sensor->pvtdata.latBytes[2] = 0;
    gps_sensor->pvtdata.latBytes[3] = 0;
    gps_sensor->pvtdata.fLon = 0.0;
    gps_sensor->pvtdata.fLat = 0.0;

    gps_sensor->pvtdata.height = 0;
    gps_sensor->pvtdata.hMSL = 0;
    gps_sensor->pvtdata.hMSLBytes[0] = 0;
    gps_sensor->pvtdata.hMSLBytes[1] = 0;
    gps_sensor->pvtdata.hMSLBytes[2] = 0;
    gps_sensor->pvtdata.hMSLBytes[3] = 0;

    gps_sensor->pvtdata.hAcc = 0;
    gps_sensor->pvtdata.vAcc = 0;
}

HAL_StatusTypeDef get_pvtdata(gps_sensor_t *gps_sensor)
{
    if (!gps_sensor) return HAL_ERROR;

    HAL_StatusTypeDef hal_status = HAL_OK;

    hal_status |= HAL_UART_Transmit(gps_sensor->Interface.huart, getPVTData, sizeof(getPVTData), 1000);
    HAL_UART_Receive(gps_sensor->Interface.huart, gps_sensor->raw_data, 101, 1000);
    HAL_Delay(1000);

    gps_sensor->pvtdata.yearBytes[0] = gps_sensor->raw_data[10];
	gps_sensor->pvtdata.yearBytes[1] = gps_sensor->raw_data[11];
	gps_sensor->pvtdata.year = gps_sensor->pvtdata.yearBytes[1] << 8 | gps_sensor->pvtdata.yearBytes[0];
	gps_sensor->pvtdata.month = gps_sensor->raw_data[12];
	gps_sensor->pvtdata.day = gps_sensor->raw_data[13];
	gps_sensor->pvtdata.hour = gps_sensor->raw_data[14];
	gps_sensor->pvtdata.min = gps_sensor->raw_data[15];
	gps_sensor->pvtdata.sec = gps_sensor->raw_data[16];
	gps_sensor->pvtdata.fixType = gps_sensor->raw_data[26];

    for (size_t i = 0; i < 4; i++) {
        gps_sensor->pvtdata.lonBytes[i] = gps_sensor->raw_data[30 + i];
    }

	gps_sensor->pvtdata.lon =   gps_sensor->pvtdata.lonBytes[3] << 24 | 
                                gps_sensor->pvtdata.lonBytes[2] << 16 | 
                                gps_sensor->pvtdata.lonBytes[1] << 8 | 
                                gps_sensor->pvtdata.lonBytes[0];

	gps_sensor->pvtdata.fLon = gps_sensor->pvtdata.lon / 10000000.0;

    for (size_t i = 0; i < 4; i++) {
        gps_sensor->pvtdata.latBytes[i] = gps_sensor->raw_data[34 + i];
    }

	gps_sensor->pvtdata.lat =   gps_sensor->pvtdata.latBytes[3] << 24 | 
                                gps_sensor->pvtdata.latBytes[2] << 16 | 
                                gps_sensor->pvtdata.latBytes[1] << 8 | 
                                gps_sensor->pvtdata.latBytes[0];

	gps_sensor->pvtdata.fLat = gps_sensor->pvtdata.lat / 10000000.0;

    gps_sensor->pvtdata.height =    gps_sensor->raw_data[41] << 24 | 
                                    gps_sensor->raw_data[40] << 16 | 
                                    gps_sensor->raw_data[39] << 8 | 
                                    gps_sensor->raw_data[38];

    for (size_t i = 0; i < 4; i++) {
        gps_sensor->pvtdata.hMSLBytes[i] = gps_sensor->raw_data[42 + i];
    }

	gps_sensor->pvtdata.hMSL =  gps_sensor->pvtdata.hMSLBytes[3] << 24 | 
                                gps_sensor->pvtdata.hMSLBytes[2] << 16 | 
                                gps_sensor->pvtdata.hMSLBytes[1] << 8 | 
                                gps_sensor->pvtdata.hMSLBytes[0];

    gps_sensor->pvtdata.hAcc = gps_sensor->raw_data[49] << 24 | gps_sensor->raw_data[48] << 16 | gps_sensor->raw_data[47] << 8 | gps_sensor->raw_data[46];
    gps_sensor->pvtdata.vAcc = gps_sensor->raw_data[53] << 24 | gps_sensor->raw_data[52] << 16 | gps_sensor->raw_data[51] << 8 | gps_sensor->raw_data[50];

    return hal_status;
}