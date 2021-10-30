#include "seos.h"

/* PRIVATE */
int seos_init(void);

extern uint8_t Flag_RC;
extern uint8_t Flag_LRC;
extern uint8_t Flag_write;

static uint16_t counter_write;

int seos_init(void)
{
	Flag_RC  = 0;
	Flag_LRC = 0;
	Flag_write = 0;
	
	//se inicializan los flags y contadores
	counter_write = OOPH_WRITE;

	/* se configura el sistem tick para interrupir una vez cada 100 ms */
	if (SysTick_Config(SystemCoreClock / 10)){
		/* error handling */
	}

	return 0;
}

int SEOS_Boot(void)
{
	/* Setting-up modules */
	RC_Init(1); /* Inicializa el modulo RC con interrupciones activadas y modo continuo */
	LRC_Init();
	LCD_Init();
	TIMER_Init();
	DATAMANAGER_Init();

	/* Starting OS functions */
	seos_init();

	/* Starting start-up programs */
	DATAMANAGER_Start();
	
	return 0;
}

int SEOS_Schedule(void)
{
	if(++counter_write == OVERF_WRITE)
	{
		Flag_write    = 1;
		counter_write = 0; 
	}
	return 0;
}

int SEOS_Dispatch(void)
{
	//el despachador ejecuta las tareas que estén pendientes y baja los flags
	if(Flag_RC)
	{
		DATAMANAGER_RCread();
		Flag_RC = 0;
	}
	
	if(Flag_LRC)
	{
		DATAMANAGER_LRCread();
		Flag_LRC = 0;
	}
	
	if(Flag_write)
	{
	  DATAMANAGER_Write();
		Flag_write = 0;
	}
	return 0;
}

int SEOS_Sleep(void)
{
	//sleep no se implementa en la simulación
	return 0;
}
