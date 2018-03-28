#include "app.h"
#include "regmap.h"
#include "flash.h"
#include "adc.h"
#include "delay.h"
#include "string.h"
#include "LTC2630ISC6.h"
#include "products.h"

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
	memset(system_status.deviceName,0,sizeof(system_status.deviceName));
	memset(system_status.serial,0,sizeof(system_status.serial));
	memset(system_status.sampleName,0,sizeof(system_status.sampleName));
	memset(system_status.hardwareVer,0,sizeof(system_status.hardwareVer));
	memset(system_status.softwareVer,0,sizeof(system_status.softwareVer));
	
	system_status.runStatus=0;
	system_status.commStatus=0;
	system_status.calibStatus=0;
	system_status.configStatus=1;  
	system_status.productNum=SP500X;  //SP500X尚未申请pn号，此处后期务必修正
	strcpy(system_status.deviceName,"SP500X");
	strcpy(system_status.serial,"1");
	strcpy(system_status.sampleName,"CL-F");
	strcpy(system_status.hardwareVer,HW_VERSION);
	strcpy(system_status.softwareVer,SW_VERSION);
	
	measure_settings.command=0;
	measure_settings.sampleCycle=4;  //测量周期，秒
	measure_settings.reserved1=0;
	measure_settings.reserved2=0;
	
	comm_settings.modbusAddr=0x26;	
	comm_settings.modbusDatabits=8;
	comm_settings.modbusParity=MODBUS_PARITY_EVEN;
	comm_settings.modbusBaud=9600;
	memset(comm_settings.reserved,0,sizeof(comm_settings.reserved));
	
	sensor_param.led=0;
	sensor_param.ledCurrent=0;
	sensor_param.pdChannel=PD_NONE;
	sensor_param.sampleTimes=0;  //采样次数
	sensor_param.adcResult=0;
	sensor_param.dark=0;
	memset(sensor_param.reserved,0,sizeof(sensor_param.reserved));
	
	StoreModbusReg();
}

static void Measure_TIM3Init(void)
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

u16 count_seconds=0;
u8 startMeasure=0;
 void TIM3_IRQHandler(void)
{
  if( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET )
  {	 
    IWDG_ReloadCounter();    //喂狗		
		if(count_seconds<measure_settings.sampleCycle)
		{
			count_seconds++;
			startMeasure=0;
		}
		else
		{
			count_seconds=0;
			startMeasure=1;
		}
	  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
  }
}

static void Modbus_TIM2Init(void)
{
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;// 
  
  TIM_TimeBaseStructure.TIM_Period =50;     //50ms
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;      
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);     
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);   
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);   

  TIM_Cmd(TIM2,ENABLE);	
}

u8 count_50ms=0;
void TIM2_IRQHandler(void)
{		
	if( TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET )
	{	
		  IWDG_ReloadCounter();    //喂狗
			eMBPoll();	
			
		  if(bitmodbus==1)
			{
				bitmodbus=0;
				StoreModbusReg();
			}
		
			if(measure_settings.command==CMD_JUMP_BOOTLOADER)
			{
				++count_50ms;
				if(count_50ms>10)
				{
					measure_settings.command=CMD_NONE;
					StoreModbusReg();  //此处必须保存寄存器，否则CMD得不到真正复位，探头不断进入Boot模式
					delay_ms(1500);
					__disable_irq();
					NVIC_SystemReset();			
				}				
			}
	
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);				
	}
}

void System_Init(void)
{
	delay_init();
	LTC2630ISC6_Init();  //LED power
	ADG1606_Init();  // LED switch
	ADC_GpioInit();
	Measure_TIM3Init();
	Modbus_TIM2Init();
}

void FunctionPoll(void)
{
	u16 adcValue=0;
	if(startMeasure==1)
	{
		startMeasure=0;
		sensor_param.dark=adcReadDark(sensor_param.led,sensor_param.pdChannel,sensor_param.sampleTimes);
		adcValue=adcReadChannel(sensor_param.led,sensor_param.ledCurrent,sensor_param.pdChannel,sensor_param.sampleTimes);
	  sensor_param.adcResult=adcValue-sensor_param.dark;
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
			measure_settings.command=CMD_NONE;
			Param_Reset();
		  break;
		}
		case CMD_CALIB_ZERO:
		{
			measure_settings.command=CMD_NONE;
			break;
		}
		case CMD_CALIB_SLOPE:
		{
			measure_settings.command=CMD_NONE;
			break;
		}
	}
}
