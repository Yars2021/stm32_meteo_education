#define MAIN

#include "include.h"

uint8_t message;

int main (void)
{
    initLowLevel();
    
    while (1)
    {
        message = read_reg(0x42);
        HAL_Delay(1000);
    }
}
