/*
 * example_008_i2c_master_rx_testing.c
 *
 *  Created on: 2026-07-31
 *      Author: su622
 */

#include "example_008_i2c_master_rx_testing.h"

#include <string.h>

extern I2C_HandleTypeDef hi2c1;

#define EXAMPLE008_SLAVE_ADDR_7BIT        (0x68U)
#define EXAMPLE008_CMD_READ_LENGTH        (0x51U)
#define EXAMPLE008_CMD_READ_PAYLOAD       (0x52U)
#define EXAMPLE008_RX_BUFFER_SIZE         (32U)
#define EXAMPLE008_I2C_TIMEOUT_MS         (100U)

static uint8_t rcv_buf[EXAMPLE008_RX_BUFFER_SIZE + 1U];

static uint8_t Example008_WaitButtonPress(void);
static HAL_StatusTypeDef Example008_ReadFromSlave(uint8_t *rx_data, uint8_t *rx_len);
static void Example008_ShowResult(HAL_StatusTypeDef status);

void Example008_I2cMasterRxTesting_Init(void)
{
    memset(rcv_buf, 0, sizeof(rcv_buf));

    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
}

void Example008_I2cMasterRxTesting_Loop(void)
{
    uint8_t len = 0;
    HAL_StatusTypeDef status;

    if (Example008_WaitButtonPress() == 0U)
    {
        return;
    }

    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

    status = Example008_ReadFromSlave(rcv_buf, &len);
    if (status == HAL_OK)
    {
        rcv_buf[len] = '\0';
    }

    Example008_ShowResult(status);
}

static uint8_t Example008_WaitButtonPress(void)
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

static HAL_StatusTypeDef Example008_ReadFromSlave(uint8_t *rx_data, uint8_t *rx_len)
{
    HAL_StatusTypeDef status;
    uint8_t command = EXAMPLE008_CMD_READ_LENGTH;
    uint8_t len = 0;
    uint16_t slave_addr = (EXAMPLE008_SLAVE_ADDR_7BIT << 1);

    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     slave_addr,
                                     &command,
                                     1,
                                     EXAMPLE008_I2C_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_I2C_Master_Receive(&hi2c1,
                                    slave_addr,
                                    &len,
                                    1,
                                    EXAMPLE008_I2C_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return status;
    }

    if (len > EXAMPLE008_RX_BUFFER_SIZE)
    {
        len = EXAMPLE008_RX_BUFFER_SIZE;
    }

    command = EXAMPLE008_CMD_READ_PAYLOAD;
    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     slave_addr,
                                     &command,
                                     1,
                                     EXAMPLE008_I2C_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_I2C_Master_Receive(&hi2c1,
                                    slave_addr,
                                    rx_data,
                                    len,
                                    EXAMPLE008_I2C_TIMEOUT_MS);
    if (status == HAL_OK)
    {
        *rx_len = len;
    }

    return status;
}

static void Example008_ShowResult(HAL_StatusTypeDef status)
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