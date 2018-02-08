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
	  NO_STD,  //δ�������ֵ��������ı���ֵ����У׼��Χ��
		STD_ERR				//��������
} CalibState;

//2 byte aligned
#pragma pack(2)//

typedef	struct sysStatusOld
{											/* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	16bit integer	read  ����״̬(1���� ������ֵ����) */
	uint16   	commStatus;					/* 41002	16bit integer	read  ͨ��״̬(1���� ������ֵ����) */
	uint16 		calibStatus;				/* 41003 	16bit integer   read  �궨״̬(0����,1�궨��,2�궨�����,3�궨ʧ��)*/
	uint16		configStatus;				/* 41004	16bit integer	read  ����״̬(0ϵͳδ����,1ϵͳ������)*/
	uint16		sensorType;					/* 41005	16bit integer	read  ̽ͷ���� 1ph 2orp 3ph&orp 4�絼*/	
	uint32		serialNum;					/* 41006	32bit integer	read  ���к� */
	uint16		softwareVer;				/* 41008	16bit integer	read  Ӳ���汾*/
	uint16		softwareRev;				/* 41009	16bit integer	read  �����汾*/
	uint16		hardwareVer;				/* 41010	16bit integer	read  �����޶�*/
	uint16		reserved[4];				/* reserved  ����*/
} SYS_STATUS_T_OLD;

typedef	struct sysStatus
{											/* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	16bit integer	read  ����״̬(1���� ������ֵ����) */
	uint16   	commStatus;					/* 41002	16bit integer	read  ͨ��״̬(1���� ������ֵ����) */
	uint16 		calibStatus;				/* 41003 	16bit integer read  �궨״̬(0����,1�궨��,2�궨�����,3�궨ʧ��)*/
	uint16		configStatus;				/* 41004	16bit integer	read  ����״̬(0ϵͳδ����,1ϵͳ������)*/
	uint32		productNum;					/* 41005	16bit integer	read  r read */
	char		deviceName[16];       /* 41007	128bit ch̽ͷ���� 1ph 2orp 3ph&orp 4�絼*/	
	char		serial[16];           /* 41015	128bit chaar read */
	char		hardwareVer[16];      /* 41023	128bit char read */
	char		softwareVer[16];      /* 41031	128bit char read */
	uint16  newStructFlg;					/* 41039  16bit integer read */
	uint16		reserved[7];				/* 41040-41046  reserved */
} SYS_STATUS_T;

typedef struct commSettings
{											/* Register		Type  		 R/W */
	uint16		modbusAddr;					/* 42001 	 16bit integer   r/w modbus��ַ  */
	uint16		modbusDatabits;			/* 42002 	 16bit integer   r/w ����λ   */
	uint16		modbusParity;				/* 42003 	 16bit integer   r/w ��żУ�� */
	uint32		modbusBaud;					/* 42004 	 32bit integer   r/w ������ */
	uint16		reserved[4];				/* 42006   reserved  ����*/
} COMM_SETTINGS_T;

typedef	struct measureSettings
{											/* Register		Type  		 R/W */
	uint16		sampleCycle;			  	/* 43001	 16bit integer	r/w ����Ƶ��(��λ����)  */
	float		measureRange;			   	  /* 43002	 32bit float	r/w  �������(e.g,300) */
	float		smoothingFactor;		    /* 43004 	 32bit float  r/w    ����ƽ��ϵ��(ȡֵ��Χ0-1) */
	uint16		command;				    	/* 43006	 16bit integer	r/w */
	uint16		reserved[4];			    /* 43007   reserved  ����*/
} MEASURE_SETTINGS_T;


typedef	struct calibSettings
{																		/* Register		Type  		 R/W */
	uint16		reserved[14];				  /* 44001-44014  ����*/
} CALIB_SETTINGS_T;


typedef	struct filterSettings
{																		/* Register		Type  		 R/W */
	uint16		reserved[8];						/* 45004   reserved  ����*/
} FILTER_SETTINGS_T;

typedef struct measureValues
{																/* Register		Type  		 R/W */
	float		sensorValue;					/* 46001	32bit float		read  ����ֵ*/
	float		sensorValue_mA;				/* 46003	32bit float		read  ����ֵ(4-20ma��ʽ)*/
	float   temperatureValue;			/* 46005	32bit float		read  �¶�(���϶�)*/
	float   v1;										/* 46007  32bit float   ͨ��6�ɼ��ĵ�ѹ */
	float   v2;										/* 46009  32bit float   ͨ��7�ɼ��ĵ�ѹ */
	uint16		reserved[4];				/* 46011  1eserved  ����*/
} MEASURE_VALUES_T;


// Below are sensor specific registers, pH meter may have different definition
// All sensor specific registers start from 48001
// ��ʱû����
typedef	struct sensorParam
{											/* Register		Type  		 R/W */	
	float  	slope;						/* 48001	32bit float		 r/w б�� */
	float  	v0_sum;						/* 48003	32bit float		 r/w ���ϲ�����Ʒʱ�ĵ�ѹֵ ����λ V*/
	float  	weight_20mA;			/* 48005	32bit float		 r/w 4-20mAת��ʱ20mA��Ӧ�Ĳ���ֵ����λ g */
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
