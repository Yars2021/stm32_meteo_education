#define MAIN

#include "include.h"

gps_sensor_t gps;

int main (void)
{
    initLowLevel();

    init_gps(&gps);

    while (1)
    {
        get_pvtdata(&gps);
    }
}
