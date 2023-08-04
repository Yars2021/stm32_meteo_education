#ifndef _PRESSURE_H_
#define _PRESSURE_H_

typedef struct {
    SPI_HandleTypeDef *spi;
    uint8_t raw_data[3];
    float pressure;
} pressure_sensor_t;

void init_pressure(pressure_sensor_t *pressure_sensor);
HAL_StatusTypeDef get_pressure(pressure_sensor_t *pressure_sensor);
     
#endif /* _PRESSURE_H_ */