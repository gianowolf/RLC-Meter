#include "data_manager.h"

static uint32_t lrc, rc;

char msg_capacitance[] = "Capacitance";
char msg_meter[] = "Meter";
char msg_value[] = "00.000 uF";

void DATAMANAGER_Start(void);
void DATAMANAGER_RCread(void);
void DATAMANAGER_LRCread(void);
void DATAMANAGER_Write(void);


void DATAMANAGER_Init(void)
{
	lrc = 0;
	rc = 0;
}

void DATAMANAGER_Start(void)
{
	LRC_StartConvertion();
	RC_StartConvertion(1);
}

void DATAMANAGER_RCread(void)
{
	rc = RC_GetCapacitance();
}

void DATAMANAGER_LRCread(void)
{
	lrc = LRC_GetInductance();
}

void DATAMANAGER_Write(void)
{
	printf("\n%d ",RC_GetCapacitance());
}
