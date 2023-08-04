#include "include.h"

TIM_HandleTypeDef htim2, htim3;

static void init_rcc (void);
static void init_gpio (void);
static void init_TIM2 (void);
static void init_TIM3 (void);

void initLowLevel (void)
{
    HAL_Init();
    init_rcc();
    init_gpio();
    init_TIM2();
    init_TIM3();
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

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // PA1 senses_humidity_PWM
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PB2 pwr_periph_EN
    GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

static void init_TIM2(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = HUM_PRESCALER;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = HUM_PERIOD;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.RepetitionCounter = 0;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void init_TIM3(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = HUM_PRESCALER;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = HUM_PERIOD;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.RepetitionCounter = 0;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
}