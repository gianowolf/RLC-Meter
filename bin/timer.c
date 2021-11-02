#include "timer.h"

void SysTick_Handler(void) {
	SEOS_Schedule();
}

void TIMER_delay_ms(int ms)
{
	ms-=1;
}

void TIMER_delay_us(int us)
{
	us-=1;
}

uint32_t TIMER_getTime(void)
{
	uint32_t time;
	time = 0;
	return time;
}

void TIMER_Init(void)
{
	RCC->APB2ENR |= 0xFC;               /* habilita GPIO clocks */
	RCC->APB2ENR |= (1<<APB2ENR_ADC1);  /* habilita el clock para adc1 */
}
