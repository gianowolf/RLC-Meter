# SysTic timer 

- Every ARM Cortex M.

## contador

- Down counter STCURRENT (Systick->VAL). cAN RECEIVE CLOCK from 2 diff sources.
- The clock ssc is chosen using CLK_SRC bit of STCTRL (SysTick->CTRL) register.

## Registros

### STCTRL (SysTick Control and Status)

- 0 ENABLE: 0 diasbled, 1 enabes systick
- 1 TICKINT: 0 int dis, 1 when systick counts to 0 int is generated.
- 2 CLKSOURCE: 0 AHB clock divideb by 8 - AHB clock
- 16 COUNTFLAG: 0 SysTick has not counted dt0 - 1: systick has counted to zero
- NOTE: COUNTLAG is clared by reading STRCTRL or writin STCURRENT.

### SysTick Reload value register (STRELOAD)

STREOAD is used to program de start value of ystick down counter. the stcurrent register, dth streload sohuld contain the value n0 1 for d=the count to fire envery n clock cycles 

# STM32F10x TIMERS

## Delay generation

### Types of Timers

- Basic
- General purpose (Timers 2, 3 and 4)
- Advanced control timers (Timer 1)

![](2021-10-30-17-57-43.png)

## Registers

### CNT (Counter)

- 16-bit up/down counter.
- TIMx_CNT to load value

### ARR Auto Reload Register

- 16 bit  
- Continously compared with the TIMx_CNT reg


### TIMx_CR1 (Control Registe)

- ways: up, down, and both like a yoyo

![](2021-10-30-18-01-08.png)

- 9-8 CKD: Clock division used by dead-time
- 7 ARPE: Auto reload preload enable 0 ARR not buff, 1 Buffered
- 6-5 Center-aligned mode selection 
    - 00 up or down
- 4 DIR: is CMS is 00, dir chooses te direction of counting. 0 ups, 1 down 
- 3 OPM: one pulse mode 0 continously, 1 counter stops at the next update event 
- 2 URS update request source 
- 1 UDIS update disable (mask)
- 0 CEN counter enable 0 DIS, 1 ENA

### TIMx_SR (Status Register)


![](2021-10-30-18-05-56.png)

- 0 UIF: Update Interrupt Flag. 
- When conuting up mode, when the value CNT reaches ARR, UIF flag is SET. The lag remains high until it is clered by software in the counting down mode. The flag is se twhn conuter sreaches 0 

### Counting Up

- When TIMx_CNT counter is counting up, it is compared with the ocuntents of TIMx_ARR register. 
- When CNT and ARR are equal UIF flag goes up indicating is a match and CNT rolls over to zero.

### Enabling Clock

For each timer we have a separate clock enable bit in RCC_APB1ENR and RCC_APB2ENr registers.

## Making Delays using TIM timer in up counting mode 

1. Enable the clock to TIMx module
2. load TIMx_ARR register with proper value
3. Clear UIF flag.
4. Set mode as up couter timer and enable timer 
5. Wait for UIF flag to go HIGH
6. Stop the timer 



## Prescaler 

![](2021-10-31-08-59-41.png)

- Contains a 16 bit up counter and a comparator
- up-counter counts from - t value of TIMx_PSC reg.
  

```c
/*
- ARR = 71 -> 72 states (clk goes from 0 to 71) 
- default timer clk *72MHz -> each clock lasts 1/72MHz
- so delay = 71 * 1/72Mhz = 1us
*/
void delay()
{
    RCC->APB1ENR |= (1<<0); /* enable TIM2 clk */
    TIM2->ARR = 71;
    TIM2->SR  = 0; /* clear UIF flag */
    TIM2->CR1 = 1; /* up counting */
    while((TIM2->SR & 1) == 0); /* wait until UIF flag is set */
    TIM2->CR1 = 0; /* stop counting */
}

void delayConPrescaler()
{
    RCC->APB1ENR |= (1<<0);
    TIM2->PSC = 7200-1;   /* psc = 7199 */
    TIM2->ARR = 500-1;

    TIM2->SR = 0;  /* clear UIF */
    TIM2->CR1 = 1; /* up counting */
    while((TIM2->SR & 1) == 0); 
    TIM2->CR1 = 0; /* stop counting */
}


void delayOnePulseMode() //timer stops automatically
{
    RCC->APB1ENR |= (1<<0); /* enable TIM2 clk */
    TIM2->PSC = 7200-1;
    TIM2->ARR = 500 - 1;
    TIM2->SR  = 0; /* clear UIF flag */
    TIM2->CR1 = 9; /* up counting & OnePulseMode*/
    while((TIM2->SR & 1) == 0); /* wait until UIF flag is set */
}
```

## Programming Output Compare 

- In some apps we need control digital output transition with PRECISION TIMING.
- In STM32 each TIMx has 4 channels 
- Each ch has an input circuit for input capturing and output circuit for output compare.

![](2021-10-31-09-25-28.png)

### Cap/Comp Reg (TIMx_CCRn)

- CCR1 to 4.
- Readable and writable
- TIMx_CCRn is compared with TIMx_CNT after each clock cycle.
- When CNT and CCRn match, CCnOF flag is set HIGH
- There is a WAVE GENERATOR in each chanel, making pin go low to high (toggling ping).

### Cap/Comp Mode Reg (TIMx_CCMR1 TIM_CCMR2)

- D0-7, D8-15, D16-31, ... for ch1, ch2, ch3, and ch4
- Each ch has CCnS (Comp/Capt Selection)
- CCnS bits are used to enable the input or output circuit of the ch 
- CCnS = 00, output circuit is enable and channel is in Compare mode.
- OCnM select the behavior of the wave generator con copare match (TIMx_CNT = TIMx_CCRn). -> the wave generator might SET the pin, make low or toggle it. Example: if OCnM = 011 the wave gen toggles the GPIO on compare match

![](2021-11-01-10-04-26.png)

### Cap/Comp Enable Register (TIMx_CCER)

![](2021-11-01-10-05-29.png)

- if CCnP=0 the output of the wave generator goes directly to the GPIO pin.
- if CCnP=1 the output signal becomes INVERTED and goes to the GPIO pin.

## Channels 

![](2021-11-01-10-09-43.png)

## Wave Generation Examples


### Example 1: Simple Wave 

- Timer counts and toggles CH3 when CNT=CCR3.
- CCR3 is set to 200, so CH3 toggles after 200 clocks
- The CH3 pin toggles agin on next CNT=CCR3 
```c
int main()
{
    RCC->APB2ENR |= 0xFC; /* enable gpio clocks */
    RCC->APB1ENR |= (1<<0); /* enable TIM2 clock */
    GPIO->CRL = 0x44444B44 /* PA2: Alternate func output */

    TIM2->CCR3 = 200;
    TIM2->CCER = 0x1 <<8; /* CC3P = 0, CC3E = 1 */
    TIM2->CCMR2= 0x0030 /*toggle channel 3 */
    TIM2->ARR = 10000-1;
    TIM2->CR1 = 1; /* start counting */

    while(1)
    {
    }
}



```





