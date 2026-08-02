#include "example_011_i2c_slave_tx_long_string.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

static const uint8_t tx_buf[] = "HiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHi...123";
static uint32_t data_len, length_index, data_index;
static uint8_t command_code, rx_command, tx_byte;
static volatile uint8_t listen_restart;

static void ArmListen(void);

void Example011_I2cSlaveTxLongString_Init(void)
{
    data_len = strlen((const char *)tx_buf);
    length_index = 0; data_index = 0; command_code = 0xFFU; listen_restart = 1;
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    ArmListen();
}

void Example011_I2cSlaveTxLongString_Loop(void)
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
        if (command_code == 0x51U)
        {
            tx_byte = (uint8_t)((data_len >> ((length_index % 4U) * 8U)) & 0xFFU);
            length_index++;
        }
        else if (command_code == 0x52U)
        {
            tx_byte = tx_buf[data_index++];
            if (data_index >= data_len) { data_index = 0; command_code = 0xFFU; }
        }
        else tx_byte = 0xFFU;
        HAL_I2C_Slave_Transmit_IT(&hi2c1, &tx_byte, 1);
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance != I2C1) return;
    command_code = rx_command;
    if (command_code == 0x51U) length_index = 0;
    if (command_code == 0x52U) data_index = 0;
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
    if (hi2c->Instance == I2C1) { listen_restart = 1; HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); }
}