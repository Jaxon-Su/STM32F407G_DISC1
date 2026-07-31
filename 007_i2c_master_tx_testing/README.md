# 007 I2C Master TX Testing

這個案例把課程的 `010i2c_master_tx_testing.c` 搬成 STM32 HAL / CubeMX 版本。

## 目標

使用 `STM32F407G-DISC1` 作為 I2C master，按下板上 `PA0/B1` 後，透過 `I2C1` 對 slave address `0x68` 傳送字串：

```text
We are testing I2C master Tx
```

## 腳位

| 功能 | STM32 腳位 | 說明 |
| --- | --- | --- |
| I2C1_SCL | `PB6` | I2C clock |
| I2C1_SDA | `PB9` | I2C data |
| Button | `PA0` | 板上藍色 B1 button |
| Success LED | `PD12 / LD4` | 傳送成功 toggle |
| Activity LED | `PD13 / LD3` | 接受一次按鈕後 toggle |
| Error LED | `PD14 / LD5` | 傳送失敗或沒有 ACK 時 toggle |

## I2C 設定

- Mode：I2C master transmit
- Speed：Standard mode `100 kHz`
- Own address：`0x61`
- Slave address：`0x68` 7-bit address
- GPIO：`PB6/PB9` 使用 `GPIO_MODE_AF_OD`
- Alternate function：`GPIO_AF4_I2C1`

HAL 傳送時使用：

```c
HAL_I2C_Master_Transmit(&hi2c1,
                        (0x68U << 1),
                        data,
                        size,
                        timeout);
```

`0x68` 是 7-bit address，HAL API 的 `DevAddress` 要傳入左移 1 bit 後的值。

## 硬體注意

I2C 是 open-drain bus，`SCL` 和 `SDA` 需要 pull-up。

本案例用 STM32 內部 pull-up 方便練習；實際接外部 I2C 模組時，建議使用外部 `3.3k` 或 `4.7k` pull-up 到 `3.3V`。

如果模組已經 pull-up 到 `5V`，不要直接接 STM32F407 的 I2C 腳位，請改成 3.3V pull-up 或使用 level shifter。

## 沒有 slave 時

如果目前沒有接 I2C slave，或 slave address 不是 `0x68`，按下 B1 後會因為收不到 ACK 而失敗，`LD5` 會 toggle。這是合理現象。