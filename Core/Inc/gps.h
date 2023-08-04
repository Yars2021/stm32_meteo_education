#ifndef _GPS_H_
#define _GPS_H_

#define UART2_TX_LENGTH 128
#define UART2_RX_LENGTH 128
#define GPS_RAW_DATA_SIZE 256

typedef struct {
	uint16_t year;
	uint8_t yearBytes[2];
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t fixType;

	int32_t lon;
	uint8_t lonBytes[4];
	int32_t lat;
	uint8_t latBytes[4];
	float fLon;
	float fLat;

	int32_t height;
	int32_t hMSL;
	uint8_t hMSLBytes[4];

    uint32_t hAcc;
    uint32_t vAcc;
} GNSS_DataTypeDef;

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t raw_data[GPS_RAW_DATA_SIZE];
    GNSS_DataTypeDef pvtdata;
} gps_sensor_t;

void init_gps(gps_sensor_t *gps_sensor);
HAL_StatusTypeDef get_pvtdata(gps_sensor_t *gps_sensor);

static uint8_t configUBX[] = {0xB5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xD0,0x08,0x00,0x00,0x80,0x25,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x9A,0x79};

static uint8_t setNMEA410[] = {0xB5,0x62,0x06,0x17,0x14,0x00,0x00,0x41,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,0x57};

static uint8_t setGNSS[] = {0xB5,0x62,0x06,0x3E,0x24,0x00,0x00,0x00,0x20,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x01,0x02,0x00,0x00,0x00,0x01,0x00,0x01,0x01,0x06,0x00,0x00,0x00,0x01,0x00,0x01,0x01,0xA1,0x92};

static uint8_t getPVTData[] = {0xB5,0x62,0x01,0x07,0x00,0x00,0x08,0x19};

static uint8_t setPortableMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x00,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3C};

static uint8_t setStationaryMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x02,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80};

static uint8_t setPedestrianMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x03,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xA2};

static uint8_t setAutomotiveMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x04,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x82,0xC4};

static uint8_t setAirbone1GMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x06,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x08};

static uint8_t setAirbone2GMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x07,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x85,0x2A};

static uint8_t setAirbone4GMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x08,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x86,0x4C};

static uint8_t setWirstMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x09,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x6E};

static uint8_t setBikeMode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x0A,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x90};

#endif /* _GPS_H_ */