#define MAIN

#include "include.h"

extern SPI_HandleTypeDef spi_1;

uint8_t message;

int main (void)
{
    initLowLevel();

    char * buf = "Hello";

    transiver trans;

    init_trans(&trans, GPIOA, GPIO_PIN_4, &spi_1);
    trans.output = buf;
    trans.output_size = sizeof(buf);
    
    while (1)
    {
        transmite(&trans);
        HAL_Delay(1000);
    }
}
