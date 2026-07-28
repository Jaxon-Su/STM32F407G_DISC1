# 005_spi_rx_arduino_interrupt

本案例把課程的 SPI interrupt receive demo 搬成 STM32 HAL / CubeMX 寫法。

STM32F407G-DISC1 作為 SPI master，Arduino 或其他 SPI slave 作為資料來源。Slave 有資料時先透過 `PD6` 通知 STM32，STM32 再用 `SPI2` 逐 byte 讀資料。

## 練習目標

- 使用 `SPI2` full-duplex master
- 使用 hardware NSS output
- 使用 `HAL_SPI_TransmitReceive_IT()` 做 interrupt mode SPI 收發
- 使用 `PD6` external interrupt 作為 slave data available 訊號
- 理解 master read 時仍需送 dummy byte 產生 SCK
- 用 callback 串起 EXTI 與 SPI interrupt 流程

## 硬體設定

| 功能 | STM32 腳位 | CubeMX 設定 |
| --- | --- | --- |
| SPI2_NSS | PB12 | Alternate Function AF5 / Hardware NSS Output |
| SPI2_SCK | PB13 | Alternate Function AF5 |
| SPI2_MISO | PB14 | Alternate Function AF5 |
| SPI2_MOSI | PB15 | Alternate Function AF5 |
| Slave data available | PD6 | GPIO_EXTI6 / falling edge / pull-up |
| Success LED | PD12 / LD4 | GPIO output push-pull |
| Interrupt indicator | PD13 / LD3 | GPIO output push-pull |
| Error LED | PD14 / LD5 | GPIO output push-pull |
| Busy LED | PD15 / LD6 | GPIO output push-pull |

## SPI 設定

| 項目 | 設定 |
| --- | --- |
| Mode | Full-Duplex Master |
| NSS | Hardware NSS Output |
| Data Size | 8 bits |
| First Bit | MSB First |
| CPOL | Low |
| CPHA | 1 Edge |
| Prescaler | 32 |

## 行為說明

1. Slave 準備好一段字串資料。
2. Slave 把 `PD6` 對應的通知線拉低。
3. STM32 進入 `EXTI9_5_IRQHandler()`。
4. HAL 呼叫 `HAL_GPIO_EXTI_Callback()`。
5. 範例程式設定 `s_dataAvailable = 1`。
6. `Example005_SpiRxArduinoInterrupt_Loop()` 開始 SPI 接收。
7. STM32 每次送出 dummy byte `0xFF`，讓 SPI master 產生 clock。
8. Slave 同步從 MISO 回傳 1 byte。
9. `HAL_SPI_TxRxCpltCallback()` 把收到的 byte 存到 `s_rcvBuff`。
10. 收到 `\0` 或 buffer 滿時停止接收。

## LED 狀態

| LED | 意義 |
| --- | --- |
| LD3 / PD13 | 收到 PD6 external interrupt 時 toggle |
| LD4 / PD12 | 成功完成一次接收時 toggle |
| LD5 / PD14 | SPI timeout 或 HAL error 時 toggle |
| LD6 / PD15 | 接收期間亮起，接收結束熄滅 |

## 觀察資料

目前沒有把 `printf()` retarget 到 SWV / UART，因此接收到的字串主要可在 debug 時觀察：

```c
s_rcvBuff
```

也可以呼叫：

```c
Example005_SpiRxArduinoInterrupt_GetLastMessage();
Example005_SpiRxArduinoInterrupt_IsMessageReady();
```

## 沒有 Arduino 時

這個案例需要 Arduino 或其他 SPI slave 才能看到完整行為。

沒有 slave 時：

- `PD6` 不會自動觸發，STM32 會等待 data available。
- 即使手動觸發 `PD6`，`MISO` 沒有有效資料，收到的 byte 可能是 floating value。
- 若一直沒有收到 `\0`，程式會到 buffer 上限才停止。

## 電壓提醒

STM32F407 是 3.3V 邏輯。Arduino UNO 多數是 5V 邏輯。

若 Arduino 的 `MISO` 或 data available interrupt 線輸出 5V 到 STM32，可能會傷到 STM32。實際接線時建議使用 level shifter，或確認 Arduino 輸出已限制在 3.3V。