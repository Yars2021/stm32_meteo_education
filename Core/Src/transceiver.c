#include "include.h"

HAL_StatusTypeDef status;
SPI_HandleTypeDef spi;

void init_trans(){
    __HAL_RCC_SPI1_CLK_ENABLE();
    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_MASTER;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spi) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(10);
}

uint8_t read_reg(uint8_t addr) {
	uint8_t txByte = addr & 0x7f;
	uint8_t rxByte = 0x00;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(&spi, &txByte, 1, 1000);
	while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	
	status = HAL_SPI_Receive(&spi,&rxByte, 1, 1000);
	while(HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
	return rxByte;
}

void trans_transmit(uint8_t message){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(&spi, message, sizeof(message), 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

uint8_t trans_reseive(void){
    uint8_t message = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    status = HAL_SPI_Receive(&spi, &message, sizeof(message), 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    return message;
}