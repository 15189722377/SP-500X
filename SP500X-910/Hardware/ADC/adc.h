#ifndef __ADC_H

#define __ADC_H

#include "stm32f10x.h"

#define CHANNEL_6 6
#define CHANNEL_7 7

void RCC_Configuration(void);
float ADC_GetValue(u8 channel);
void ADC1_Init(void);

#endif
