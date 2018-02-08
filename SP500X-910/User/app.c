#include "app.h"
#include "regmap.h"
#include "flash.h"
#include "adc.h"
#include "delay.h"
#include "string.h"

u8 bitmodbus=0;

SYS_STATUS_T system_status;

COMM_SETTINGS_T comm_settings;

MEASURE_SETTINGS_T measure_settings;

CALIB_SETTINGS_T calib_settings;

FILTER_SETTINGS_T filter_settings;

MEASURE_VALUES_T measure_values;

SENSOR_PARAM_T sensor_param;

void Param_Reset(void)
{
	memset(system_status.hardwareVer,0,sizeof(system_status.hardwareVer));
	memset(system_status.softwareVer,0,sizeof(system_status.softwareVer));
	strcpy(system_status.hardwareVer,HW_VERSION);
	strcpy(system_status.softwareVer,SW_VERSION);
	
	system_status.configStatus=1;  
	
	measure_settings.command=0;
	measure_settings.measureRange=2000.0;
	measure_settings.sampleCycle=4;
	measure_settings.smoothingFactor=1;
	
	comm_settings.modbusAddr=0x0E;	
	comm_settings.modbusDatabits=8;
	comm_settings.modbusParity=MODBUS_PARITY_EVEN;
	comm_settings.modbusBaud=9600;
	
	sensor_param.slope=30000.0;
	sensor_param.v0_sum=0.1;
	sensor_param.weight_20mA=2000.0;
	sensor_param.calibSolution=1000.0;
	
	StoreModbusReg();
}

float lastWeight=0;
float Weight_Measure(void)
{
	float weight,v_sum;
	
	measure_values.v1=ADC_GetValue(CHANNEL_6);
	measure_values.v2=ADC_GetValue(CHANNEL_7);
	v_sum=measure_values.v1+measure_values.v2;
	
	weight=sensor_param.slope*(v_sum-sensor_param.v0_sum);
	weight=measure_settings.smoothingFactor*weight+(1-measure_settings.smoothingFactor)*lastWeight;	
	lastWeight=weight;
	
	return weight;
}

static void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  uint16_t PrescalerValue = 0;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器3时钟
	
	//定时器时间基配置说明
  //HCLK为72MHz，APB1经过2分频为36MHz
  //TIM3的时钟倍频后为72MHz（硬件自动倍频,达到最大）
  //TIM3的分频系数为3599，时间基频率为72M/ (1 + Prescaler) = 10KHz,基准为0.1ms
  //TIM最大计数值为usTim1Timerout50u
  PrescalerValue = (uint16_t) (SystemCoreClock / 10000) - 1;// 
  
  TIM_TimeBaseStructure.TIM_Period =9999;     //    timing cycle = 1S
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;  //预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;      //设置分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  TIM_ARRPreloadConfig(TIM3, ENABLE);     //预装载使能
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //定时器3中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);   
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);   //使能TIM3更新中断

  TIM_Cmd(TIM3,ENABLE);	
}

u8 weight_measure=0;
 void TIM3_IRQHandler(void)
{
	u8 measurePeriod=0;
  if( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET )
  {	  
		if((++measurePeriod)>=measure_settings.sampleCycle)
		{
			measurePeriod=0;
			weight_measure=1;
		}
	  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
  }
}

void System_Init(void)
{
	RCC_Configuration();
	delay_init();
  //ADC1_Init();
	//TIM3_Init();
}

void FunctionPoll(void)
{
	float calibSolution_v_sum;
	
	if(bitmodbus==1)
	{
		StoreModbusReg();
	}
	switch(measure_settings.command)
	{
		case CMD_SAVE_MODBUSREG:
		{
			measure_settings.command=CMD_NONE;
			StoreModbusReg();
		  break;
		}
		case CMD_RESETT_PARAM:
		{
			Param_Reset();
		  break;
		}
		case CMD_CALIB_ZERO:
		{
			measure_settings.command=CMD_NONE;
			measure_values.v1=ADC_GetValue(CHANNEL_6);
			measure_values.v2=ADC_GetValue(CHANNEL_7);
			sensor_param.v0_sum=measure_values.v1+measure_values.v2;
			break;
		}
		case CMD_CALIB_SLOPE:
		{
			measure_settings.command=0;
			measure_values.v1=ADC_GetValue(CHANNEL_6);
			measure_values.v2=ADC_GetValue(CHANNEL_7);
			calibSolution_v_sum=measure_values.v1+measure_values.v2;
			sensor_param.slope=(sensor_param.calibSolution-0)/(calibSolution_v_sum-sensor_param.v0_sum);
		}
	}
	if(weight_measure==1)
	{
		weight_measure=0;		
		measure_values.sensorValue=Weight_Measure();
		measure_values.sensorValue_mA=measure_values.sensorValue*16/sensor_param.weight_20mA+4;
	}
}
