#ifndef _TIMER_H
#define _TIMER_H

#include <inttypes.h>
#include <stdio.h>
#include "stm32f10x.h"  

void SysTick_Handler(void);

/* Public Functions */
void TIMER_delay_ms(int ms);
void TIMER_delay_us(int us);
uint32_t TIMER_getTime(void);
uint32_t TIMER_pulse(void);
#endif
