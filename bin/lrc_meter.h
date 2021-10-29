#ifndef _LRC_H
#define _LRC_H

#include "stm32f10x.h"  
#include <stdint.h>
#include <stdio.h>

/* Public Functions */
void LRC_Init(void);
void LRC_StartConvertion(void);
uint32_t LRC_GetInductance(void);

/* Private Functions */

#endif
