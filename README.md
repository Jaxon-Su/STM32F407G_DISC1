# STM32F407G-DISC1 HAL 練習專案

這個 repository 是個人的 STM32 HAL / CubeMX 學習筆記與範例集合，目標板為 `STM32F407G-DISC1`。

每個案例都保留成獨立的 STM32CubeIDE / CubeMX 專案。這樣每個 `.ioc` 都能對應自己的腳位、外設、時鐘、中斷與 middleware 設定，避免不同練習互相污染。

## 硬體與工具

- 開發板：`STM32F407G-DISC1`
- MCU：`STM32F407VGT6`
- IDE：`STM32CubeIDE`
- 設定工具：`STM32CubeMX`

## 案例列表

| ID | 專案 | 主題 | 說明 |
| --- | --- | --- | --- |
| 001 | `001_gpio_exti_button_interrupt` | GPIO EXTI 按鈕中斷 | 使用板上 `PA0` 藍色按鈕中斷切換 `PD12`，並在 main loop 中閃爍 `PD13`、`PD14`、`PD15`。 |
| 002 | `002_spi_tx_testing` | SPI2 傳送測試 | 使用 `SPI2` master mode，透過 `PB15/MOSI` 送出 `"Hello world"`，並從 `PB13/SCK` 產生 clock。 |
| 003 | `003_spi_tx_arduino_button` | SPI2 按鈕觸發傳送 | 按下 `PA0` 後，透過 `SPI2` 送出 `[length][payload]`，使用 hardware `PB12/NSS`、`PB13/SCK`、`PB15/MOSI`。 |
| 004 | `004_spi_cmd_arduino_slave` | SPI command/response 協定 | 按下 `PA0` 後逐步執行 SPI command，使用 `HAL_SPI_TransmitReceive()` 驗證 slave ACK 與回傳資料。 |
| 005 | `005_spi_rx_arduino_interrupt` | SPI interrupt receive | Slave 透過 `PD6` external interrupt 通知 STM32 有資料，STM32 使用 `HAL_SPI_TransmitReceive_IT()` 逐 byte 讀到 `\0` 結尾。 |
| 006 | `006_spi_rx_arduino_interrupt_swv` | SPI RX interrupt + SWV | 在 005 的接收流程上加入 `printf()` retarget 到 SWV/ITM，收到 slave 字串後印到 ITM Data Console。 |

## 資料夾結構

```text
STM32F407G_DISC1/
  001_gpio_exti_button_interrupt/
    Core/
    Drivers/
    STM32F407G_DISC1_Practice.ioc
    README.md

  002_spi_tx_testing/
    Core/
    Drivers/
    002_spi_tx_testing.ioc
    README.md

  003_spi_tx_arduino_button/
    Core/
    Drivers/
    003_spi_tx_arduino_button.ioc
    README.md

  004_spi_cmd_arduino_slave/
    Core/
    Drivers/
    004_spi_cmd_arduino_slave.ioc
    README.md

  005_spi_rx_arduino_interrupt/
    Core/
    Drivers/
    005_spi_rx_arduino_interrupt.ioc
    README.md

  006_spi_rx_arduino_interrupt_swv/
    Core/
    Drivers/
    006_spi_rx_arduino_interrupt_swv.ioc
    README.md

  README.md
  NOTICE.md
```

## 為什麼每個案例都有自己的 `.ioc`

STM32CubeMX 的設定本身就是案例的一部分。不同練習可能需要不同腳位、alternate function、clock tree、中斷、DMA 或 middleware 設定。

因此這個 repository 採用：

```text
不同外設 / 腳位 / 時鐘設定 -> 建立獨立 CubeMX 專案
相同硬體設定但不同應用邏輯 -> 可在同一專案內切換 example
```

這樣回頭複習時，可以直接打開某個案例的 `.ioc`，看到當時的完整硬體設定。

## 預計練習方向

- GPIO input/output 與 EXTI
- UART transmit/receive
- SPI communication
- I2C communication
- Timer、PWM、基本 time base
- ADC polling、interrupt、DMA

## 授權與出處

本 repository 用於個人學習與參考。

自訂範例程式與筆記是基於個人學習、STM32CubeMX 產生的設定，以及實際操作整理而成。STM32 HAL、CMSIS、startup code、linker scripts 與 CubeMX 產生檔案由 STMicroelectronics / Arm 提供，仍保留原始授權與 copyright。

本 repository 不包含付費課程原始 source code、投影片或重新散布的課程材料。

第三方與 vendor 元件說明見 `NOTICE.md`。

## Build

將下列路徑作為 STM32CubeIDE workspace：

```text
C:\Users\su622\Desktop\STM32F407G_DISC1
```

在 STM32CubeIDE 中 import 或 open 指定案例專案，再使用 `Debug` configuration build。

產生的 build output 已由 `.gitignore` 排除。