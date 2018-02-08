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
    IDLE=0,              
		CALIB_SUCCESS,
	  NO_STD,  //未输入标样值或者输入的标样值不在校准范围内
		STD_ERR				//标样错误
} CalibState;

//2 byte aligned
#pragma pack(2)//

typedef	struct sysStatusOld
{											/* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	16bit integer	read  运行状态(1正常 ，其余值故障) */
	uint16   	commStatus;					/* 41002	16bit integer	read  通信状态(1正常 ，其余值故障) */
	uint16 		calibStatus;				/* 41003 	16bit integer   read  标定状态(0空闲,1标定中,2标定已完成,3标定失败)*/
	uint16		configStatus;				/* 41004	16bit integer	read  配置状态(0系统未配置,1系统已配置)*/
	uint16		sensorType;					/* 41005	16bit integer	read  探头类型 1ph 2orp 3ph&orp 4电导*/	
	uint32		serialNum;					/* 41006	32bit integer	read  序列号 */
	uint16		softwareVer;				/* 41008	16bit integer	read  硬件版本*/
	uint16		softwareRev;				/* 41009	16bit integer	read  软件版本*/
	uint16		hardwareVer;				/* 41010	16bit integer	read  软件修订*/
	uint16		reserved[4];				/* reserved  保留*/
} SYS_STATUS_T_OLD;

typedef	struct sysStatus
{											/* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	16bit integer	read  运行状态(1正常 ，其余值故障) */
	uint16   	commStatus;					/* 41002	16bit integer	read  通信状态(1正常 ，其余值故障) */
	uint16 		calibStatus;				/* 41003 	16bit integer read  标定状态(0空闲,1标定中,2标定已完成,3标定失败)*/
	uint16		configStatus;				/* 41004	16bit integer	read  配置状态(0系统未配置,1系统已配置)*/
	uint32		productNum;					/* 41005	16bit integer	read  r read */
	char		deviceName[16];       /* 41007	128bit ch探头类型 1ph 2orp 3ph&orp 4电导*/	
	char		serial[16];           /* 41015	128bit chaar read */
	char		hardwareVer[16];      /* 41023	128bit char read */
	char		softwareVer[16];      /* 41031	128bit char read */
	uint16  newStructFlg;					/* 41039  16bit integer read */
	uint16		reserved[7];				/* 41040-41046  reserved */
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
	float		measureRange;			   	  /* 43002	 32bit float	r/w  最大量程(e.g,300) */
	float		smoothingFactor;		    /* 43004 	 32bit float  r/w    滑动平均系数(取值范围0-1) */
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
	float   v1;										/* 46007  32bit float   通道6采集的电压 */
	float   v2;										/* 46009  32bit float   通道7采集的电压 */
	uint16		reserved[4];				/* 46011  1eserved  保留*/
} MEASURE_VALUES_T;


// Below are sensor specific registers, pH meter may have different definition
// All sensor specific registers start from 48001
// 暂时没开辟
typedef	struct sensorParam
{											/* Register		Type  		 R/W */	
	float  	slope;						/* 48001	32bit float		 r/w 斜率 */
	float  	v0_sum;						/* 48003	32bit float		 r/w 秤上不放物品时的电压值 ，单位 V*/
	float  	weight_20mA;			/* 48005	32bit float		 r/w 4-20mA转换时20mA对应的测量值，单位 g */
	float   calibSolution;    /* 48007	32bit float	   r/w */
	uint16 	s365Di;						/* 48009	16bit integer	 r/w */
	uint16 	t420Di;						/* 48010	16bit integer	 r/w */
	uint16 	s420Di;						/* 48011	16bit integer	 r/w */
	uint16 	t365;							/* 48012	16bit integer	 r/w */
	uint16 	s365;							/* 48013	16bit integer	 r/w */
	uint16 	t420;							/* 48014	16bit integer	 r/w */
	uint16 	s420;							/* 48015	16bit integer	 r/w */
	uint16 	cT365;						/* 48016	16bit integer	 r/w */
	uint16 	cS365;						/* 48017	16bit integer	 r/w */
	uint16 	cT420;						/* 48018	16bit integer	 r/w */
	uint16  cS420;						/* 48019	16bit integer	 r/w */	
} SENSOR_PARAM_T;
#pragma pack()

/* Exported_Functions ---------------------------------------------------------*/


#endif
/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/
/*****************test for struct******************/

