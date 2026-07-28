/*
 * example_005_spi_rx_arduino_interrupt.h
 *
 *  Created on: 2026-07-28
 *      Author: su622
 */

#ifndef INC_EXAMPLE_005_SPI_RX_ARDUINO_INTERRUPT_H_
#define INC_EXAMPLE_005_SPI_RX_ARDUINO_INTERRUPT_H_

#include "main.h"

void Example005_SpiRxArduinoInterrupt_Init(void);
void Example005_SpiRxArduinoInterrupt_Loop(void);
const char *Example005_SpiRxArduinoInterrupt_GetLastMessage(void);
uint8_t Example005_SpiRxArduinoInterrupt_IsMessageReady(void);

#endif /* INC_EXAMPLE_005_SPI_RX_ARDUINO_INTERRUPT_H_ */