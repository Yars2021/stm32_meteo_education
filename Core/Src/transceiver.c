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

    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;   
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_Delay(10);
    
    uint8_t ret;
    ret = read_reg(REG_VERSION);
	if(ret != 0x12){
		Error_Handler();
	}
	write_reg(REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_SLEEP));
	set_frequency(FREQUENCY[FREQ_867MHZ]);
	write_reg(REG_FIFO_TX_BASE_ADDR, 0);
	write_reg(REG_FIFO_RX_BASE_ADDR, 0);
	ret = read_reg(REG_LNA);
	write_reg(REG_LNA, ret | 0x03);
	write_reg(REG_MODEM_CONFIG_3, 0x04);
	write_reg(REG_PA_CONFIG, 0x8f);
	write_reg(REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_STDBY));
}

uint8_t read_reg(uint8_t addr) {
	uint8_t txByte = addr & 0x7f;
	uint8_t rxByte = 0x00;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, &txByte, 1, 1000);
	while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	
	HAL_SPI_Receive(&spi,&rxByte, 1, 1000);
	while(HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
	return rxByte;
}

void write_reg(uint8_t addr, uint8_t cmd){
	uint8_t add = addr | 0x80;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
    HAL_SPI_Transmit(&spi, &add, 1, 1000);
	while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(&spi, &cmd, 1, 1000);
	while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
}

void set_frequency(uint64_t freq){
	uint64_t frf = ((uint64_t)freq << 19) / 32000000;
  write_reg(REG_FRF_MSB, (uint8_t)(frf >> 16));
  write_reg(REG_FRF_MID, (uint8_t)(frf >> 8));
  write_reg(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

uint8_t begin_packet(){
	//int ret;
	if ((read_reg(REG_OP_MODE) & MODE_TX) == MODE_TX) {
    return 1;
  }
	write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
	write_reg(REG_MODEM_CONFIG_1, 0x72);
	write_reg(REG_FIFO_ADDR_PTR, 0);
    write_reg(REG_PAYLOAD_LENGTH, 0);
	return 0;
}

void transmite(uint8_t * buf, uint8_t size){
	int currentLength = read_reg(REG_PAYLOAD_LENGTH);
  if ((currentLength + size > MAX_PKT_LENGTH)){
    size = MAX_PKT_LENGTH - currentLength;
  }

  for (int i = 0; i < size; i++) {
    write_reg(REG_FIFO, buf[i]);
  }
  write_reg(REG_PAYLOAD_LENGTH, currentLength + size);
}

uint8_t end_packet(){
	uint8_t timeout = 100;
	write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
    while((read_reg(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0) {
            if(--timeout==0){
                HAL_Delay(1);
                return 1;
            }
    }
    write_reg(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    return 0;
}