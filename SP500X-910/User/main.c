#include "stdio.h"
#include "stm32f10x.h"
#include "mb.h"
#include "flash.h"

void IWG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����	
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ256	
	IWDG_SetReload(1562);  //����IWDG��װ��ֵ 10S	
	IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������	
	IWDG_Enable();  //ʹ��IWDG
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
