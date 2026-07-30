# 006_spi_rx_arduino_interrupt_swv

本案例把課程的 SPI interrupt receive demo 搬成 STM32 HAL / CubeMX 寫法，並使用 SWV/ITM 輸出 `printf()`。

STM32F407G-DISC1 作為 SPI master，Arduino 或其他 SPI slave 作為資料來源。Slave 有資料時先透過 `PD6` 通知 STM32，STM32 再用 `SPI2` 逐 byte 讀資料，收到 `\0` 後用 `printf()` 印到 SWV ITM Data Console。

## 練習目標

- 使用 `SPI2` full-duplex master
- 使用 hardware NSS output
- 使用 `HAL_SPI_TransmitReceive_IT()` 做 interrupt mode SPI 收發
- 使用 `PD6` external interrupt 作為 slave data available 訊號
- 使用 `ITM_SendChar()` retarget `printf()` 到 SWV
- 理解 EXTI interrupt、SPI interrupt、HAL callback 的分工

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
4. HAL 清 EXTI pending flag，然後呼叫 `HAL_GPIO_EXTI_Callback()`。
5. 範例程式設定 `s_dataAvailable = 1`。
6. `Example006_SpiRxArduinoInterruptSwv_Loop()` 開始 SPI 接收。
7. STM32 每次送出 dummy byte `0xFF`，讓 SPI master 產生 clock。
8. Slave 同步從 MISO 回傳 1 byte。
9. `HAL_SPI_TxRxCpltCallback()` 把收到的 byte 存到 `s_rcvBuff`。
10. 收到 `\0` 或 buffer 滿時停止接收。
11. 成功時執行 `printf("Rcvd data = %s\r\n", s_rcvBuff)`。

## SWV printf

本案例在 example source 內實作：

```c
int __io_putchar(int ch)
{
    ITM_SendChar((uint32_t)ch);
    return ch;
}
```

`syscalls.c` 裡的 `_write()` 會呼叫 `__io_putchar()`，所以 `printf()` 最後會走到 ITM stimulus port。

要在 STM32CubeIDE 看到輸出，需要開啟 SWV ITM Data Console，並啟用 stimulus port 0。

## LED 狀態

| LED | 意義 |
| --- | --- |
| LD3 / PD13 | 收到 PD6 external interrupt 時 toggle |
| LD4 / PD12 | 成功完成一次接收時 toggle |
| LD5 / PD14 | SPI timeout 或 HAL error 時 toggle |
| LD6 / PD15 | 接收期間亮起，接收結束熄滅 |

## 沒有 Arduino 時

這個案例需要 Arduino 或其他 SPI slave 才能看到完整行為。
沒有 slave 時，`PD6` 不會自動觸發；即使手動觸發 `PD6`，`MISO` 沒有有效資料，收到的 byte 也可能是 floating value。

## 電壓提醒

STM32F407 是 3.3V 邏輯。Arduino UNO 多數是 5V 邏輯。

若 Arduino 的 `MISO` 或 data available interrupt 線輸出 5V 到 STM32，可能會傷到 STM32。實際接線時建議使用 level shifter，或確認 Arduino 輸出已限制在 3.3V。