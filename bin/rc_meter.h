#ifndef _RC_H
#define _RC_H

#include "stm32f10x.h"         
#include <stdint.h>
#include <stdio.h>

#define EOCIE 5
#define ADON 0
#define CONT 1
#define APB2ENR_ADC1 9

void ADC1_2_IRQHandler(void);

/* Public Functions */
void     RC_Init(int countinuously, int interrputions);
void     RC_StartConvertion(void);
uint32_t RC_GetCapacitance(void);

/* Private Functions */
void enableInterrupts(void);
void disableInterrupts(void);
//uint32_t transform(void);

#endif
