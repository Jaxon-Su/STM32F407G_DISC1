#include "examples.h"

void Example001_ButtonInterrupt_Init(void)
{
    HAL_GPIO_WritePin(LED_PD12_GPIO_Port, LED_PD12_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PD13_GPIO_Port, LED_PD13_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PD14_GPIO_Port, LED_PD14_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PD15_GPIO_Port, LED_PD15_Pin, GPIO_PIN_RESET);
}

void Example001_ButtonInterrupt_Loop(void)
{
	HAL_Delay(500);
	HAL_GPIO_TogglePin(LED_PD13_GPIO_Port, LED_PD13_Pin);
	HAL_GPIO_TogglePin(LED_PD14_GPIO_Port, LED_PD14_Pin);
	HAL_GPIO_TogglePin(LED_PD15_GPIO_Port, LED_PD15_Pin);

}

void Example001_ButtonInterrupt_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BTN_PA0_Pin)
    {
        HAL_GPIO_TogglePin(LED_PD12_GPIO_Port, LED_PD12_Pin);
    }
}
