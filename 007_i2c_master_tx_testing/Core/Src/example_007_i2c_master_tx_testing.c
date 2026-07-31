/*
 * example_007_i2c_master_tx_testing.c
 *
 *  Created on: 2026-07-31
 *      Author: su622
 */

#include "example_007_i2c_master_tx_testing.h"

#include <string.h>

extern I2C_HandleTypeDef hi2c1;

#define EXAMPLE007_SLAVE_ADDR_7BIT    (0x68U)
#define EXAMPLE007_TX_TIMEOUT_MS      (100U)

static const uint8_t kSomeData[] = "We are testing I2C master Tx\n";

static uint8_t Example007_WaitButtonPress(void);
static void Example007_ShowResult(HAL_StatusTypeDef status);

void Example007_I2cMasterTxTesting_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
}

void Example007_I2cMasterTxTesting_Loop(void)
{
    HAL_StatusTypeDef status;

    if (Example007_WaitButtonPress() == 0U)
    {
        return;
    }

    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     (EXAMPLE007_SLAVE_ADDR_7BIT << 1),
                                     (uint8_t *)kSomeData,
                                     (uint16_t)strlen((const char *)kSomeData),
                                     EXAMPLE007_TX_TIMEOUT_MS);

    Example007_ShowResult(status);
}

static uint8_t Example007_WaitButtonPress(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return 0U;
    }

    HAL_Delay(200);

    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return 0U;
    }

    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(10);
    }

    return 1U;
}

static void Example007_ShowResult(HAL_StatusTypeDef status)
{
    if (status == HAL_OK)
    {
        HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else
    {
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }
}