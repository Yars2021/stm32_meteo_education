#include "include.h"

extern TIM_HandleTypeDef htim2;

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
_Noreturn void HardFault_Handler(void)
{
    while (1)
    {

    }
}

/**
  * @brief This function handles Memory management fault.
  */
_Noreturn void MemManage_Handler(void)
{
    while (1)
    {

    }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
_Noreturn void BusFault_Handler(void)
{
    while (1)
    {

    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
_Noreturn void UsageFault_Handler(void)
{
    while (1)
    {

    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

_Noreturn void Error_Handler (void)
{
    while(1)
    {

    }
}

void EXTI1_IRQHandler(void)
{
	  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}