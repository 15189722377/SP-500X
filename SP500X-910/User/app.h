#ifndef __APP_H

#define __APP_H

#include "stm32f10x.h"
#include "regmap.h"

#define HW_VERSION  "1.0"
#define SW_VERSION  "1.0"

#define MODBUS_PARITY_NONE 	0
#define MODBUS_PARITY_ODD 	1
#define MODBUS_PARITY_EVEN 	2

#define CMD_NONE	          0
#define CMD_SAVE_MODBUSREG	1
#define CMD_RESETT_PARAM	  2
#define CMD_CALIB_ZERO	    3
#define CMD_CALIB_SLOPE		  4

//系统相关寄存器声明
extern SYS_STATUS_T system_status;

extern COMM_SETTINGS_T comm_settings;

extern MEASURE_SETTINGS_T measure_settings;

extern CALIB_SETTINGS_T calib_settings;

extern FILTER_SETTINGS_T filter_settings;

extern MEASURE_VALUES_T measure_values;

extern SENSOR_PARAM_T sensor_param;

void Param_Reset(void);
void System_Init(void);
void FunctionPoll(void);

#endif
