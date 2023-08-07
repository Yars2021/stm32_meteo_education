#define MAIN

#include "include.h"

extern UART_HandleTypeDef huart2;

gps_sensor_t gps;

int main (void)
{
    initLowLevel();

    init_gps(&gps, &huart2);

    while (1)
    {
        get_pvtdata(&gps);
    }
}
