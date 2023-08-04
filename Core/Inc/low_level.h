#ifndef _LOW_LEVEL_H_
#define _LOW_LEVEL_H_

// USART Modem
#define PIN_UART_MODEM_TX				B,10,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART_MODEM_RX				B,11,H,INPUT_FLOATING,SPEED_NONE

// USART Debug
#define PIN_UART_GPS_TX	    			A, 9,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART_GPS_RX 				A,10,H,INPUT_FLOATING,SPEED_NONE

// SPI для общения с трансиверами
#define PIN_TRC_SPI_SCK					A, 5,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_TRC_SPI_MOSI				A, 7,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_TRC_SPI_MISO				A, 6,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_TRC_SPI_CS					A, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Дополнительные ноги для трансивера
#define PIN_TRC_INT0					B, 5,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_TRC_INT1					B, 6,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_TRC_INT2					B, 7,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_TRC_RESET					B, 8,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Управление усилителями + переключатель тракта
#define PIN_SWITCH_RX_EN				B, 0,H,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SWITCH_TX_EN				B, 1,H,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_AMP_TX_EN					B, 3,H,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_AMP_RX_EN					B, 4,H,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Питание на модуль GPS
#define PIN_SERVO_PWR_EN				A, 8,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// SPI для общения с FLASH памятью
#define PIN_FLASH_SPI_SCK				B,13,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_FLASH_SPI_MOSI				B,15,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_FLASH_SPI_MISO				B,14,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_FLASH_SPI_CS				B,12,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// UART на телеметрию
#define PIN_UART_TELEMETRY_TX           A, 2,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART_TELEMETRY_RX           A, 3,H,INPUT_FLOATING,SPEED_50MHZ


// LED`s
#define PIN_LED_GREEN					A,15,H,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_LED_BLUE					A,12,H,OUTPUT_PUSH_PULL,SPEED_50MHZ

// PA Ramp
#define PIN_PA_RAMP						A,11,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ

#define PIN_MISO_SPI2                   B,14,H,INPUT_FLOATING,SPEED_50MHZ

#define uart_debug						uart1
#define uart_modem						uart3
#define uart_ap3						uart1

void initLowLevel (void);
void switchToRx (void);
void switchToTx (void);
void set_pa_level (uint8_t percent);
void init_spi(void);

#endif /* _LOW_LEVEL_H_ */
