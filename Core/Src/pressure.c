#include "include.h"

void init_pressure(pressure_sensor_t *pressure_sensor, GPIO_TypeDef *CS_GPIOx, uint16_t CS_GPIO_Pin, SPI_HandleTypeDef *spi)
{
    if (!pressure_sensor) return;

    pressure_sensor->Interface.CS_GPIOx = CS_GPIOx;
    pressure_sensor->Interface.CS_GPIO_Pin = CS_GPIO_Pin;
    pressure_sensor->Interface.spi = spi;

    pressure_sensor->raw_data[0] = 0;
    pressure_sensor->raw_data[1] = 0;
    pressure_sensor->raw_data[2] = 0;
    pressure_sensor->pressure = 0.0;
}

HAL_StatusTypeDef get_pressure(pressure_sensor_t *pressure_sensor)
{
    if (!pressure_sensor) return HAL_ERROR;

    HAL_StatusTypeDef hal_status = HAL_OK;

    /*
        100000 - ctrl1 address
        
        ctrl1 data:
            [0] = 1 (PD, active mode)
            [1] = 1 (ODR2) \
            [2] = 0 (ODR1)  > ODR = 100 (25HZ pressure output rate)  
            [3] = 0 (ODR3) /
            [4] = 0 (DIFF_EN, interrupts disabled)
            [5] = 0 (DBDU, continuous data update)
            [6] = 0 (DELTA_EN, delta regs disabled)
            [7] = 0 (SIM, 4-wire SPI interface)
    */

    uint8_t ctrl1[2] = {0b00100000, 0b11000000};
    uint8_t tx_h  = 0b10101010, 
            tx_l  = 0b10101001, 
            tx_xl = 0b10101000;

    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_RESET);
    hal_status |= HAL_SPI_Transmit(pressure_sensor->Interface.spi, ctrl1, 2, 512);
    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_RESET);
    hal_status |= HAL_SPI_Transmit(pressure_sensor->Interface.spi, &tx_xl, 1, 512);
    hal_status |= HAL_SPI_Receive(pressure_sensor->Interface.spi, pressure_sensor->raw_data + 2, 1, 512);
    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_RESET);
    hal_status |= HAL_SPI_Transmit(pressure_sensor->Interface.spi, &tx_l, 1, 512);
    hal_status |= HAL_SPI_Receive(pressure_sensor->Interface.spi, pressure_sensor->raw_data + 1, 1, 512);
    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_RESET);
    hal_status |= HAL_SPI_Transmit(pressure_sensor->Interface.spi, &tx_h, 1, 512);
    hal_status |= HAL_SPI_Receive(pressure_sensor->Interface.spi, pressure_sensor->raw_data, 1, 512);
    HAL_GPIO_WritePin(pressure_sensor->Interface.CS_GPIOx, pressure_sensor->Interface.CS_GPIO_Pin, GPIO_PIN_SET);

    pressure_sensor->pressure =    (pressure_sensor->raw_data[0] << 16 | 
                                    pressure_sensor->raw_data[1] << 8 | 
                                    pressure_sensor->raw_data[2]) / 4096.0;
}