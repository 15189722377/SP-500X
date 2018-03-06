#ifndef __ADC_H

#define __ADC_H

#include "stm32f10x.h"

#define SPI_SLAVE_IN_Pin GPIO_Pin_8
#define SPI_SLAVE_IN_PORT GPIOA
#define MOSI_H  GPIO_SetBits(SPI_SLAVE_IN_PORT,SPI_SLAVE_IN_Pin)
#define MOSI_L  GPIO_ResetBits(SPI_SLAVE_IN_PORT,SPI_SLAVE_IN_Pin)

#define SPI_SLAVE_OUT_Pin GPIO_Pin_9
#define SPI_SLAVE_OUT_PORT GPIOA

#define SPI_CLK_Pin GPIO_Pin_15
#define SPI_CLK_PORT GPIOB
#define SPI_CLK_H GPIO_SetBits(SPI_CLK_PORT,SPI_CLK_Pin)
#define SPI_CLK_L GPIO_ResetBits(SPI_CLK_PORT,SPI_CLK_Pin)

#define ADC_CS_Pin GPIO_Pin_11
#define ADC_CS_PORT GPIOA
#define ADC_CS_H GPIO_SetBits(ADC_CS_PORT,ADC_CS_Pin)
#define ADC_CS_L GPIO_ResetBits(ADC_CS_PORT,ADC_CS_Pin)

void ADC_GpioInit(void);
float adcReadDark(u8 led,u8 pd, u16 repeat);
float adcReadChannel(u8 led, uint16_t current, u8 pd, u16 repeat);

#endif
