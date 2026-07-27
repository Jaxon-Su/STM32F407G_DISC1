# 003_spi_tx_arduino_button

This example practices SPI2 master transmit with a PA0 button trigger on the STM32F407G-DISC1 board.

## Goal

Press the onboard blue button on PA0, then transmit a length-prefixed text message through SPI2.

Data format:

```text
[length: 1 byte][payload: N bytes]
```

## STM32 Pins

| Signal | STM32 Pin | Purpose |
| --- | --- | --- |
| B1 | PA0 | Button trigger |
| SPI2_NSS | PB12 | Hardware slave select |
| SPI2_SCK | PB13 | SPI clock |
| SPI2_MISO | PB14 | Optional receive line |
| SPI2_MOSI | PB15 | SPI transmit data |
| LD4 | PD12 | Toggle when transmit succeeds |
| LD5 | PD14 | Toggle when transmit fails |

## SPI Settings

| Item | Value |
| --- | --- |
| Mode | Master |
| Direction | 2-line full duplex |
| Data size | 8-bit |
| CPOL | Low |
| CPHA | 1 edge |
| SPI mode | Mode 0 |
| NSS | Hardware output |
| Prescaler | 32 |
| First bit | MSB first |

## Without Arduino

The example can still be built and flashed without Arduino. Pressing PA0 should toggle LD4 if `HAL_SPI_Transmit()` returns `HAL_OK`.

This only confirms that the HAL transmit path runs. To verify real SPI waveforms, use a logic analyzer or oscilloscope on PB12/PB13/PB15.