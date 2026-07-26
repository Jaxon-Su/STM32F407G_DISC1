/*
 * example_002_spi_tx_testing.c
 *
 *  Created on: 2026-07-26
 *      Author: su622
 */

#include "example_002_spi_tx_testing.h"

#include <string.h>

extern SPI_HandleTypeDef hspi2;

static const uint8_t kUserData[] = "Hello world";

void Example002_SpiTxTesting_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
}

void Example002_SpiTxTesting_Loop(void)
{
    if (HAL_SPI_Transmit(&hspi2,
                         (uint8_t *)kUserData,
                         strlen((const char *)kUserData),
                         HAL_MAX_DELAY) == HAL_OK)
    {
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }

    HAL_Delay(1000);
}

