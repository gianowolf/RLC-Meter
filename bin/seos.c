#include "seos.h"
#include "sensor.h"
#include "data_manager.h"
#include "lcd.h"

/* PUBLIC FLAGS */
volatile uint16_t Flag_read;
volatile uint16_t Flag_write;

/* PRIVATE */
int seos_init(void);

static uint16_t counter_read;
static uint16_t counter_write;

void SysTick_Handler(void) {
	SEOS_Schedule();
}

int seos_init(void)
{
	Flag_read  = 0;
	Flag_write = 0;
	//se inicializan los flags y contadores
	counter_write = OOPH_WRITE;
	counter_read  = OOPH_READ;

	//se configura el sistem tick para interrupir una vez cada 100 ms
	if (SysTick_Config(SystemCoreClock / 10)){
		//error handling
	}

	return 0;
}

int SEOS_Boot(void)
{
	//inicializa todos los módulos
	LCD_init();
	SENSOR_Init();
	DATAMANAGER_Init();
	seos_init();
	return 0;
}

int SEOS_Schedule(void)
{
	//el planificador levanta el flag de las tareas que se tengan que despachar
	if(++counter_read == OVERF_READ)
	{
		Flag_read   = 1; 
		counter_read = 0;
	}
#ifndef DEBUG
	if(++counter_write == OVERF_WRITE)
	{
		Flag_write    = 1;
		counter_write = 0; 
	}
#endif
	return 0;
}

int SEOS_Dispatch(void)
{
	//el despachador ejecuta las tareas que estén pendientes y baja los flags
	if(Flag_read)
	{
		DATAMANAGER_Read();
		Flag_read = 0;
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