#include "include.h"
extern SPI_HandleTypeDef spi_1;

uint8_t begin_packet(transiver *trans);
uint8_t end_packet(transiver *trans);
uint8_t read_reg(uint8_t addr, transiver *trans);
void write_reg(uint8_t addr, uint8_t cmd, transiver *trans);
void set_frequency(uint64_t freq, transiver *trans);

void init_trans(transiver *trans, GPIO_TypeDef *CS_PORT, uint32_t CS_Pin,SPI_TypeDef *SPI){

  trans->Inteface.SPI = SPI;
  trans->Inteface.CS_Pin = CS_Pin;
  trans->Inteface.CS_PORT = CS_PORT;
    
    uint8_t ret;
    ret = read_reg(REG_VERSION, trans);
	if(ret != 0x12){
		Error_Handler();
	}
	write_reg(REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_SLEEP), trans);
	set_frequency(FREQUENCY[FREQ_867MHZ], trans);
	write_reg(REG_FIFO_TX_BASE_ADDR, 0, trans);
	write_reg(REG_FIFO_RX_BASE_ADDR, 0, trans);
	ret = read_reg(REG_LNA, trans);
	write_reg(REG_LNA, ret | 0x03, trans);
	write_reg(REG_MODEM_CONFIG_3, 0x04, trans);
	write_reg(REG_PA_CONFIG, 0x8f, trans);
	write_reg(REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_STDBY), trans);
}

uint8_t read_reg(uint8_t addr, transiver *trans) {
	uint8_t txByte = addr & 0x7f;
	uint8_t rxByte = 0x00;
	HAL_GPIO_WritePin(trans->Inteface.CS_PORT, trans->Inteface.CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(trans->Inteface.SPI, &txByte, 1, 1000);
	while (HAL_SPI_GetState(trans->Inteface.SPI) != HAL_SPI_STATE_READY);
	
	HAL_SPI_Receive(trans->Inteface.SPI,&rxByte, 1, 1000);
	while(HAL_SPI_GetState(trans->Inteface.SPI) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(trans->Inteface.CS_PORT, trans->Inteface.CS_Pin,GPIO_PIN_SET);
	return rxByte;
}

void write_reg(uint8_t addr, uint8_t cmd, transiver *trans){
	uint8_t add = addr | 0x80;
    HAL_GPIO_WritePin(trans->Inteface.CS_PORT, trans->Inteface.CS_Pin,GPIO_PIN_RESET);
    HAL_SPI_Transmit(trans->Inteface.SPI, &add, 1, 1000);
	while (HAL_SPI_GetState(trans->Inteface.SPI) != HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(trans->Inteface.SPI, &cmd, 1, 1000);
	while (HAL_SPI_GetState(trans->Inteface.SPI) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(trans->Inteface.CS_PORT, trans->Inteface.CS_Pin,GPIO_PIN_SET);
}

void set_frequency(uint64_t freq, transiver *trans){
	uint64_t frf = ((uint64_t)freq << 19) / 32000000;
  write_reg(REG_FRF_MSB, (uint8_t)(frf >> 16), trans);
  write_reg(REG_FRF_MID, (uint8_t)(frf >> 8), trans);
  write_reg(REG_FRF_LSB, (uint8_t)(frf >> 0), trans);
}

uint8_t begin_packet(transiver *trans){
	//int ret;
	if ((read_reg(REG_OP_MODE, trans) & MODE_TX) == MODE_TX) {
    return 1;
  }
	write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY, trans);
	write_reg(REG_MODEM_CONFIG_1, 0x72, trans);
	write_reg(REG_FIFO_ADDR_PTR, 0, trans);
    write_reg(REG_PAYLOAD_LENGTH, 0, trans);
	return 0;
}

void transmite(transiver *trans){
  begin_packet(trans);
	int currentLength = read_reg(REG_PAYLOAD_LENGTH, trans);
  if ((currentLength + trans->output_size > MAX_PKT_LENGTH)){
    trans->output_size = MAX_PKT_LENGTH - currentLength;
  }

  for (int i = 0; i < trans->output_size; i++) {
    write_reg(REG_FIFO, trans->output[i], trans);
  }
  write_reg(REG_PAYLOAD_LENGTH, currentLength + trans->output_size, trans);
  end_packet(trans);
}

uint8_t end_packet(transiver *trans){
	uint8_t timeout = 100;
	write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX, trans);
    while((read_reg(REG_IRQ_FLAGS, trans) & IRQ_TX_DONE_MASK) == 0) {
            if(--timeout==0){
                HAL_Delay(1);
                return 1;
            }
    }
    write_reg(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK, trans);
    return 0;
}