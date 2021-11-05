#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"               
#include <stdint.h>
#include <stdio.h>

#define LCDPORT GPIOB //puerto al que se conecta el lcd
#define LCDIOCLK RCC_APB2ENR_IOPBEN

#define LCD_D0	8	//pin conectado a LCD D0
#define LCD_D1	9	//pin conectado a LCD D1
#define LCD_D2	10	//pin conectado a LCD D2
#define LCD_D3	11	//pin conectado a LCD D3
#define LCD_D4	12	//pin conectado a LCD D4
#define LCD_D5	13	//pin conectado a LCD D5
#define LCD_D6	14	//pin conectado a LCD D6
#define LCD_D7	15	//pin conectado a LCD D7
#define LCD_RW	6 //pin conectado a LCD R/W
#define LCD_RS	7 //pin conectado a LCD RS
#define LCD_E	5	//pin conectado a LCD E

void LCD_Init(void);
void LCD_sendCommand(uint8_t);
void LCD_goToXY(uint8_t, uint8_t);
void LCD_sendString(uint8_t*, uint8_t);
void LCD_clear(void);
void LCD_sendChar(uint8_t c);


void LCD_sendInt(unsigned int i);
void LCD_sendStr(char *str);
void LCD_sendByte(unsigned char c);





#endif
