#include "delay.h"

uint32_t multiplier;

void TM_Delay_Init(void)
{
    uint32_t RCC_Clocks;
    
    /* Get system clocks */
    RCC_Clocks = F_CPU;
    
    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M */
    multiplier = RCC_Clocks / 4000000;
}

void TM_DelayMicros(uint32_t micros)
{
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}
 
void TM_DelayMillis(uint32_t millis) 
{
    /* Multiply millis with multipler */
    /* Substract 10 */
    millis = 1000 * millis * multiplier - 10;
    /* 4 cycles for one loop */
    while (millis--);
}
