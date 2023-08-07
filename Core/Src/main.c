#define MAIN

#include "include.h"

extern TIM_HandleTypeDef htim2, htim3;

humidity_sensor_t humidity;

int main(void)
{
    initLowLevel();

    init_humidity(&humidity, GPIOA, GPIO_PIN_1, &htim2, &htim3);
    switch_to_hum_sensor(&humidity);

    while (1)
    {
        get_humidity(&humidity);
        HAL_Delay(1000); // Humidity is measured once per second
    }   
}