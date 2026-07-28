# 004_spi_cmd_arduino_slave

這個案例把課程中的 SPI command / response demo 搬移成 STM32 HAL 版本。

STM32F407G-DISC1 會作為 `SPI2 Master`，Arduino 或另一個 MCU 需要作為 SPI Slave。Slave 收到 command 後，預期先回傳 `0xF5` 作為 ACK，STM32 收到 ACK 後才繼續送參數或讀資料。

## 目標

練習一個簡單的 SPI command protocol：

| Command | Code | 意義 |
| --- | --- | --- |
| `COMMAND_LED_CTRL` | `0x50` | 要求 slave 控制 LED |
| `COMMAND_SENSOR_READ` | `0x51` | 要求 slave 回傳 analog value |
| `COMMAND_LED_READ` | `0x52` | 要求 slave 回傳 LED 狀態 |
| `COMMAND_PRINT` | `0x53` | 傳送字串給 slave |
| `COMMAND_ID_READ` | `0x54` | 要求 slave 回傳 10-byte ID |

每按一次板上的 `PA0/B1` 藍色按鈕，就執行下一個 command。

```text
第 1 次按：COMMAND_LED_CTRL
第 2 次按：COMMAND_SENSOR_READ
第 3 次按：COMMAND_LED_READ
第 4 次按：COMMAND_PRINT
第 5 次按：COMMAND_ID_READ
第 6 次按：回到 COMMAND_LED_CTRL
```

## 行為說明

每個 command 的共同流程：

```text
1. 等待 PA0/B1 按下
2. debounce
3. SPI2 啟動，PB12/NSS 拉 Low，選中 slave
4. STM32 送出 command code
5. STM32 送 dummy byte 產生 clock
6. STM32 讀取 slave 回傳的 ACK
7. 如果 ACK == 0xF5，就繼續送參數或讀資料
8. 如果 ACK 錯誤，判定本次 command 失敗
9. 關閉 SPI2，PB12/NSS 回 High
10. 用 LED 顯示結果
```

LED 顯示：

| LED | 意義 |
| --- | --- |
| `LD4 / PD12` | 本次 command 成功時 toggle |
| `LD5 / PD14` | 本次 command 失敗時 toggle，常見原因是沒有收到 ACK `0xF5` |
| `LD6 / PD15` | 五個 command 跑完一輪後 toggle |

## STM32 腳位

| Signal | STM32 Pin | 用途 |
| --- | --- | --- |
| B1 | PA0 | 按鈕觸發 |
| SPI2_NSS | PB12 | Hardware slave select |
| SPI2_SCK | PB13 | SPI clock |
| SPI2_MISO | PB14 | 從 slave 接收資料 |
| SPI2_MOSI | PB15 | 傳送資料到 slave |
| LD4 | PD12 | command 成功指示 |
| LD5 | PD14 | command 失敗指示 |
| LD6 | PD15 | command sequence 跑完一輪指示 |

## SPI 設定

| 項目 | 設定 |
| --- | --- |
| Peripheral | SPI2 |
| Mode | Master |
| Direction | 2-line full duplex |
| Data size | 8-bit |
| CPOL | Low |
| CPHA | 1 edge |
| SPI mode | Mode 0 |
| NSS | Hardware output |
| Prescaler | 32 |
| First bit | MSB first |

## 沒有 Arduino 時

這個 firmware 可以 build，也可以燒錄到 STM32F407G-DISC1。

但如果沒有 Arduino 或其他 SPI slave，MISO 不會回傳正確的 ACK `0xF5`，所以按下 PA0 後通常會看到：

```text
LD5 toggle
```

這不代表 SPI 初始化壞掉，而是因為 004 本質上是 master/slave protocol，需要另一端裝置配合。

要完整驗證此案例，需要：

- Arduino 或另一個 MCU 作為 SPI slave
- 或使用邏輯分析儀 / 示波器觀察 `PB12/PB13/PB15/PB14`

## Arduino 電位提醒

STM32F407 是 `3.3V`，Arduino UNO 多數是 `5V`。

STM32 輸出到 Arduino 的方向通常是：

```text
STM32 PB13 SCK  -> Arduino D13 SCK
STM32 PB15 MOSI -> Arduino D11 MOSI
STM32 PB12 NSS  -> Arduino D10 SS
```

比較需要注意的是：

```text
Arduino D12 MISO -> STM32 PB14 MISO
```

如果 Arduino UNO 輸出 5V 到 STM32 PB14，會有風險。做雙向 SPI 時，建議在 `Arduino MISO -> STM32 PB14` 之間加 level shifter 或保護電路。