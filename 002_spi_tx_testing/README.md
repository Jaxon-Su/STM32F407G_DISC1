# 002 SPI TX Testing

This example ports a register-level SPI2 transmit demo to STM32 HAL.

## Goal

Verify that `SPI2` can transmit bytes through MOSI and generate SCK.

## SPI2 Pins

| Pin | Function |
| --- | --- |
| `PB13` | `SPI2_SCK` |
| `PB14` | `SPI2_MISO` |
| `PB15` | `SPI2_MOSI` |

The original demo only needs SCK and MOSI for transmit testing, but this project
keeps SPI2 in full-duplex master mode, so MISO is also configured.

## SPI Settings

| Setting | Value |
| --- | --- |
| Mode | Master |
| Direction | 2-line full duplex |
| Data size | 8-bit |
| First bit | MSB first |
| Clock polarity | High |
| Clock phase | 1 edge |
| NSS | Software |
| Prescaler | 2 |

In this CubeMX configuration the calculated SPI2 baud rate is about
`3.125 Mbit/s`.

## HAL Mapping

```text
SPI2_GPIOInits()
  -> CubeMX-generated HAL_SPI_MspInit()

SPI2_Inits()
  -> CubeMX-generated MX_SPI2_Init()

SPI_SendData()
  -> HAL_SPI_Transmit()
```

## Runtime Behavior

`Example002_SpiTxTesting_Loop()` sends:

```text
Hello world
```

through `SPI2` once per second. `LD4` toggles after a successful transmit.

Use a logic analyzer or oscilloscope on:

```text
PB13 = SCK
PB15 = MOSI
GND  = GND
```

