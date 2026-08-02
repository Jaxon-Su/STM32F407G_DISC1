#include "example_014_ds1307_rtc_i2c.h"

extern I2C_HandleTypeDef hi2c1;

#define DS1307_ADDR_7BIT      (0x68U)
#define DS1307_REG_SECONDS    (0x00U)
#define DS1307_REG_DAY        (0x03U)
#define DS1307_TIMEOUT_MS     (100U)

typedef struct { uint8_t seconds; uint8_t minutes; uint8_t hours; } RtcTime;
typedef struct { uint8_t day; uint8_t date; uint8_t month; uint8_t year; } RtcDate;

static RtcTime current_time;
static RtcDate current_date;
static uint32_t last_tick;

static uint8_t DecToBcd(uint8_t value);
static uint8_t BcdToDec(uint8_t value);
static HAL_StatusTypeDef Ds1307SetTime(const RtcTime *time);
static HAL_StatusTypeDef Ds1307SetDate(const RtcDate *date);
static HAL_StatusTypeDef Ds1307GetTime(RtcTime *time);
static HAL_StatusTypeDef Ds1307GetDate(RtcDate *date);
static void ShowResult(HAL_StatusTypeDef status);

void Example014_Ds1307RtcI2c_Init(void)
{
    HAL_StatusTypeDef status;
    current_time.hours = 11; current_time.minutes = 59; current_time.seconds = 30;
    current_date.day = 5; current_date.date = 15; current_date.month = 1; current_date.year = 21;
    status = Ds1307SetDate(&current_date);
    if (status == HAL_OK) status = Ds1307SetTime(&current_time);
    last_tick = HAL_GetTick();
    ShowResult(status);
}

void Example014_Ds1307RtcI2c_Loop(void)
{
    HAL_StatusTypeDef status;
    if ((HAL_GetTick() - last_tick) < 1000U) return;
    last_tick = HAL_GetTick();
    status = Ds1307GetTime(&current_time);
    if (status == HAL_OK) status = Ds1307GetDate(&current_date);
    ShowResult(status);
}

static HAL_StatusTypeDef Ds1307SetTime(const RtcTime *time)
{
    uint8_t data[3];
    data[0] = DecToBcd(time->seconds) & 0x7FU;
    data[1] = DecToBcd(time->minutes);
    data[2] = DecToBcd(time->hours);
    return HAL_I2C_Mem_Write(&hi2c1, (DS1307_ADDR_7BIT << 1), DS1307_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, data, 3, DS1307_TIMEOUT_MS);
}

static HAL_StatusTypeDef Ds1307SetDate(const RtcDate *date)
{
    uint8_t data[4];
    data[0] = DecToBcd(date->day); data[1] = DecToBcd(date->date); data[2] = DecToBcd(date->month); data[3] = DecToBcd(date->year);
    return HAL_I2C_Mem_Write(&hi2c1, (DS1307_ADDR_7BIT << 1), DS1307_REG_DAY, I2C_MEMADD_SIZE_8BIT, data, 4, DS1307_TIMEOUT_MS);
}

static HAL_StatusTypeDef Ds1307GetTime(RtcTime *time)
{
    uint8_t data[3];
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, (DS1307_ADDR_7BIT << 1), DS1307_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, data, 3, DS1307_TIMEOUT_MS);
    if (status == HAL_OK) { time->seconds = BcdToDec(data[0] & 0x7FU); time->minutes = BcdToDec(data[1]); time->hours = BcdToDec(data[2] & 0x3FU); }
    return status;
}

static HAL_StatusTypeDef Ds1307GetDate(RtcDate *date)
{
    uint8_t data[4];
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, (DS1307_ADDR_7BIT << 1), DS1307_REG_DAY, I2C_MEMADD_SIZE_8BIT, data, 4, DS1307_TIMEOUT_MS);
    if (status == HAL_OK) { date->day = BcdToDec(data[0]); date->date = BcdToDec(data[1]); date->month = BcdToDec(data[2]); date->year = BcdToDec(data[3]); }
    return status;
}

static uint8_t DecToBcd(uint8_t value) { return (uint8_t)(((value / 10U) << 4U) | (value % 10U)); }
static uint8_t BcdToDec(uint8_t value) { return (uint8_t)(((value >> 4U) * 10U) + (value & 0x0FU)); }

static void ShowResult(HAL_StatusTypeDef status)
{
    if (status == HAL_OK) { HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET); HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin); }
    else HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
}