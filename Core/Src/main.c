#define MAIN

#include "include.h"

int main (void)
{
    initLowLevel();

    temputer_sensor sen;

    init_temp(&sen, 0);
    while (1)
    {
        get_temp(&sen);
        HAL_Delay(1000);
    }
}
