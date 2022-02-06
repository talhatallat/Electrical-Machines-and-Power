// This program outputs a 50Hz complimentary Sinusoidal PWM signal on PA8 and PB13.
// The switching speed is 10kHz.  The program uses a mix of mbed functions and
// direct register writes.  It makes use of a lookup table generated in Octave (Thanks Richard Hayes)
// and interrupts to update the duty cycle.
// A potentiometer is attached to A0 and this can be used to control the amplitude modulation index.
// Useful reference: http://stm32.kosyak.info/doc/index.html
#include "mbed.h"
// Define some bitmasks
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#define BIT8 (1 << 8)
#define BIT9 (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)
DigitalOut myled(LED1);
const int duties[]={\
    50,51,53,54,56,57,59,60,62,63,65,66,68,69,71,72,74, \
    75,76,78,79,80,81,83,84,85,86,87,88,89,90,91,92,93,\
    93,94,95,95,96,97,97,98,98,98,99,99,99,99,99,99,100,\
    99,99,99,99,99,99,98,98,98,97,97,96,95,95,94,93,93,\
    92,91,90,89,88,87,86,85,84,83,81,80,79,78,76,75,74,\
    72,71,69,68,66,65,63,62,60,59,57,56,54,53,51,49,48,\
    46,45,43,42,40,39,37,36,34,33,31,30,28,27,25,24,23,\
    21,20,19,18,16,15,14,13,12,11,10,9,8,7,6,6,5,4,4,3,\
    2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,3,4,4,\
    5,6,6,7,8,9,10,11,12,13,14,15,16,18,19,20,21,23,24,\
    25,27,28,30,31,33,34,36,37,39,40,42,43,45,46,48\    
};
/*
Pin mappings for Timer 1 (the advanced timer with deadtime)
From: https://developer.mbed.org/users/mbed_official/code/mbed-src/file/a11c0372f0ba/targets/hal/TARGET_STM/TARGET_STM32F1/TARGET_NUCLEO_F103RB/PeripheralPins.c
    {PA_8,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH1 - Default
    {PA_9,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH2 - Default
    {PA_10, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH3 - Default
    {PB_13, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH1N - Default
    {PB_14, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH2N - Default
    {PB_15, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // TIM1_CH3N - Default
 
*/
// Use mbed to configure the relevant pins as PWM outputs;
PwmOut      PhaATop(PA_8);    
PwmOut      PhaABtm(PB_13);  // This should be the complement of PA_8  
AnalogIn    Pot(A0);
volatile float PotValue;
void init(void);
int main() {    

    init();
    while(1) {
        //myled = 1; // LED is ON
        wait(0.2); // 200 ms
        //myled = 0; // LED is OFF
        wait(0.2); // 1 sec        
        PotValue = Pot;
    }
}

int ScaleDuty(int Duty,float factor)
{
    float ScaledDuty = Duty;
    ScaledDuty = (ScaledDuty - 50) * factor;
    ScaledDuty = ScaledDuty + 50;
    return ScaledDuty;
}
int index=0;
int state = 0;
void TimerISR()
{       
    TIM1->SR &= ~0x3f; // ack the interrupt    
    TIM1->CCR1=duties[index++]*1; // get the next duty
    if (index > 199)
    {        
        index = 0;   
        myled = state;  // generate a trigger signal for a scope
        state = ~state;
    }
}
void init()
{
    
    RCC->APB2ENR |= BIT2+BIT3;  // enable GPIOA and GPIOB    
    RCC->APB2ENR |= BIT11;      // enable TIM1
    
    GPIOA->CRH |= BIT3+BIT0;    // PA8 output, alternate function
    GPIOB->CRH |= BIT23+BIT20;  // PB13 output, alternate function
    
    TIM1->CR2 = 0;
    TIM1->DIER = BIT0; // Want update interrupt
    NVIC_SetVector(TIM1_UP_IRQn,(uint32_t) TimerISR);   
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    TIM1->SR = 0;      // Clear flags.
    TIM1->CCMR1= BIT6+BIT5+BIT4;
    TIM1->CCER = BIT2+BIT0;
    TIM1->ARR = 100;  // 1MHz = base clock freq.  Divide by 100 to get 10kHz.
    TIM1->CCR1 = 10;  // Low inital duty
    TIM1->CR1 |= BIT0; //enable counter
     __enable_irq();   // enable interrupts

}