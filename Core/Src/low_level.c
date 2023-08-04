#include "include.h"

UART_HandleTypeDef huart2;

static void init_rcc (void);
static void init_gpio (void);
static HAL_StatusTypeDef init_gps_uart (void);

void initLowLevel (void)
{
    HAL_Init();
    init_rcc();
    init_gpio();
    init_gps_uart();
}

static void init_rcc (void)
{
    RCC_OscInitTypeDef sRCC = {0};
    RCC_ClkInitTypeDef sCLK = {0};

    sRCC.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    sRCC.HSEState = RCC_HSE_ON;
    sRCC.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
    sRCC.HSIState = RCC_HSI_ON;
    sRCC.PLL.PLLState = RCC_PLL_ON;
    sRCC.PLL.PLLSource = RCC_PLLSOURCE_HSE;
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

    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
}

static HAL_StatusTypeDef init_gps_uart(void)
{
    HAL_StatusTypeDef hal_status;
    uint8_t message;

    __HAL_RCC_USART2_CLK_ENABLE();

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = USART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;  

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_Delay(1000);

    hal_status = HAL_OK;

    hal_status |= HAL_UART_Transmit(&huart2, configUBX, sizeof(configUBX), 1000);
    HAL_Delay(250);
    hal_status |= HAL_UART_Transmit(&huart2, setNMEA410, sizeof(setNMEA410), 1000);
    HAL_Delay(250);
    hal_status |= HAL_UART_Transmit(&huart2, setGNSS, sizeof(setGNSS), 1000);
    HAL_Delay(250);

    hal_status |= HAL_UART_Receive(&huart2, &message, 1, 1000);

    return hal_status;
}