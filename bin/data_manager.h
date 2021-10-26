#ifndef _DATAMANAGER_H
#define _DATAMANAGER_H
          
#include <stdint.h>

#include "lcd.h"
#include "sensor.h"
#include <stdio.h>

//#define DEBUG
/************** Device Setup *************/


/* Public Functions */
int DATAMANAGER_Init(void);
int DATAMANAGER_Read(void);
int DATAMANAGER_Write(void);

#endif