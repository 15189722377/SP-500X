#include "stdio.h"
#include "stm32f10x.h"
#include "mb.h"
#include "flash.h"

__IO u16 ADC_ConvertedValueLocal;
float adc_in6_voltage,adc_in7_voltage;
int main(void)
{
	System_Init();
	
	RestoreModbusReg(); 
	
	eMBInit(MB_RTU, comm_settings.modbusAddr, 0x02, comm_settings.modbusBaud, comm_settings.modbusParity);
  eMBEnable();  
	
	while(1)
	{
		eMBPoll();
		FunctionPoll();
	}	
}
