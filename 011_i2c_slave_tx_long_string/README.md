# 011 I2C Slave TX Long String

來源概念：$src

## 目標

STM32 作為 I2C slave 0x68，回傳 4-byte 長度與長字串 payload。

## 硬體 / 腳位

PB6/PB7 I2C1 slave

## 狀態指示

- LD4 / PD12：成功或主要活動 toggle
- LD5 / PD14：錯誤 toggle
- LD3 / PD13：收到 command、按鈕接受或中間事件 toggle

## 搬移說明

這是 HAL / CubeMX 版本，不包含課程原始 driver-layer source code。保留課程流程的學習概念，實作改用 STM32 HAL API 和 CubeMX 產生專案結構。