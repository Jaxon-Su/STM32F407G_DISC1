/*
 * examples.h
 *
 *  Created on: 2026年7月26日
 *      Author: su622
 */

#ifndef INC_EXAMPLES_H_
#define INC_EXAMPLES_H_

#include "main.h"

/* Example selector */
#define EXAMPLE_001_BUTTON_INTERRUPT    1

#define ACTIVE_EXAMPLE                  EXAMPLE_001_BUTTON_INTERRUPT

/* Common example interface */
void Examples_Init(void);
void Examples_Loop(void);
void Examples_EXTI_Callback(uint16_t GPIO_Pin);

/* Example 001 */
/* GPIO Example */
void Example001_ButtonInterrupt_Init(void);
void Example001_ButtonInterrupt_Loop(void);
void Example001_ButtonInterrupt_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_EXAMPLES_H_ */
