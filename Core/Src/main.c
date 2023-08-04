#define MAIN

#include "include.h"

humidity_sensor_t humidity;

int main(void)
{
    initLowLevel();

    init_humidity(&humidity);
    switch_to_hum_sensor(&humidity);

    while (1)
    {
        get_humidity(&humidity);
        HAL_Delay(1000); // Humidity is measured once per second
    }   
}