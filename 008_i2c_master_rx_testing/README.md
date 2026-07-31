# 008 I2C Master RX Testing

這個案例把課程的 I2C master receive demo 搬成 STM32 HAL / CubeMX 版本。

## 目標

STM32F407G-DISC1 作為 I2C master，按下 `PA0/B1` 後，向 slave address `0x68` 讀取資料。

流程：

1. 送 command `0x51`。
2. 讀取 1 byte 的 payload 長度。
3. 送 command `0x52`。
4. 依照剛剛讀到的長度讀取 payload。

## 腳位

| 功能 | STM32 腳位 | 說明 |
| --- | --- | --- |
| I2C1_SCL | `PB6` | I2C clock |
| I2C1_SDA | `PB7` | I2C data |
| Button | `PA0` | 板上藍色 B1 button |
| Success LED | `PD12 / LD4` | 讀取成功 toggle |
| Activity LED | `PD13 / LD3` | 接受一次按鈕後 toggle |
| Error LED | `PD14 / LD5` | 讀取失敗或沒有 ACK 時 toggle |

## I2C 設定

- Mode：I2C master receive flow
- Speed：Standard mode `100 kHz`
- Own address：`0x61`
- Slave address：`0x68` 7-bit address
- GPIO：`PB6/PB7` 使用 `GPIO_MODE_AF_OD`
- Alternate function：`GPIO_AF4_I2C1`

## HAL API 對照

```c
HAL_I2C_Master_Transmit(&hi2c1, 0x68 << 1, &command, 1, 100);
HAL_I2C_Master_Receive(&hi2c1, 0x68 << 1, buffer, len, 100);
```

`0x68` 是 7-bit slave address；HAL I2C API 要傳入 `0x68 << 1`。

## 硬體注意

I2C 是 open-drain bus，`SCL` 和 `SDA` 需要 pull-up。

本案例用 STM32 內部 pull-up 方便練習；實際接外部 I2C 模組時，建議使用外部 `3.3k` 或 `4.7k` pull-up 到 `3.3V`。

如果模組已經 pull-up 到 `5V`，不要直接接 STM32F407 的 I2C 腳位，請改成 3.3V pull-up 或使用 level shifter。

## 沒有 slave 時

如果目前沒有接 I2C slave，或 slave address 不是 `0x68`，按下 B1 後會因為收不到 ACK 而失敗，`LD5` 會 toggle。這是合理現象。