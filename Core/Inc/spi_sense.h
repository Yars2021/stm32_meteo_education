#ifndef _SPI_SENSE_H_
#define _SPI_SENSE_H_

typedef struct{
    struct {
        GPIO_TypeDef *CS_GPIOx;
        uint16_t CS_GPIO_Pin;
        SPI_HandleTypeDef *spi;
    } Interface;
    uint8_t raw_data[2];
    float temp;
}temputer_sensor;

void init_temp(temputer_sensor *sen, GPIO_TypeDef *CS_GPIOx, uint16_t CS_GPIO_Pin, SPI_HandleTypeDef *spi);
void get_temp(temputer_sensor *sen);

#endif