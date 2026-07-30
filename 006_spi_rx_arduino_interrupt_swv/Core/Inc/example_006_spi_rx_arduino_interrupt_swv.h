/*
 * example_006_spi_rx_arduino_interrupt_swv.h
 *
 *  Created on: 2026-07-30
 *      Author: su622
 */

#ifndef INC_EXAMPLE_006_SPI_RX_ARDUINO_INTERRUPT_SWV_H_
#define INC_EXAMPLE_006_SPI_RX_ARDUINO_INTERRUPT_SWV_H_

#include "main.h"

void Example006_SpiRxArduinoInterruptSwv_Init(void);
void Example006_SpiRxArduinoInterruptSwv_Loop(void);
const char *Example006_SpiRxArduinoInterruptSwv_GetLastMessage(void);
uint8_t Example006_SpiRxArduinoInterruptSwv_IsMessageReady(void);

#endif /* INC_EXAMPLE_006_SPI_RX_ARDUINO_INTERRUPT_SWV_H_ */