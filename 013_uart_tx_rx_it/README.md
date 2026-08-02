# 013 UART TX RX Interrupt

來源概念：$src

## 目標

按下 PA0 後傳送三種訊息之一，並用 UART receive interrupt 等待同長度回覆。

## 硬體 / 腳位

USART2 PA2 TX / PA3 RX, 115200 8N1

## 狀態指示

- LD4 / PD12：成功或主要活動 toggle
- LD5 / PD14：錯誤 toggle
- LD3 / PD13：收到 command、按鈕接受或中間事件 toggle

## 搬移說明

這是 HAL / CubeMX 版本，不包含課程原始 driver-layer source code。保留課程流程的學習概念，實作改用 STM32 HAL API 和 CubeMX 產生專案結構。