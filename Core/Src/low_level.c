#include "include.h"

static void init_rcc (void);
static void init_gpio (void);
static void init_spi(void);

SPI_HandleTypeDef spi_2;

void initLowLevel (void)
{
    HAL_Init();
    init_rcc();
    init_spi();
    init_gpio();
}

static void init_rcc (void)
{
    RCC_OscInitTypeDef sRCC = {0};
    RCC_ClkInitTypeDef sCLK = {0};

    sRCC.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    sRCC.HSEState = RCC_HSE_ON;
    sRCC.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    sRCC.HSIState = RCC_HSI_ON;
    sRCC.PLL.PLLState = RCC_PLL_ON;
    sRCC.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    sRCC.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&sRCC) != HAL_OK)
    {
        Error_Handler();
    }

    sCLK.ClockType = RCC_CLOCKTYPE_HCLK |
                     RCC_CLOCKTYPE_SYSCLK |
                     RCC_CLOCKTYPE_PCLK1 |
                     RCC_CLOCKTYPE_PCLK2;
    sCLK.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    sCLK.AHBCLKDivider = RCC_SYSCLK_DIV1;
    sCLK.APB1CLKDivider = RCC_HCLK_DIV2;
    sCLK.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&sCLK, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void init_gpio (void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    pin_init(PIN_LED_GREEN);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // PB2 pwr_periph_EN
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    // PB11 p_CS (chip select) | PB12 t_CS (chip select) | 
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PB13 spi_SCK | PB14 spi_MISO | PB15 spi_MOSI
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

static void init_spi()
{
    __HAL_RCC_SPI2_CLK_ENABLE();
    spi_2.Instance = SPI2;
    spi_2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    spi_2.Init.Direction = SPI_DIRECTION_2LINES;
    spi_2.Init.CLKPhase = SPI_PHASE_2EDGE;
    spi_2.Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi_2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi_2.Init.DataSize = SPI_DATASIZE_8BIT;
    spi_2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi_2.Init.NSS = SPI_NSS_SOFT;
    spi_2.Init.TIMode = SPI_TIMODE_DISABLE;
    spi_2.Init.Mode = SPI_MODE_MASTER; 
    spi_2.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&spi_2) != HAL_OK)
    {
        Error_Handler();
    }
}