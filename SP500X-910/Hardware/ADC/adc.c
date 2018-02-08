#include "adc.h"

#define M  2 //��������ͨ��
#define N  1 //ÿͨ����1��

u16 AD_Value[2];  //�������ADCת�������Ҳ��DMA��Ŀ���ַ

void RCC_Configuration(void)
{
	SystemInit();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE); //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  //ʹ��DMA1����
}

static void DMA1_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1����Ϊȱʡֵ
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&ADC1->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&AD_Value;    //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;   //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize=M*N;   //DMAͨ���Ļ���Ĵ�С����ADC��M��ͨ���ڹ�����ÿ��ͨ����N��
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;   //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;       //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ	
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;  //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;    //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;  //DMAͨ��x�и����ȼ�
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;     //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);    //����DMA_InitStructure��ָ���Ĳ�����ʼ��DMA��ͨ��
	
	DMA_Cmd(DMA1_Channel1,ENABLE);  //����DMAͨ��
}

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;  //PA6��PA7  ADC12_IN6��ADC12_IN7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

static void ADC1_Mode_Config(void)
{
	//#define ADC1_DR_Address ((uint32_t)0x4001244C)
	
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;
	
	ADC_DeInit(ADC1);  //������ADC1��ȫ���Ĵ�����Ϊȱʡֵ
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;       //ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;   //ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //�ⲿ����ת���ر�
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;    //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=M;      //˳����й���ת����ADCͨ����Ŀ��������2��Ϊͨ��6��7
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//����ʱ�����ҲҪ71.5������Ӱ��DMA���ݴ���
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_239Cycles5);  //��ͨ6ͨ����ת��˳��Ϊ1������ʱ��Ϊ239.5����
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,2,ADC_SampleTime_239Cycles5);  //��ͨ7ͨ����ת��˳��Ϊ2������ʱ��Ϊ239.5����
	
	ADC_DMACmd(ADC1, ENABLE);  //����DAC��DMA֧�֣�DMA���������

	ADC_Cmd(ADC1, ENABLE);   //ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);    //��λָ����ADC1��У׼�Ĵ���
	
	while
	(ADC_GetResetCalibrationStatus(ADC1));  //��ȡADC1��λУ׼�Ĵ�����״̬������״̬��ȴ�
	
	ADC_StartCalibration(ADC1);     //����ָ��ADC1��У׼״̬
	
	while
	(ADC_GetCalibrationStatus(ADC1));  //��ȡָ��ADC1��У׼�Ĵ�����״̬������״̬��ȴ�
	
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
