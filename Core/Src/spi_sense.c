#include "include.h"

SPI_HandleTypeDef spi;
TIM_HandleTypeDef htim1;
double temp = 0;

void init_spi(){
    __HAL_RCC_SPI2_CLK_ENABLE();
    spi.Instance = SPI2;
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.Mode = SPI_MODE_MASTER; 
    spi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spi) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    pin_init(PIN_MISO_SPI2);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    HAL_Delay(2000);

    __HAL_RCC_TIM1_CLK_ENABLE();

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 719;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 24999;
    htim1.Init.RepetitionCounter = 0;

    HAL_TIM_Base_Init(&htim1);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

    NVIC_EnableIRQ (TIM1_UP_IRQn);
    HAL_TIM_Base_Start_IT(&htim1);
}

void get_temp(char *str){
    char out[2] = {0};
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&spi, out, str, 2, 350);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

void TIM1_UP_IRQHandler(void) {
    char str[2] = {0};
    get_temp(str);
    uint16_t temp_data = (str[0]<<8)|str[1];
    temp_data >>=5;
    temp =temp_data * 0.25;
    if(temp>=128.0) temp = -(256.0-temp);
    HAL_TIM_IRQHandler(&htim1);
}