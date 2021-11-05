#include "rc_meter.h"

volatile uint32_t adc_result;
extern uint8_t Flag_RC;

double nano_farads;
double nano_farads;
double start_time;
double elapsed_time;

void ADC1_2_IRQHandler(void)
{
    adc_result =  ADC1->DR;             /* lee el valor analogico de entrada*/
    Flag_RC = 1;                        /* levanta el flag para avisarle la dispatcher */
}

void RC_Init(int interruptions)                   
{	
    adc_result = 0;
    GPIOA->CRL = 0x44444304;            /* PA1(adc1):INPUT - PA2:OUTPUT 50 MHz */
    
    ADC1->CR2 |= (1<<ADON);              /* power-up: el cambio de 0 a 1 enciende el modulo */
    ADC1->SMPR2 = 7<<3;                 /* 239.5 ADC clock cycles en SMP1 (cada SMP usa 3 bits del registro SMPR) */


    if(interruptions){                   /* mode: 0 para polling, 1 para uso de interrupciones */            
        enableInterrupts();
    } 
}

void enableInterrupts(void)
{
    ADC1->CR1 = (1<<EOCIE);             /* EOCIE = 1 (interrupcion al finalizar la conversion habilitada) */
 }

void disableInterrupts(void)
{
    ADC1->CR1 = (0<<EOCIE);             /* EOCIE = 0 (interrupcion al finalizar la conversion desabilitada) */
}

void RC_StartConvertion(int continuously)
{
    ADC1->SQR3 = 1;                     /* elige el canal 1 como la entrada */
    if(continuously)
    {
            ADC1->CR2 |= 1<<CONT;
    }
    ADC1->CR2 |= 1<<ADON;
		NVIC_EnableIRQ(ADC1_2_IRQn);
}

uint32_t RC_GetCapacitance(void)
{
    return adc_result;
}



/* * * * * * * * * * * * * * * * * * * * * * * *
tc
    Time Constant [s]
    tc = R * C
r
    Resistance [Ohms]
c 
    Capacitance [F]

voltage 
    voltage at tc=63.2% of charging voltage
    ejemplo: (10.000 Ohms) * (100 uF) = 1 s

* * * * * * * * * * * * * * * * * * * * * * * * * */
