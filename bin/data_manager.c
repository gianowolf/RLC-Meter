#include "data_manager.h"

/* Private Functions */
int store(uint16_t value);
uint16_t comp2(uint16_t);


static uint16_t aux;
static uint8_t count;
static volatile int16_t tempe;
//La variable de temperatura es signed para poder sumar y restar hasta valores negativos

static volatile uint8_t printable[] = {'T','E','M','P',':','+','0','0','0',',','0','0',' ','C'};

/* Public Functions */

int DATAMANAGER_Init() {
	// Inicializa la temperatura y el contador de mediciones
    tempe = 0;
	count = 0;
    return 0;
}

int DATAMANAGER_Read() {
	count++; // Incrementa el contador de mediciones
	aux = SENSOR_GetTemperature();
	/* Obtiene la lectura del sensor y la guardo en una variable 
	auxiliar para anlizar si es negativo o positivo */
	if (aux > 0x0200)
		tempe = tempe - comp2(aux);
	else
		tempe = tempe + aux;
	// Si es temperatura negativa se resta su complemeto A2 se suma cc
#ifdef DEBUG
	// Para debuguear se escribe cada ves
	DATAMANAGER_Write();
#endif
    return 0;
}

int DATAMANAGER_Write() {
	volatile uint16_t centi, deci, uni, dece, cente;
	/*Las variables centi, deci, uni, dece, cente para contener cada dígito de la
	temperatura no deberían ser necesarias. Se hizo de este modo a razón de varios problemas 
	al compilar/simular */
	if (tempe < 0) {
		printable[5] = '-';
		tempe = tempe * (-1);
	} else
		printable[5] = '+';
	/* Si la temperatura acumulada es negativa se establece el signo - y se invierte el valor para realizar las cuentas
	caso contrario se establece el signo + */
#ifndef DEBUG
	tempe = tempe/count; // Se obtiene el promedio
#endif
	count = 0; //Se reinicia el contador de mediciones
	tempe = tempe * 25; /* Se multiplica por 25 para obtener la temperatura en centécimas de grado centígrado
						es por 25 ya que 25 centécimas de grado es el paso del sensor */
	centi = (tempe - 10*(tempe/10));
	tempe = tempe / 10;
	deci = (tempe - 10*(tempe/10));
	tempe = tempe / 10;
	uni = (tempe - 10*(tempe/10));
	tempe = tempe / 10;
	dece = (tempe - 10*(tempe/10));
	tempe = tempe / 10;
	cente = (tempe - 10*(tempe/10));
	tempe = 0;
	printable[11] = centi + 48;
	printable[10] = deci + 48;
	printable[8] = uni + 48;
	printable[7] = dece + 48;
	printable[6] = cente + 48;
	/* Todo este bloque de instrucciones obtiene las reprecentaciones ascii de cada dígito 
	y las prepara para enviar al LCD; se podría simplificar pero se hizo así por errores al simular */
	LCD_goToXY(0,0);
	LCD_sendString(printable, 14);
	// Se manda a escribir la temperatura al LCD
    return 0;
}

uint16_t comp2(uint16_t n){
	// Retorna el complemento A2 de un número negativo de 10 bit almacenado en una variable de 16 bits
	return (~n & 0x01FF) + 1;
}