/**
  ******************************************************************************
  * @file    LTC2630ISC6.c
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
#include "LTC2630ISC6.h"

typedef	struct 	ledparam
{																		
	u8	led_serialNum;
  u16  led_wavelength; 	
} LED_PARAM;

LED_PARAM SP910_LED[]=
{
	{1,560},
	{2,570},
	{3,0},
	{4,0},
	{5,455},
	{6,525},
	{7,365},
	{8,630},
	{9,420},
	{10,0},
	{11,0},
};

void LTC2630ISC6_GpioInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=GPIO_Pin_10;   //DAC_CS
	GPIO_Init(GPIOA,&g);
	
	g.GPIO_Pin=GPIO_Pin_15;   //SPI_CLOCK_BB
	GPIO_Init(GPIOB,&g);
	
	g.GPIO_Pin=GPIO_Pin_8;   //SPI_SLAVE_IN_BB
	GPIO_Init(GPIOA,&g);
	
	
}

void dacWriteByte(u8 byte)
{
	u8 i;
	SPI_CLOCK_LOW;
	for (i=0;i<8;i++)
	{
		if (byte&(0x80>>i))
		{
			SPI_SLAVE_IN_BB_HIGH;
		} 
		else
		{
			SPI_SLAVE_IN_BB_LOW;
		}
		SPI_CLOCK_HIGH;
		delay_us(5);
		SPI_CLOCK_LOW;
	}
}

void write_to_LTC2630ISC6(u8 Cmd, u16 Dat)
{
	if(Dat>4095)
	{
		Dat=4095;
	}
	Dat<<=4;
	DAC_CS_LOW;
	dacWriteByte(Cmd);
	dacWriteByte(Dat>>8);
	dacWriteByte(Dat|0x00ff);
	DAC_CS_HIGH;
}

void LTC2630ISC6_Init(void)
{
	 LTC2630ISC6_GpioInit();
	 write_to_LTC2630ISC6(0X60,0); //select internal reference(power-on reset default)
	 write_to_LTC2630ISC6(0X30,1000); //write to and update(power up)DAC register, 1000/4095*2.5=0.61v
}

void ADG1606_Init(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=ADG_EN_B;   
	GPIO_Init(GPIOB,&g);
	
	g.GPIO_Pin=MUX_A1_B | MUX_A2_B | MUX_A3_B;   
	GPIO_Init(GPIOB,&g);
	
	g.GPIO_Pin=MUX_A0_B;   
	GPIO_Init(GPIOA,&g);
	
	LED_SWITCH_DISABLE; //disable LED switch
}

void turnOffLEDs(void)
{
	LED_SWITCH_DISABLE;
	write_to_LTC2630ISC6(0x30,0);
}

void turnOnLED(u8 LED,u16 current)
{
	write_to_LTC2630ISC6(0x30,current);
	LED_SWITCH_ENABLE;
	
	switch(LED)
	{
		case LED1:
		{
			A3_L;
			A2_L;
			A1_L;
			A0_L;
				break;
		}
		case LED2:
		{
			A3_L;
			A2_L;
			A1_L;
			A0_H;
				break;
		}
		case LED3:
		{
			A3_L;
			A2_L;
			A1_H;
			A0_L;
				break;
		}
		case LED4:
		{
			A3_L;
			A2_L;
			A1_H;
			A0_H;
				break;
		}
		case LED5:
		{
			A3_L;
			A2_H;
			A1_L;
			A0_L;
				break;
		}
		case LED6:
		{
			A3_L;
			A2_H;
			A1_L;
			A0_H;
				break;
		}
		case LED365:
		{
			A3_L;
			A2_H;
			A1_H;
			A0_L;
				break;
		}
		case LED8:
		{
			A3_L;
			A2_H;
			A1_H;
			A0_H;
				break;
		}
		case LED9:
		{
			A3_H;
			A2_L;
			A1_L;
			A0_L;
				break;
		}
		case LED10:
		{
			A3_H;
			A2_L;
			A1_L;
			A0_H;
				break;
		}
		case LED11:
		{
			A3_H;
			A2_L;
			A1_H;
			A0_L;
				break;
		}		
		default:		
				break;
	}	
}

