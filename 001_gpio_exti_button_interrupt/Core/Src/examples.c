/*
 * examples.c
 *
 *  Created on: 2026年7月26日
 *      Author: su622
 */


#include "examples.h"

void Examples_Init(void)
{
#if ACTIVE_EXAMPLE == EXAMPLE_001_BUTTON_INTERRUPT
    Example001_ButtonInterrupt_Init();
#endif
}

void Examples_Loop(void)
{
#if ACTIVE_EXAMPLE == EXAMPLE_001_BUTTON_INTERRUPT
    Example001_ButtonInterrupt_Loop();
#endif
}

void Examples_EXTI_Callback(uint16_t GPIO_Pin)
{
#if ACTIVE_EXAMPLE == EXAMPLE_001_BUTTON_INTERRUPT
    Example001_ButtonInterrupt_EXTI_Callback(GPIO_Pin);
#endif
}
