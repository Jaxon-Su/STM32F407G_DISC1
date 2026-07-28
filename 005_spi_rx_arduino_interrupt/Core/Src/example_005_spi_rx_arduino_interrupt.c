/*
 * example_005_spi_rx_arduino_interrupt.c
 *
 *  Created on: 2026-07-28
 *      Author: su622
 */

#include "example_005_spi_rx_arduino_interrupt.h"

#include <string.h>

extern SPI_HandleTypeDef hspi2;

#define EXAMPLE005_MAX_LEN         500U
#define EXAMPLE005_DUMMY_BYTE      0xFFU
#define EXAMPLE005_BYTE_TIMEOUT_MS 100U

static char s_rcvBuff[EXAMPLE005_MAX_LEN];
static uint8_t s_dummyByte = EXAMPLE005_DUMMY_BYTE;
static uint8_t s_readByte = 0U;

static volatile uint8_t s_dataAvailable = 0U;
static volatile uint8_t s_transferDone = 0U;
static volatile uint8_t s_receiveStop = 0U;
static volatile uint8_t s_messageReady = 0U;
static volatile uint8_t s_spiError = 0U;
static uint16_t s_rcvIndex = 0U;

static void Example005_ResetReceiveState(void);
static uint8_t Example005_WaitTransferDone(uint32_t timeoutMs);
static void Example005_ShowResult(uint8_t success);

void Example005_SpiRxArduinoInterrupt_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);

    Example005_ResetReceiveState();
    __HAL_SPI_DISABLE(&hspi2);
}

void Example005_SpiRxArduinoInterrupt_Loop(void)
{
    uint8_t success = 1U;

    if (!s_dataAvailable)
    {
        return;
    }

    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    s_dataAvailable = 0U;
    Example005_ResetReceiveState();
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET);

    while (!s_receiveStop)
    {
        s_transferDone = 0U;
        s_readByte = 0U;

        if (HAL_SPI_TransmitReceive_IT(&hspi2, &s_dummyByte, &s_readByte, 1U) != HAL_OK)
        {
            success = 0U;
            break;
        }

        if (!Example005_WaitTransferDone(EXAMPLE005_BYTE_TIMEOUT_MS))
        {
            HAL_SPI_Abort(&hspi2);
            success = 0U;
            break;
        }

        if (s_spiError)
        {
            success = 0U;
            break;
        }
    }

    while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_BSY))
    {
    }

    __HAL_SPI_DISABLE(&hspi2);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
    Example005_ShowResult(success);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

const char *Example005_SpiRxArduinoInterrupt_GetLastMessage(void)
{
    return s_rcvBuff;
}

uint8_t Example005_SpiRxArduinoInterrupt_IsMessageReady(void)
{
    return s_messageReady;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == SLAVE_INT_PD6_Pin)
    {
        s_dataAvailable = 1U;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance != SPI2)
    {
        return;
    }

    if (s_rcvIndex < (EXAMPLE005_MAX_LEN - 1U))
    {
        s_rcvBuff[s_rcvIndex++] = (char)s_readByte;
    }

    if ((s_readByte == '\0') || (s_rcvIndex >= (EXAMPLE005_MAX_LEN - 1U)))
    {
        s_rcvBuff[s_rcvIndex] = '\0';
        s_receiveStop = 1U;
        s_messageReady = 1U;
    }

    s_transferDone = 1U;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
    {
        s_spiError = 1U;
        s_receiveStop = 1U;
        s_transferDone = 1U;
    }
}

static void Example005_ResetReceiveState(void)
{
    memset(s_rcvBuff, 0, sizeof(s_rcvBuff));
    s_rcvIndex = 0U;
    s_readByte = 0U;
    s_transferDone = 0U;
    s_receiveStop = 0U;
    s_messageReady = 0U;
    s_spiError = 0U;
}

static uint8_t Example005_WaitTransferDone(uint32_t timeoutMs)
{
    uint32_t startTick = HAL_GetTick();

    while (!s_transferDone)
    {
        if ((HAL_GetTick() - startTick) >= timeoutMs)
        {
            return 0U;
        }
    }

    return 1U;
}

static void Example005_ShowResult(uint8_t success)
{
    if (success)
    {
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else
    {
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }
}