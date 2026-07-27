/*
 * example_003_spi_tx_arduino_button.c
 *
 *  Created on: 2026-07-27
 *      Author: su622
 */

#include "example_003_spi_tx_arduino_button.h"

#include <string.h>

extern SPI_HandleTypeDef hspi2;

static const char kUserData[] =
    "An Arduino Uno board is best suited for beginners who have just started "
    "using microcontrollers, on the other hand, Arduino Mega board is for "
    "enthusiasts who require a lot of I/O pins for their projects";

void Example003_SpiTxArduinoButton_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
}

void Example003_SpiTxArduinoButton_Loop(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return;
    }

    HAL_Delay(200);

    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return;
    }

    uint8_t dataLen = (uint8_t)strlen(kUserData);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi2, &dataLen, 1, HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        status = HAL_SPI_Transmit(&hspi2,
                                  (uint8_t *)kUserData,
                                  dataLen,
                                  HAL_MAX_DELAY);
    }

    __HAL_SPI_DISABLE(&hspi2);

    if (status == HAL_OK)
    {
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else
    {
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }

    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(10);
    }
}