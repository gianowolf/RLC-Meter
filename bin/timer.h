#ifndef _TIMER_H
#define _TIMER_H

#include <inttypes.h>
#include <stdio.h>
#include "stm32f10x.h"  

/* Public Functions */
void TIMER_delay_ms(int ms);
void TIMER_delay_us(int us);
uint32_t TIMER_getTime(void);

#endif
