#define MAIN

#include "include.h"

extern SPI_HandleTypeDef spi_2;

int main (void)
{
    initLowLevel();

    temputer_sensor sen;

    init_temp(&sen, GPIOB, GPIO_PIN_12, &spi_2);
    while (1)
    {
        get_temp(&sen);
        HAL_Delay(1000);
    }
}
