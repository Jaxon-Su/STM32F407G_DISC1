#include "example_012_uart_tx.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

static uint8_t msg[] = "UART Tx testing...\n\r";

static uint8_t WaitButtonPress(void);

void Example012_UartTx_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
}

void Example012_UartTx_Loop(void)
{
    if (!WaitButtonPress()) return;
    if (HAL_UART_Transmit(&huart2, msg, (uint16_t)strlen((const char *)msg), 100) == HAL_OK) HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    else HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
}

static uint8_t WaitButtonPress(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    HAL_Delay(200);
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) HAL_Delay(10);
    return 1U;
}