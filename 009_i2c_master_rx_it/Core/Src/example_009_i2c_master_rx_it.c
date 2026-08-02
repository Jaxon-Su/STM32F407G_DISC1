#include "example_009_i2c_master_rx_it.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDR_7BIT      (0x68U)
#define CMD_READ_LENGTH      (0x51U)
#define CMD_READ_PAYLOAD     (0x52U)
#define RX_BUFFER_SIZE       (32U)

typedef enum { EX009_IDLE = 0, EX009_WAIT_LEN_CMD_TX, EX009_WAIT_LEN_RX, EX009_WAIT_PAYLOAD_CMD_TX, EX009_WAIT_PAYLOAD_RX, EX009_DONE, EX009_ERROR } Example009_State;

static uint8_t rx_buf[RX_BUFFER_SIZE + 1U];
static uint8_t rx_len;
static uint8_t command;
static volatile Example009_State state;

static uint8_t WaitButtonPress(void);
static void StartReadSequence(void);
static void ShowResult(HAL_StatusTypeDef status);

void Example009_I2cMasterRxIt_Init(void)
{
    memset(rx_buf, 0, sizeof(rx_buf));
    rx_len = 0;
    state = EX009_IDLE;
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

void Example009_I2cMasterRxIt_Loop(void)
{
    if (state == EX009_IDLE && WaitButtonPress())
    {
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        StartReadSequence();
    }

    if (state == EX009_DONE)
    {
        rx_buf[rx_len] = '\0';
        ShowResult(HAL_OK);
        state = EX009_IDLE;
    }
    else if (state == EX009_ERROR)
    {
        ShowResult(HAL_ERROR);
        state = EX009_IDLE;
    }
}

static void StartReadSequence(void)
{
    command = CMD_READ_LENGTH;
    state = EX009_WAIT_LEN_CMD_TX;
    if (HAL_I2C_Master_Transmit_IT(&hi2c1, (SLAVE_ADDR_7BIT << 1), &command, 1) != HAL_OK) state = EX009_ERROR;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance != I2C1) return;
    if (state == EX009_WAIT_LEN_CMD_TX)
    {
        state = EX009_WAIT_LEN_RX;
        if (HAL_I2C_Master_Receive_IT(&hi2c1, (SLAVE_ADDR_7BIT << 1), &rx_len, 1) != HAL_OK) state = EX009_ERROR;
    }
    else if (state == EX009_WAIT_PAYLOAD_CMD_TX)
    {
        state = EX009_WAIT_PAYLOAD_RX;
        if (HAL_I2C_Master_Receive_IT(&hi2c1, (SLAVE_ADDR_7BIT << 1), rx_buf, rx_len) != HAL_OK) state = EX009_ERROR;
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance != I2C1) return;
    if (state == EX009_WAIT_LEN_RX)
    {
        if (rx_len > RX_BUFFER_SIZE) rx_len = RX_BUFFER_SIZE;
        command = CMD_READ_PAYLOAD;
        state = EX009_WAIT_PAYLOAD_CMD_TX;
        if (HAL_I2C_Master_Transmit_IT(&hi2c1, (SLAVE_ADDR_7BIT << 1), &command, 1) != HAL_OK) state = EX009_ERROR;
    }
    else if (state == EX009_WAIT_PAYLOAD_RX) state = EX009_DONE;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) state = EX009_ERROR;
}

static uint8_t WaitButtonPress(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    HAL_Delay(200);
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) return 0U;
    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) HAL_Delay(10);
    return 1U;
}

static void ShowResult(HAL_StatusTypeDef status)
{
    if (status == HAL_OK) { HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET); HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin); }
    else HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
}