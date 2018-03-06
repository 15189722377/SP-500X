#include "adc.h"
#include "delay.h"
#include "LTC2630ISC6.h"

u16 AdcChannelCmd;
const u8 ADC_ChannelCmd[]=
{
	0x84,
	0xC4,
	0x94,
	0xD4,
	0xA4,
	0xE4,
	0xB4,
	0xF4
};

void ADC_GpioInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=ADC_CS_Pin;   
	GPIO_Init(ADC_CS_PORT,&g);
	
	g.GPIO_Pin=SPI_CLK_Pin;   
	GPIO_Init(SPI_CLK_PORT,&g);
	
	g.GPIO_Pin=SPI_SLAVE_IN_Pin;   
	GPIO_Init(SPI_SLAVE_IN_PORT,&g);
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=SPI_SLAVE_OUT_Pin;   
	GPIO_Init(SPI_SLAVE_OUT_PORT,&g);
	
	ADC_CS_H;
	SPI_CLK_L;
}

void ADC_SetChannel(u8 channel)
{
	if((channel==0)||(channel>9))
	{
		AdcChannelCmd=0x84;  //channel0
	}
	else
	{
		AdcChannelCmd=ADC_ChannelCmd[channel-1];
	}
	AdcChannelCmd=AdcChannelCmd<<8;
}

uint16_t adcRead(void)
{
	u8  i;
	uint16_t adcValue=0;

	SPI_CLK_L;
	ADC_CS_H;
	delay_us(10);
	ADC_CS_L;
	delay_us(100);
	
	//send		
	for (i=0; i<16; i++)
	{
		// output channel command
		if (AdcChannelCmd & (0x8000 >> i))
			MOSI_H;
		else
			MOSI_L;

		SPI_CLK_H;
		delay_us(10);
    //delayUs(30);

		// read AD result
		if (GPIO_ReadInputDataBit(SPI_SLAVE_OUT_PORT, SPI_SLAVE_OUT_Pin))
			adcValue |= 0x01;
		if (i != 15)
			adcValue <<= 1;

		SPI_CLK_L;
	}

	ADC_CS_H;
			
	return (adcValue);
}

float adcReadDark(u8 led,u8 pd, u16 repeat)
{
	u16 j=0;
	double average = 0;
	
	turnOffLEDs();
	delay_ms(10);
	ADC_SetChannel(pd);
	adcRead();
	
	for(j = 0; j <= repeat; j++)  //舍弃首个测量值
	{
		if(j != 0)
			average += adcRead();
	}
	if(repeat==0)
	{
		return 0;
	}
	else
	{
		return (average / repeat);
	}    
}

float adcReadChannel(u8 led, uint16_t current, u8 pd, u16 repeat)
{
	u8 i;	
	double average = 0;

	if (current == 0)
	{
		turnOffLEDs();
	}
	else
	{
		turnOnLED(led, current);
	}
	delay_ms(10);
	ADC_SetChannel(pd);
	adcRead();
	
	for(i = 0; i <= repeat; i++)  //舍弃首个测量值
	{
		if(i != 0)
			average += adcRead();
	}
	
	if (current != 0)  //采样结束切记关灯
	{
		turnOffLEDs();
	}
	
	if(repeat==0)
	{
		return 0;
	}
	else
	{
		return (average / repeat);
	}    
}

