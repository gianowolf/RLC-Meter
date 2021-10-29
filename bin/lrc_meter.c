#include "lrc_meter.h"


volatile uint32_t result;

void LRC_Init(void)
{
	result = 0;
}

void LRC_StartConvertion(void)
{
	
}

uint32_t LRC_GetInductance(void)
{
	return result;
}
