#include "timer.h"


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
