#include "include.h"

void init_temp(temputer_sensor *sen, GPIO_TypeDef *CS_GPIOx, uint16_t CS_GPIO_Pin, SPI_HandleTypeDef *spi){
    sen->raw_data[0] = 0;
    sen->raw_data[1] = 0;
    sen->temp = 0;
    sen->Interface.spi = &spi;
    sen->Interface.CS_GPIO_Pin = CS_GPIO_Pin;
    sen->Interface.CS_GPIOx = CS_GPIOx;
}

void get_temp(temputer_sensor *sen){
    char out[2] = {0};
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(sen->spi, out, sen->raw_data, 2, 350);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    uint16_t raw = sen->raw_data[1]>>5 | sen->raw_data[0]<<3;
    sen->temp = raw * 0.25;
    if(sen->temp > 128.0) sen->temp = 128.0 - sen->temp; 
}
