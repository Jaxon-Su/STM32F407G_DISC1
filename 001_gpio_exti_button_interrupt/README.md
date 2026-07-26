# STM32F407G-DISC1 HAL Practice

This repository contains my personal STM32 HAL/CubeMX learning examples for the
STM32F407G-DISC1 board.

The goal is to practice STM32 peripheral usage with HAL while keeping example
logic separated from CubeMX-generated initialization code.

## Hardware

- Board: `STM32F407G-DISC1`
- MCU: `STM32F407VGT6`
- IDE: `STM32CubeIDE`
- Configuration tool: `STM32CubeMX`

## Examples

| ID | Topic | Source File | Summary |
| --- | --- | --- | --- |
| 001 | GPIO EXTI button interrupt | `Core/Src/examples/example_001_button_interrupt.c` | Use the onboard `PA0` button interrupt to toggle `PD12`; blink `PD13`, `PD14`, and `PD15` in the main loop. |

Future examples can be added under `Core/Src/examples/` and selected through
`Core/Inc/examples.h`.

Planned practice areas:

- GPIO input/output and EXTI
- UART transmit/receive
- SPI communication
- I2C communication
- Timer, PWM, and basic time-base usage
- ADC polling, interrupt, and DMA practice

## Project Structure

```text
Core/
  Inc/
    examples.h
    main.h

  Src/
    main.c
    examples.c
    Note.txt

    examples/
      example_001_button_interrupt.c

Drivers/
  CMSIS/
  STM32F4xx_HAL_Driver/

STM32F407G_DISC1_Practice.ioc
```

## Design Notes

`main.c` is kept as the system entry point and CubeMX initialization layer.
Example logic is placed in separate files.

General startup flow:

```text
main()
  -> HAL_Init()
  -> SystemClock_Config()
  -> MX_xxx_Init()
  -> Examples_Init()
  -> while (1)
       -> Examples_Loop()
```

Interrupt and callback events are forwarded from HAL callbacks into the active
example through `examples.c`.

```text
HAL callback
  -> Examples_xxx_Callback()
  -> active Examplexxx callback
```

## Example Selection

The active example is selected in `Core/Inc/examples.h`:

```c
#define EXAMPLE_001_BUTTON_INTERRUPT    1
#define ACTIVE_EXAMPLE                  EXAMPLE_001_BUTTON_INTERRUPT
```

`Core/Src/examples.c` dispatches the common interface to the selected example:

```c
Examples_Init();
Examples_Loop();
Examples_EXTI_Callback(GPIO_Pin);
```

## Current Example: 001 GPIO EXTI Button Interrupt

`001` uses the onboard blue button connected to `PA0` as an EXTI interrupt
source. Pressing the button toggles `PD12`.

`PD13`, `PD14`, and `PD15` are toggled every 500 ms from the main loop with
`HAL_Delay(500)`, which makes it easy to observe the difference between
polling-style loop work and interrupt-driven button handling.

Pin setup:

| Pin | Mode | Pull | Trigger / Output |
| --- | --- | --- | --- |
| `PA0` / `BTN_PA0` | `GPIO_EXTI0` | No pull | Rising edge |
| `PD12` / `LED_PD12` | GPIO output | No pull | Toggle in EXTI callback |
| `PD13` / `LED_PD13` | GPIO output | No pull | Toggle in main loop |
| `PD14` / `LED_PD14` | GPIO output | No pull | Toggle in main loop |
| `PD15` / `LED_PD15` | GPIO output | No pull | Toggle in main loop |

Interrupt flow:

```text
PA0 rising edge
  -> EXTI0_IRQHandler()
  -> HAL_GPIO_EXTI_IRQHandler(BTN_PA0_Pin)
  -> HAL_GPIO_EXTI_Callback(GPIO_Pin)
  -> Examples_EXTI_Callback(GPIO_Pin)
  -> Example001_ButtonInterrupt_EXTI_Callback(GPIO_Pin)
  -> HAL_GPIO_TogglePin(LED_PD12_GPIO_Port, LED_PD12_Pin)
```

Main-loop flow:

```text
while (1)
  -> Examples_Loop()
  -> HAL_Delay(500)
  -> Toggle LED_PD13, LED_PD14, LED_PD15
```

## Notes

Learning notes are kept in `Core/Src/Note.txt`.

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

Open the project with STM32CubeIDE and build the `Debug` configuration.

Generated build outputs are intentionally ignored by Git.

