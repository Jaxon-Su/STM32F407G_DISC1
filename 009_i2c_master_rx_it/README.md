# 009 I2C Master RX Interrupt

來源概念：$src

## 目標

I2C master 使用 interrupt 依序送 0x51、讀長度、送 0x52、讀 payload。

## 硬體 / 腳位

PB6/PB7 I2C1, slave 0x68

## 狀態指示

- LD4 / PD12：成功或主要活動 toggle
- LD5 / PD14：錯誤 toggle
- LD3 / PD13：收到 command、按鈕接受或中間事件 toggle

## 搬移說明

這是 HAL / CubeMX 版本，不包含課程原始 driver-layer source code。保留課程流程的學習概念，實作改用 STM32 HAL API 和 CubeMX 產生專案結構。