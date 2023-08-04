#define MAIN

#include "include.h"

pressure_sensor_t pressure;

int main (void)
{
    initLowLevel();

    init_pressure(&pressure);

    while (1)
    {
        get_pressure(&pressure);
    }
}
