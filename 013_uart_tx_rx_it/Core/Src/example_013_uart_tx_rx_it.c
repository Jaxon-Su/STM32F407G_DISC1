#include "example_013_uart_tx_rx_it.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

static uint8_t msg0[] = "hihihihihihi123";
static uint8_t msg1[] = "Hello How are you ?";
static uint8_t msg2[] = "Today is Monday !";
static uint8_t *msgs[] = { msg0, msg1, msg2 };
static uint8_t rx_buf[128];
static volatile uint8_t rx_complete;
static uint32_t msg_index;

static uint8_t WaitButtonPress(void);

void Example013_UartTxRxIt_Init(void)
{
    rx_complete = 0;
    msg_index = 0;
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
}

void Example013_UartTxRxIt_Loop(void)
{
    uint8_t *msg;
    uint16_t len;
    if (!WaitButtonPress()) return;
    msg = msgs[msg_index % 3U];
    len = (uint16_t)strlen((const char *)msg);
    rx_complete = 0;
    if (HAL_UART_Receive_IT(&huart2, rx_buf, len) != HAL_OK) { HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); return; }
    if (HAL_UART_Transmit(&huart2, msg, len, 100) != HAL_OK) { HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); return; }
    while (!rx_complete) {}
    rx_buf[len] = '\0';
    msg_index++;
    HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) rx_complete = 1;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) { rx_complete = 1; HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); }
}

static uint8_t WaitButtonPress(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    HAL_Delay(200);
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) HAL_Delay(10);
    return 1U;
}