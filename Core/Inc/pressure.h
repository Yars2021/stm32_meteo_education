#ifndef _PRESSURE_H_
#define _PRESSURE_H_

typedef struct {
    struct {
        GPIO_TypeDef *GPIOx;
        uint16_t GPIO_Pin;
        SPI_HandleTypeDef *spi;
    } Interface;
    uint8_t raw_data[3];
    float pressure;
} pressure_sensor_t;

void init_pressure(pressure_sensor_t *pressure_sensor, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, SPI_HandleTypeDef *spi);
HAL_StatusTypeDef get_pressure(pressure_sensor_t *pressure_sensor);
     
#endif /* _PRESSURE_H_ */