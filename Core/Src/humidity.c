#include "include.h"

humidity_sensor_t internal_humidity;

void init_humidity(humidity_sensor_t *humidity_sensor, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, TIM_HandleTypeDef *main_tim, TIM_HandleTypeDef *high_lvl_tim)
{
    if (!humidity_sensor) return;

    humidity_sensor->Interface.GPIOx = GPIOx;
    humidity_sensor->Interface.GPIO_Pin = GPIO_Pin;

    humidity_sensor->Interface.main_timer = main_tim;
    humidity_sensor->Interface.high_lvl_timer = high_lvl_tim;
    humidity_sensor->PWM_signal_state = 0;
    internal_humidity.high_lvl_cnt = 0;
    humidity_sensor->humidity = 0.0;

    HAL_TIM_Base_Start_IT(humidity_sensor->Interface.main_timer);
    HAL_TIM_Base_Start(humidity_sensor->Interface.high_lvl_timer);
}

void switch_to_hum_sensor(humidity_sensor_t *humidity_sensor)
{
    internal_humidity.Interface.GPIOx = humidity_sensor->Interface.GPIOx;
    internal_humidity.Interface.GPIO_Pin = humidity_sensor->Interface.GPIO_Pin;

    internal_humidity.Interface.main_timer = humidity_sensor->Interface.main_timer;
    internal_humidity.Interface.high_lvl_timer = humidity_sensor->Interface.high_lvl_timer;
}

void get_humidity(humidity_sensor_t *humidity_sensor)
{
    if (!humidity_sensor) return;

    humidity_sensor->PWM_signal_state = internal_humidity.PWM_signal_state;
    humidity_sensor->high_lvl_cnt = internal_humidity.high_lvl_cnt;
    humidity_sensor->humidity = internal_humidity.humidity;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == internal_humidity.Interface.GPIO_Pin) {
        if (HAL_GPIO_ReadPin(internal_humidity.Interface.GPIOx, internal_humidity.Interface.GPIO_Pin)) {
            if (internal_humidity.PWM_signal_state == 0) HAL_TIM_Base_Start(internal_humidity.Interface.high_lvl_timer);
            internal_humidity.PWM_signal_state = 1;
        } else {
            if (internal_humidity.PWM_signal_state == 1) HAL_TIM_Base_Stop(internal_humidity.Interface.high_lvl_timer);
            internal_humidity.PWM_signal_state = 0;
        }
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == internal_humidity.Interface.main_timer->Instance) {
        internal_humidity.high_lvl_cnt = internal_humidity.Interface.high_lvl_timer->Instance->CNT;
        internal_humidity.humidity = 125.0 * (internal_humidity.high_lvl_cnt/ (float) HUM_PERIOD) - 6.0;

        if (internal_humidity.PWM_signal_state == 1) HAL_TIM_Base_Stop(internal_humidity.Interface.high_lvl_timer);

        internal_humidity.Interface.high_lvl_timer->Instance->CNT &= 0;

        if (internal_humidity.PWM_signal_state == 1) HAL_TIM_Base_Start(internal_humidity.Interface.high_lvl_timer);
    }
}