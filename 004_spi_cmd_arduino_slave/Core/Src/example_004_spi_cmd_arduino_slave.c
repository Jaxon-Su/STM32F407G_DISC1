/*
 * example_004_spi_cmd_arduino_slave.c
 *
 *  Created on: 2026-07-27
 *      Author: su622
 */

#include "example_004_spi_cmd_arduino_slave.h"

#include <string.h>

extern SPI_HandleTypeDef hspi2;

#define COMMAND_LED_CTRL      0x50U
#define COMMAND_SENSOR_READ   0x51U
#define COMMAND_LED_READ      0x52U
#define COMMAND_PRINT         0x53U
#define COMMAND_ID_READ       0x54U

#define ARDUINO_ACK           0xF5U
#define DUMMY_BYTE            0xFFU

#define LED_ON                1U
#define ANALOG_PIN0           0U
#define ARDUINO_LED_PIN       9U

typedef enum
{
    EXAMPLE004_CMD_LED_CTRL = 0,
    EXAMPLE004_CMD_SENSOR_READ,
    EXAMPLE004_CMD_LED_READ,
    EXAMPLE004_CMD_PRINT,
    EXAMPLE004_CMD_ID_READ,
    EXAMPLE004_CMD_COUNT
} Example004_CommandStep_t;

static Example004_CommandStep_t s_commandStep = EXAMPLE004_CMD_LED_CTRL;
static uint8_t s_lastSensorValue = 0;
static uint8_t s_lastLedStatus = 0;
static uint8_t s_lastId[11] = {0};

static uint8_t Example004_WaitButtonPress(void);
static HAL_StatusTypeDef Example004_TransferByte(uint8_t txByte, uint8_t *rxByte);
static HAL_StatusTypeDef Example004_SendByteIgnoreRx(uint8_t txByte);
static uint8_t Example004_RequestAck(uint8_t commandCode);
static void Example004_ShowResult(uint8_t success);

void Example004_SpiCmdArduinoSlave_Init(void)
{
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
}

void Example004_SpiCmdArduinoSlave_Loop(void)
{
    uint8_t success = 0;
    uint8_t args[2];
    uint8_t message[] = "Hello ! How are you ??";

    if (!Example004_WaitButtonPress())
    {
        return;
    }

    switch (s_commandStep)
    {
        case EXAMPLE004_CMD_LED_CTRL:
            if (Example004_RequestAck(COMMAND_LED_CTRL))
            {
                args[0] = ARDUINO_LED_PIN;
                args[1] = LED_ON;
                success = (HAL_SPI_Transmit(&hspi2, args, 2, HAL_MAX_DELAY) == HAL_OK);
            }
            break;

        case EXAMPLE004_CMD_SENSOR_READ:
            if (Example004_RequestAck(COMMAND_SENSOR_READ))
            {
                args[0] = ANALOG_PIN0;
                success = (Example004_SendByteIgnoreRx(args[0]) == HAL_OK);
                HAL_Delay(10);

                if (success)
                {
                    success = (Example004_TransferByte(DUMMY_BYTE, &s_lastSensorValue) == HAL_OK);
                }
            }
            break;

        case EXAMPLE004_CMD_LED_READ:
            if (Example004_RequestAck(COMMAND_LED_READ))
            {
                args[0] = ARDUINO_LED_PIN;
                success = (Example004_SendByteIgnoreRx(args[0]) == HAL_OK);
                HAL_Delay(10);

                if (success)
                {
                    success = (Example004_TransferByte(DUMMY_BYTE, &s_lastLedStatus) == HAL_OK);
                }
            }
            break;

        case EXAMPLE004_CMD_PRINT:
            if (Example004_RequestAck(COMMAND_PRINT))
            {
                uint8_t msgLen = (uint8_t)strlen((char *)message);
                success = (Example004_SendByteIgnoreRx(msgLen) == HAL_OK);
                HAL_Delay(10);

                if (success)
                {
                    success = (HAL_SPI_Transmit(&hspi2, message, msgLen, HAL_MAX_DELAY) == HAL_OK);
                }
            }
            break;

        case EXAMPLE004_CMD_ID_READ:
            if (Example004_RequestAck(COMMAND_ID_READ))
            {
                success = 1;
                for (uint8_t i = 0; i < 10; i++)
                {
                    if (Example004_TransferByte(DUMMY_BYTE, &s_lastId[i]) != HAL_OK)
                    {
                        success = 0;
                        break;
                    }
                }
                s_lastId[10] = '\0';
            }
            break;

        default:
            s_commandStep = EXAMPLE004_CMD_LED_CTRL;
            break;
    }

    __HAL_SPI_DISABLE(&hspi2);
    Example004_ShowResult(success);

    s_commandStep++;
    if (s_commandStep >= EXAMPLE004_CMD_COUNT)
    {
        s_commandStep = EXAMPLE004_CMD_LED_CTRL;
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
    }
}

static uint8_t Example004_WaitButtonPress(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return 0;
    }

    HAL_Delay(200);

    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET)
    {
        return 0;
    }

    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(10);
    }

    return 1;
}

static HAL_StatusTypeDef Example004_TransferByte(uint8_t txByte, uint8_t *rxByte)
{
    return HAL_SPI_TransmitReceive(&hspi2, &txByte, rxByte, 1, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef Example004_SendByteIgnoreRx(uint8_t txByte)
{
    uint8_t dummyRead;
    return Example004_TransferByte(txByte, &dummyRead);
}

static uint8_t Example004_RequestAck(uint8_t commandCode)
{
    uint8_t ackByte = 0;

    if (Example004_SendByteIgnoreRx(commandCode) != HAL_OK)
    {
        return 0;
    }

    if (Example004_TransferByte(DUMMY_BYTE, &ackByte) != HAL_OK)
    {
        return 0;
    }

    return (ackByte == ARDUINO_ACK);
}

static void Example004_ShowResult(uint8_t success)
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