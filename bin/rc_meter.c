#include "rc_meter.h"

volatile uint32_t adc_result;
extern uint8_t Flag_RC;

void ADC1_2_IRQHandler(void)
{
		printf("ADC1 IRQ Handler");
    Flag_RC = 1;
    adc_result =  ADC1->DR;
}

void RC_Init(int interruptions)                   
{		adc_result = 0;
    RCC->APB2ENR |= (1<<APB2ENR_ADC1);  /* habilita el clock para adc1 */
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
    // return transform(); /* por si se necesita una transformacion del valor para la lectura */
}

// uint16_t transform()
// {
//     adc_result = adc_result algo;
//     return adc_result;
// }
