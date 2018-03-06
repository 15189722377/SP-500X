#include "stdio.h"
#include "stm32f10x.h"
#include "mb.h"
#include "flash.h"

void IWG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作	
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //设置IWDG预分频值:设置IWDG预分频值为256	
	IWDG_SetReload(1562);  //设置IWDG重装载值 10S	
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器	
	IWDG_Enable();  //使能IWDG
}

int main(void)
{
	#define VECT_TAB_OFFSET  0x3000

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECT_TAB_OFFSET);
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	
	System_Init();
	IWG_Init();
	RestoreModbusReg(); 
	
	eMBInit(MB_RTU, comm_settings.modbusAddr, 0x02, comm_settings.modbusBaud, comm_settings.modbusParity);
  eMBEnable();  
	
	while(1)
	{
		FunctionPoll();
	}	
}
