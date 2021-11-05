#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>

#define C_CHANNEL 3 /* Timer 2, Channel n */
#define C_PERIOD  5/* [ms] */

#define L_CHANNEL 2 /* Timer 2, Channel n */
#define L_PERIOD  5/* [ms] */

void USART_Init(void);
void USART_SendByte(unsigned char c);
void USART_SendStr(char *str);
void USART_SendInt(unsigned int i);

void RCC_Init(void);

void GPIO_Init(void);

void     TIM2_Init(void);
void     TIM2_StartCountingUp(void);
uint16_t TIM2_PeriodToRCC_ms(uint16_t period_in_ms); // privada cuando se pase al programa principal
void     TIM2_SetupReadInputFreq(void);
void     TIM2_ReadFreq(void);

void TIM3_Init(void);
void TIM3_Delay_us(uint16_t t_us);
void TIM3_Delay_ms(uint16_t t_ms);

void ADC1_Init(void);
void ADC1_StartConversion(void);
void ADC1_2_IRQHandler(void);

static uint16_t t  = 0;
static uint16_t t0 = 0;
static uint16_t incapResult = 0;

static uint32_t adcResult;

int main(void)
{
	uint16_t frequency;
	uint32_t voltage;
	
	RCC_Init();
	GPIO_Init();
	USART_Init();
	TIM2_Init();
	TIM3_Init();
	ADC1_Init();
	
	/* Input Signals */
	//TIM2_SetupReadInputFreq();

	ADC1_StartConversion();
	TIM2_StartCountingUp();
	
	while(1)
	{
		TIM3_Delay_ms(5000);
		//TIM2_ReadFreq();
		
		voltage = adcResult;
		frequency = incapResult;
		//printf("\n\nVOLTAGE: %d",voltage);
		//printf("\nFREQUENCY: %d",frequency);
		USART_SendStr("Voltage: ");
		USART_SendInt(adcResult);
	}
}



void RCC_Init(void)
{
	RCC->APB2ENR |= (0xFC); 					/* enable GPIO clocks */
	RCC->APB2ENR |= (1<<9); 					/* enable ADC1 clock  */
	RCC->APB1ENR |= (1<<0); 					/* enable TIM2 clock  */
	RCC->APB1ENR |= (1<<1);           /* enable TIM3 clock  */
}

void GPIO_Init(void)
{
	GPIOA->CRL = 0x44480BB4; 		 /* (C & L outputs)PA1(ch2)=B, PA2(ch3)=B -> Alternate func output | (C) PA3=8 (ADC3) | (L) PA4=0 -> Analog Input */
	GPIOA->CRH = 0x444448B4;     /*(USART) RX1=input with p-up, TX1=alt func outp */
	GPIOA->ODR |= (1<<10);       /*(USART) pull-up PA10 */
	GPIOA->ODR |= (1<<2);
}




void TIM2_Init(void)
{
	/* Output Compare Ch2 & Ch3, Input Capture Ch4*/
		TIM2->CCR2 = 1000;
	  TIM2->CCR3 = 4000;
		TIM2->CCER =  (0x1<<4) | (0x1<<8) | (0x1<<12); /* CCnP=0 CC3E=1 CC2E=1 CC4E=1-> Enable Compare/Capture outputs Ch2 and Ch3*/
		TIM2->CCMR1 = 0x3000; 						/* toggle channel 2 */
		TIM2->CCMR2 = 0x1030; 						/* toggle channel 3, rising edge no division channel 4*/
		TIM2->PSC = 7200-1; 							/* clk_freq=72 MHz, Prescaler = 7200 -> Ttimer = 0.1 ms */
		TIM2->ARR = 10000-1;
}

uint16_t TIM2_PeriodToRCC_ms(uint16_t period_in_ms)
{
	return period_in_ms*5;
}


void TIM2_StartCountingUp(void)
{
		TIM2->CR1 = 1; 										/* START COUNTING UP */
}


void TIM2_ReadFreq(void)
{
	t = TIM2->CCR4; /* read captured value */
	
	incapResult = t - t0;
	t0 = t;
}

void TIM3_Init(void)
{
	TIM3->PSC = 72-1;
}
void TIM3_Delay_us(uint16_t t_us)
{
	TIM3->ARR = t_us - 1;
	TIM3->SR = 0; /* clear UIF flag */
	TIM3->CR1 = 1; /* up counting */
	while( (TIM3->SR & 1) == 0 ); /*wait until UIF flag is set */
	TIM3->CR1 = 0; /* stop counting */
}

void TIM3_Delay_ms(uint16_t t_ms)
{
	TIM3_Delay_us(t_ms*1000);
}




void ADC1_Init(void)
{
	ADC1->CR2 = 1; /* ADON=1 (power up adc1) */
	ADC1->SMPR2 = (7<<3); /* SMP1 = 111 (239.5 ADC clock cycles) */
	
	TIM3_Delay_us(1);
	
	ADC1->CR1 = (1<<5); /* EOCIE = 1 */
	
	ADC1->SQR3 = 1; /* choose channel 1 as the input */
	ADC1->CR2 |= 1<<1; /* CONT = 1 (Convert continuously) */
}

void ADC1_StartConversion(void)
{
	ADC1->CR2 |= 1<<0; /* ADCON = 1 (Start conversion) */
	NVIC_EnableIRQ(ADC1_2_IRQn); /* enable ADC1_2 interrupt */ 
}

void ADC1_2_IRQHandler(void)
{
	adcResult = ADC1->DR; 
}


void USART_Init(void)
{
	USART1->CR1 = 0x200C;
	USART1->BRR = 7500; // 72Mhz/9600bps = 7500
}

void USART_SendByte(unsigned char c)
{
	while((USART1->SR&(1<<6))==0);
	USART1->DR = c;
}

void USART_SendStr(char *str)
{
	while(*str != 0)
	{
		USART_SendByte(*str);
		str++;
	}
}

void USART_SendInt(unsigned int i)
{
	char str[10];  
	sprintf(str,"%d",i); 
	
  USART_SendStr(str);
}



