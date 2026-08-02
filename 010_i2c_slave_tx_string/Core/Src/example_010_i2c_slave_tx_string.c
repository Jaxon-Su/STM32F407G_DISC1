#include "example_010_i2c_slave_tx_string.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

static const uint8_t tx_buf[] = "STM32 Slave mode testing..";
static uint8_t command_code, tx_index, tx_byte, rx_command;
static volatile uint8_t listen_restart;

static void ArmListen(void);

void Example010_I2cSlaveTxString_Init(void)
{
    command_code = 0xFFU;
    tx_index = 0;
    listen_restart = 1;
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    ArmListen();
}

void Example010_I2cSlaveTxString_Loop(void)
{
    if (listen_restart) { listen_restart = 0; ArmListen(); }
}

static void ArmListen(void)
{
    if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    (void)AddrMatchCode;
    if (hi2c->Instance != I2C1) return;
    if (TransferDirection == I2C_DIRECTION_TRANSMIT)
    {
        HAL_I2C_Slave_Receive_IT(&hi2c1, &rx_command, 1);
    }
    else
    {
        if (command_code == 0x51U) tx_byte = (uint8_t)strlen((const char *)tx_buf);
        else if (command_code == 0x52U)
        {
            tx_byte = tx_buf[tx_index++];
            if (tx_index >= strlen((const char *)tx_buf)) tx_index = 0;
        }
        else tx_byte = 0xFFU;
        HAL_I2C_Slave_Transmit_IT(&hi2c1, &tx_byte, 1);
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance != I2C1) return;
    command_code = rx_command;
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) listen_restart = 1;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) { command_code = 0xFFU; tx_index = 0; listen_restart = 1; HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); }
}