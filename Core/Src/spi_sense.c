#include "include.h"

double temp = 0;

void init_spi(){
    __HAL_RCC_SPI2_CLK_ENABLE();
    static SPI_HandleTypeDef spi = { .Instance = SPI2 };
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

    GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    char str[2] = {0};
    char out[2] = {0};
    
    while(1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&spi, out, str, 2, 1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        uint16_t temp_data = (str[0]<<8)|str[1];
        if(temp_data & 0x200){
            temp_data >>=5;
            temp = temp_data * 0.25;
        } else {
            temp_data >>=5;
            temp =temp_data * 0.25;
        }
    }
    HAL_Delay(1000);
}