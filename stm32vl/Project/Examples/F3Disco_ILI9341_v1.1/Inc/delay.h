//#include <stdio.h>
#include "stm32f3xx_hal.h"

#define F_CPU 72000000

void TM_Delay_Init(void);
void TM_DelayMicros(uint32_t micros);
void TM_DelayMillis(uint32_t millis) ;
