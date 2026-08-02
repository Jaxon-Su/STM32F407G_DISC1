# 015 ADC Voltage / Current

這個案例示範最基本的 ADC polling 讀值。

## 目標

使用 `ADC1_IN1 / PA1` 讀取外部類比電壓，並把 12-bit ADC raw value 轉成：

- `g_adc_raw`
- `g_adc_voltage_mv`
- `g_adc_current_ma`

這三個變數刻意設成 `volatile`，方便在 STM32CubeIDE debug/watch 視窗觀察。

## 腳位

| 功能 | STM32 腳位 | 說明 |
| --- | --- | --- |
| ADC input | `PA1 / ADC1_IN1` | 外部 0-3.3V 類比輸入 |
| Activity LED | `PD13 / LD3` | 每次 sample 成功 toggle |
| Threshold LED | `PD12 / LD4` | 電壓 >= 1.65V 時亮 |
| Error LED | `PD14 / LD5` | ADC error 時 toggle |

## 接線注意

`PA1` 輸入不可超過 `3.3V`，也不可低於 GND。

如果量測高於 3.3V 的電壓，要先用電阻分壓。  
如果量測電流，可以用 shunt resistor 或電流感測器，並依照實際 shunt / gain 修改：

```c
#define CURRENT_SHUNT_MOHM     (100UL)
#define CURRENT_SENSE_GAIN     (1UL)
```

## HAL API

核心流程：

```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 10);
raw = HAL_ADC_GetValue(&hadc1);
HAL_ADC_Stop(&hadc1);
```