/**
  ******************************************************************************
  * @file    LTC2630ISC6.h
  * @author  bo.zhu
  * @version V1.0
  * @date    2016-12-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  *  
  ******************************************************************************
  */ 
#ifndef __LTC2630ISC6_H__
#define __LTC2630ISC6_H__

#include "stm32f10x.h"
//#include "sys.h"
#include "delay.h"

#define  DAC_CS_HIGH  GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define  DAC_CS_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_10)

#define SPI_SLAVE_IN_BB_HIGH    GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define SPI_SLAVE_IN_BB_LOW     GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define SPI_CLOCK_HIGH   GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define SPI_CLOCK_LOW    GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define LTC2630ISC6_POWER_DOWN_ 0X70
#define LTC2630ISC6_WRITE_TO_AND_UPDATE 0X30

#define ADG_EN_B GPIO_Pin_7
#define LED_SWITCH_ENABLE  GPIO_SetBits(GPIOB,ADG_EN_B)
#define LED_SWITCH_DISABLE GPIO_ResetBits(GPIOB,ADG_EN_B);

#define MUX_A0_B GPIO_Pin_12
#define MUX_A1_B GPIO_Pin_5
#define MUX_A2_B GPIO_Pin_6
#define MUX_A3_B GPIO_Pin_9
#define A0_H  GPIO_SetBits(GPIOA,MUX_A0_B)
#define A1_H  GPIO_SetBits(GPIOB,MUX_A1_B)
#define A2_H  GPIO_SetBits(GPIOB,MUX_A2_B)
#define A3_H  GPIO_SetBits(GPIOB,MUX_A3_B)
#define A0_L  GPIO_ResetBits(GPIOA,MUX_A0_B)
#define A1_L  GPIO_ResetBits(GPIOB,MUX_A1_B)
#define A2_L  GPIO_ResetBits(GPIOB,MUX_A2_B)
#define A3_L  GPIO_ResetBits(GPIOB,MUX_A3_B)

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED365 7
#define LED8 8
#define LED9 9
#define LED10 10
#define LED11 11

void write_to_LTC2630ISC6(u8 Cmd, u16 Dat);
void LTC2630ISC6_Init(void );
void ADG1606_Init(void);
void turnOnLED(u8 LED,u16 current);
void turnOffLEDs(void);

#endif
