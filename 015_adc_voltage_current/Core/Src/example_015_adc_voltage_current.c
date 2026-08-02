#include "example_015_adc_voltage_current.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_REFERENCE_MV       (3300UL)
#define ADC_MAX_COUNTS         (4095UL)
#define ADC_SAMPLE_PERIOD_MS   (500UL)

/*
 * Current conversion example:
 * current_mA = voltage_mV / shunt_mOhm
 *
 * If you use a current-sense amplifier, include its gain:
 * current_mA = voltage_mV * 1000 / (shunt_mOhm * gain)
 */
#define CURRENT_SHUNT_MOHM     (100UL)
#define CURRENT_SENSE_GAIN     (1UL)

volatile uint32_t g_adc_raw;
volatile uint32_t g_adc_voltage_mv;
volatile uint32_t g_adc_current_ma;

static uint32_t last_sample_tick;

static HAL_StatusTypeDef ReadAdcOnce(uint32_t *raw);
static uint32_t RawToMillivolt(uint32_t raw);
static uint32_t MillivoltToMilliamp(uint32_t mv);
static void ShowSampleStatus(HAL_StatusTypeDef status);

void Example015_AdcVoltageCurrent_Init(void)
{
    g_adc_raw = 0;
    g_adc_voltage_mv = 0;
    g_adc_current_ma = 0;
    last_sample_tick = HAL_GetTick();

    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
}

void Example015_AdcVoltageCurrent_Loop(void)
{
    uint32_t raw = 0;
    HAL_StatusTypeDef status;

    if ((HAL_GetTick() - last_sample_tick) < ADC_SAMPLE_PERIOD_MS)
    {
        return;
    }
    last_sample_tick = HAL_GetTick();

    status = ReadAdcOnce(&raw);
    if (status == HAL_OK)
    {
        g_adc_raw = raw;
        g_adc_voltage_mv = RawToMillivolt(raw);
        g_adc_current_ma = MillivoltToMilliamp(g_adc_voltage_mv);
    }

    ShowSampleStatus(status);
}

static HAL_StatusTypeDef ReadAdcOnce(uint32_t *raw)
{
    HAL_StatusTypeDef status;

    status = HAL_ADC_Start(&hadc1);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_ADC_PollForConversion(&hadc1, 10);
    if (status == HAL_OK)
    {
        *raw = HAL_ADC_GetValue(&hadc1);
    }

    (void)HAL_ADC_Stop(&hadc1);
    return status;
}

static uint32_t RawToMillivolt(uint32_t raw)
{
    return (raw * ADC_REFERENCE_MV) / ADC_MAX_COUNTS;
}

static uint32_t MillivoltToMilliamp(uint32_t mv)
{
    return (mv * 1000UL) / (CURRENT_SHUNT_MOHM * CURRENT_SENSE_GAIN);
}

static void ShowSampleStatus(HAL_StatusTypeDef status)
{
    if (status == HAL_OK)
    {
        HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        if (g_adc_voltage_mv >= (ADC_REFERENCE_MV / 2UL))
        {
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
        }
    }
    else
    {
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }
}