# STM32F407G-DISC1 HAL Practice

This repository contains my personal STM32 HAL/CubeMX learning examples for the
STM32F407G-DISC1 board.

Each example is kept as an independent STM32CubeIDE/CubeMX project. This keeps
each `.ioc` file matched to the exact peripheral, pin, clock, and interrupt
configuration used by that example.

## Hardware

- Board: `STM32F407G-DISC1`
- MCU: `STM32F407VGT6`
- IDE: `STM32CubeIDE`
- Configuration tool: `STM32CubeMX`

## Examples

| ID | Project | Topic | Summary |
| --- | --- | --- | --- |
| 001 | `001_gpio_exti_button_interrupt` | GPIO EXTI button interrupt | Use onboard `PA0` button interrupt to toggle `PD12`; blink `PD13`, `PD14`, and `PD15` from the main loop. |
| 002 | `002_spi_tx_testing` | SPI2 transmit testing | Use `SPI2` master mode to transmit `"Hello world"` on `PB15/MOSI` with clock on `PB13/SCK`. |

## Repository Layout

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

  README.md
  NOTICE.md
```

## Why Each Example Has Its Own `.ioc`

STM32CubeMX configuration is part of the example. Different examples often need
different pin assignments, alternate functions, clocks, interrupts, DMA, or
middleware settings.

Keeping one project per example avoids mixing unrelated peripheral setup into a
single large `.ioc` file.

Use this rule:

```text
Different peripheral / pin / clock setup -> separate CubeMX project
Same setup but different application logic -> same project can switch examples
```

## Planned Practice Areas

- GPIO input/output and EXTI
- UART transmit/receive
- SPI communication
- I2C communication
- Timer, PWM, and basic time-base usage
- ADC polling, interrupt, and DMA practice

## Licensing And Attribution

This repository is for personal learning and reference.

Custom example code and notes are written for learning practice. STM32 HAL,
CMSIS, startup code, linker scripts, and CubeMX-generated files are provided by
STMicroelectronics and Arm through STM32CubeMX/STM32CubeIDE and remain under
their original licenses.

No original Udemy course source code, slides, or paid materials are included.
The implementation is based on personal learning, STM32CubeMX-generated setup,
and hands-on experimentation.

See `NOTICE.md` for third-party component notes.

## Build

Open the root folder as the STM32CubeIDE workspace:

```text
C:\Users\su622\Desktop\STM32F407G_DISC1
```

Import or open each example project from the workspace, then build the selected
example project with the `Debug` configuration.

Generated build outputs are intentionally ignored by Git.

