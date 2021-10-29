#ifndef _SEOS_H
#define _SEOS_H

#include <inttypes.h>
#include <stdio.h>
#include "stm32f10x.h"  
#include "data_manager.h"

#define OVERF_WRITE 5  // 0.5 s
#define OOPH_WRITE 0

/* Public Functions */
int SEOS_Boot(void);
int SEOS_Schedule(void);
int SEOS_Dispatch(void);
int SEOS_Sleep(void);

#endif
