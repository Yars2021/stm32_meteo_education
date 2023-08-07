#define MAIN

#include "include.h"

extern SPI_HandleTypeDef spi_2;

pressure_sensor_t pressure;

int main (void)
{
    initLowLevel();

    init_pressure(&pressure, GPIOB, GPIO_PIN_11, &spi_2);

    while (1)
    {
        get_pressure(&pressure);
    }
}
