#ifndef _SPI_SENSE_H_
#define _SPI_SENSE_H_



typedef struct{
    SPI_HandleTypeDef *spi;
    uint8_t raw_data[2];
    float temp;
}temputer_sensor;

void init_temp(temputer_sensor *sen, size_t id);
void get_temp(temputer_sensor *sen);

#endif