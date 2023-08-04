#include "include.h"

extern spi_2;

void init_temp(temputer_sensor *sen, size_t id){
    sen->raw_data[0] = 0;
    sen->raw_data[1] = 0;
    sen->temp = 0;
    sen->spi = &spi_2;
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
