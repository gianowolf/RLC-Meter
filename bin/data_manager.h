#ifndef _DATAMANAGER_H
#define _DATAMANAGER_H
          
#include <stdint.h>
#include <stdio.h>

#include "lcd.h"
#include "rc_meter.h"
#include "lrc_meter.h"

/* Public Functions */
void DATAMANAGER_Init(void);
void DATAMANAGER_Start(void);
void DATAMANAGER_RCread(void);
void DATAMANAGER_LRCread(void);
void DATAMANAGER_Write(void);

#endif
