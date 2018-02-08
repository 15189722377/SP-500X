#include "adc.h"

#define M  2 //两个采样通道
#define N  1 //每通道采1次

u16 AD_Value[2];  //用来存放ADC转换结果，也是DMA的目标地址

void RCC_Configuration(void)
{
	SystemInit();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE); //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  //使能DMA1传输
}

static void DMA1_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);   //将DMA的通道1重设为缺省值
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&ADC1->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&AD_Value;    //DMA内存基地址
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;   //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize=M*N;   //DMA通道的缓存的大小，若ADC有M个通道在工作，每个通道采N次
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;   //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;       //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位	
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;  //数据宽度为16位
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;    //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;  //DMA通道x有高优先级
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;     //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);    //根据DMA_InitStructure中指定的参数初始化DMA的通道
	
	DMA_Cmd(DMA1_Channel1,ENABLE);  //启动DMA通道
}

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;  //PA6，PA7  ADC12_IN6，ADC12_IN7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; //模拟输入引脚
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

static void ADC1_Mode_Config(void)
{
	//#define ADC1_DR_Address ((uint32_t)0x4001244C)
	
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;
	
	ADC_DeInit(ADC1);  //将外设ADC1的全部寄存器设为缺省值
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;       //模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;   //模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //外部触发转换关闭
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;    //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel=M;      //顺序进行规则转换的ADC通道数目，这里用2，为通道6和7
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//采样时间最短也要71.5，否则影响DMA数据传输
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_239Cycles5);  //开通6通道，转换顺序为1，采样时间为239.5周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,2,ADC_SampleTime_239Cycles5);  //开通7通道，转换顺序为2，采样时间为239.5周期
	
	ADC_DMACmd(ADC1, ENABLE);  //开启DAC的DMA支持，DMA需独立配置

	ADC_Cmd(ADC1, ENABLE);   //使能指定的ADC1

	ADC_ResetCalibration(ADC1);    //复位指定的ADC1的校准寄存器
	
	while
	(ADC_GetResetCalibrationStatus(ADC1));  //获取ADC1复位校准寄存器的状态，设置状态则等待
	
	ADC_StartCalibration(ADC1);     //开启指定ADC1的校准状态
	
	while
	(ADC_GetCalibrationStatus(ADC1));  //获取指定ADC1的校准寄存器的状态，设置状态则等待
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
float test;
float ADC_GetValue(u8 channel)
{
	if(channel==6)
	{
		test=3.3*AD_Value[0]/4096;
		test=test;
		return (3.3*AD_Value[0]/4096);
	}
	else if(channel==7)
	{
		return (3.3*AD_Value[1]/4096);
	}
	else
		return -1;
}

void ADC1_Init(void)
{
	ADC1_GPIO_Config();	
	ADC1_Mode_Config();
	DMA1_Configuration();
}
