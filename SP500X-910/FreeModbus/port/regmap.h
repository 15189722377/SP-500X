/**
  ******************************************************************************
  * @file    regmap.h
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   Modbus register address map
  ******************************************************************************
  * @attention
  *  
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion --------------------------------------*/

#ifndef __REGMAP_H

#define __REGMAP_H

/* Includes -------------------------------------------------------------------*/

#include "stm32f10x.h"

/* Exported_Macros ------------------------------------------------------------*/

typedef uint16_t uint16;
typedef uint32_t uint32;

/* Exported_Types -------------------------------------------------------------*/

typedef enum
{
    PD_NONE=0,              
		PD_OUT1,
	  PD_OUT2,  
		PD_OUT3,
		PD_OUT4,
		PD_OUT5,
		PD_OUT6,
		PD_OUT7,
		PD4T1	   //PD4的一级放大
} PD_Channel;

//2 byte aligned
#pragma pack(2)//

typedef	struct sysStatus
{											/* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	16bit integer	read  */
	uint16   	commStatus;					/* 41002	16bit integer	read  */
	uint16 		calibStatus;				/* 41003 	16bit integer read  */
	uint16		configStatus;				/* 41004	16bit integer	read  配置状态(0系统未配置,1系统已配置)*/
	uint32		productNum;					/* 41005	16bit integer	read  */
	char		deviceName[16];       /* 41007	128bit */	
	char		serial[16];           /* 41015	128bit char read */
	char		hardwareVer[16];      /* 41023	128bit char read */
	char		softwareVer[16];      /* 41031	128bit char read */
	char		sampleName[16];				/* 41039-41046  所测物质的名称 */
} SYS_STATUS_T;

typedef struct commSettings
{											/* Register		Type  		 R/W */
	uint16		modbusAddr;					/* 42001 	 16bit integer   r/w modbus地址  */
	uint16		modbusDatabits;			/* 42002 	 16bit integer   r/w 数据位   */
	uint16		modbusParity;				/* 42003 	 16bit integer   r/w 奇偶校验 */
	uint32		modbusBaud;					/* 42004 	 32bit integer   r/w 波特率 */
	uint16		reserved[4];				/* 42006   reserved  保留*/
} COMM_SETTINGS_T;

typedef	struct measureSettings
{											/* Register		Type  		 R/W */
	uint16		sampleCycle;			  	/* 43001	 16bit integer	r/w 采样频率(单位：秒)  */
	float		reserved1;			   	    /* 43002	 32bit float	r/w   */
	float		reserved2;		          /* 43004 	 32bit float  r/w   */
	uint16		command;				    	/* 43006	 16bit integer	r/w */
	uint16		reserved[4];			    /* 43007   reserved  保留*/
} MEASURE_SETTINGS_T;


typedef	struct calibSettings
{																		/* Register		Type  		 R/W */
	uint16		reserved[14];				  /* 44001-44014  保留*/
} CALIB_SETTINGS_T;


typedef	struct filterSettings
{																		/* Register		Type  		 R/W */
	uint16		reserved[8];						/* 45004   reserved  保留*/
} FILTER_SETTINGS_T;

typedef struct measureValues
{																/* Register		Type  		 R/W */
	float		sensorValue;					/* 46001	32bit float		read  测量值*/
	float		sensorValue_mA;				/* 46003	32bit float		read  测量值(4-20ma形式)*/
	float   temperatureValue;			/* 46005	32bit float		read  温度(摄氏度)*/
	uint16		reserved[8];				/* 46006-46013  reserved  保留*/
} MEASURE_VALUES_T;

// Below are sensor specific registers, pH meter may have different definition
// All sensor specific registers start from 48001
// 暂时没开辟
typedef	struct sensorParam
{											/* Register		Type  		 R/W */	
	uint16 	led;						/* 48001	16bit integer	 r/w */
	uint16 	ledCurrent;			/* 48002	16bit integer	 r/w */
	uint16 	pdChannel;			/* 48003	16bit integer	 r/w */
	uint16 	sampleTimes;	  /* 48004	16bit integer	 r/w */
	uint16 	adcResult;			/* 48004	16bit integer	 r/w */
	uint16  dark;           /* 48006 */
	uint16  reserved[10];   /* 48007-48016 */
} SENSOR_PARAM_T;

#pragma pack()

/* Exported_Functions ---------------------------------------------------------*/


#endif
/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/
/*****************test for struct******************/

