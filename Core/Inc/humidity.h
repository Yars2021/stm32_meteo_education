#ifndef _HUMIDIDTY_H_
#define _HUMIDIDTY_H_

#define HUM_PERIOD 65535
#define HUM_PRESCALER 495

typedef struct {
    struct {
        GPIO_TypeDef *GPIOx;
        uint16_t GPIO_Pin;
        TIM_HandleTypeDef *main_timer;
        TIM_HandleTypeDef *high_lvl_timer;
    } Interface;
    uint8_t PWM_signal_state; // 0 - low, 1 - high
    uint32_t high_lvl_cnt;
    float humidity;
} humidity_sensor_t;

void init_humidity(humidity_sensor_t *humidity_sensor, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, TIM_HandleTypeDef *main_tim, TIM_HandleTypeDef *high_lvl_tim);
void switch_to_hum_sensor(humidity_sensor_t *humidity_sensor);
void get_humidity(humidity_sensor_t *humidity_sensor);

#endif /* _HUMIDIDTY_H_ */