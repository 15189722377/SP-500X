/**
  ******************************************************************************
  * @file    embreg.c
  * @author  AndyChen
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  *  
  ******************************************************************************
  */ 

/* Includes -------------------------------------------------------------------*/

#include "embreg.h"
#include "app.h"
extern u8 bitmodbus;

/* Private_Macros -------------------------------------------------------------*/

//�Ĵ�����ַ���
#define INDEX_BASE	1000	

/* Private_TypesDefinitions ---------------------------------------------------*/

/* Private_Variables ----------------------------------------------------------*/	

/* Private_Functions ----------------------------------------------------------*/

/**
  * @brief  ������Ĵ���
  * @param  pucRegBuffer����������ָ��
			usAddress	������Ĵ�����ʼ��ַ
			usNRegs		��Ҫ���ļĴ�����Ŀ		
  * @retval �������
  */
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	volatile u8 test;
	uint16_t iRegIndex,uRegs;
	uint16_t *data_pointer;
	eMBErrorCode eStatus = MB_ENOERR;

	uRegs= usAddress/INDEX_BASE;

	//���ݵ�ַȷ��Ҫ���ʵļĴ���
	switch( uRegs )
	{
		//ϵͳ״̬�Ĵ���
		case SYSREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= SYSREG_SADDR + SYSREG_NREGS)
			{
				data_pointer= (void *)&system_status;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
		
		//�������ݼĴ���
		case MVREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= MVREG_SADDR + MVREG_NREGS)
			{
				data_pointer= (void *)&measure_values;
			}
			else
			{
				eStatus=MB_ENOREG;			
			}
			break;
			
		//ר�мĴ���
		case SENSORREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= SENSORREG_SADDR + SENSORREG_NREGS)
			{
				data_pointer= (void *)&sensor_param;
			}
			else
			{
				eStatus=MB_ENOREG;			
			}
			break;
		
		//�Ƿ���ַ
		default:
			eStatus=MB_ENOREG;
			break;
	}
	
	if( eStatus != MB_ENOREG )
	{		
		iRegIndex= usAddress % INDEX_BASE;
		iRegIndex--;		
		
		//ѭ��������
		while( usNRegs > 0 )
		{	
			//��ȡ��λ�ֽ�
			*pucRegBuffer++ = ( uint8_t )( *(data_pointer + iRegIndex) >> 8 );				
			
			//��ȡ��λ�ֽ�
			*pucRegBuffer++ = ( uint8_t )( *(data_pointer + iRegIndex) & 0xFF );				
			
			iRegIndex++;
			
			usNRegs--;
		}
	}	

	return eStatus;
}

/**
  * @brief  ��/д���ּĴ���
  * @param  pucRegBuffer������ָ��
			usAddress	������Ĵ�����ʼ��ַ
			usNRegs		��Ҫ�����ļĴ�����Ŀ	
			eMode		: �Ա��ּĴ����Ĳ���������д��
  * @retval �������
  */
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode )
{	
	uint16_t iRegIndex,uRegs;
	uint16_t *data_pointer;
	eMBErrorCode eStatus = MB_ENOERR;		
	
	uRegs= usAddress/INDEX_BASE;
	
	//���ݵ�ַȷ��Ҫ���ʵļĴ���
	switch( uRegs )
	{
				//ϵͳ״̬�Ĵ���
		case SYSREG_FLAG:             //41001
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= SYSREG_SADDR + SYSREG_NREGS)
			{
				data_pointer= (void *)&system_status;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
		//ͨ�Ų������üĴ���            //42001
		case COMSREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= COMSREG_SADDR + COMSREG_NREGS)
			{
				data_pointer= (void *)&comm_settings;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
			
		
		//�������üĴ���               //43001
		case MSREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= MSREG_SADDR + MSREG_NREGS)
			{
				data_pointer= (void *)&measure_settings;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
			
		
		//У׼���üĴ���            //44001
		case CALSREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= CALSREG_SADDR + CALSREG_NREGS)
			{
				data_pointer= (void *)&calib_settings;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
			
		//�˲����������üĴ���     //45001
		case FSREG_FLAG:
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= FSREG_SADDR + FSREG_NREGS)
			{
				data_pointer= (void *)&filter_settings;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
			
		//�豸ר�мĴ���
		case SENSORREG_FLAG:
			if(usAddress + usNRegs <= SENSORREG_SADDR + SENSORREG_NREGS)
			{
				data_pointer= (void *)&sensor_param;
			}
			else
			{
				eStatus=MB_ENOREG;
			}
			break;
			
			//�������ݼĴ���
		case MVREG_FLAG:              //46001
			//�жϵ�ַ�ͼĴ�����Ŀ�Ϸ���
			if(usAddress + usNRegs <= MVREG_SADDR + MVREG_NREGS)
			{
				data_pointer= (void *)&measure_values;
			}
			else
			{
				eStatus=MB_ENOREG;			
			}
			break;
		
		//�Ƿ���ַ
		default:
			eStatus=MB_ENOREG;
			break;
	}
	//data_pointer++;  //
	if(eStatus != MB_ENOREG)
	{		
		iRegIndex= usAddress % INDEX_BASE;
		//iRegIndex--;
		
		switch(eMode)
		{
			//�����ּĴ���
			case MB_REG_READ:					
				while( usNRegs > 0 )
				{						
					//��ȡ��λ�ֽ�
					*pucRegBuffer++ = ( uint8_t )( *(data_pointer + iRegIndex) >> 8 );				
					
					//��ȡ��λ�ֽ�
					*pucRegBuffer++ = ( uint8_t )( *(data_pointer +iRegIndex) & 0xFF );				
					
					iRegIndex++;
					
					usNRegs--;
				}
				break;
			//д���ּĴ���
			case MB_REG_WRITE:
				while( usNRegs > 0 )
				{
					*(data_pointer + iRegIndex) = *pucRegBuffer++ << 8;
					*(data_pointer + iRegIndex) |= *pucRegBuffer++;
					iRegIndex++;
					usNRegs--;
				}	
				bitmodbus=1;
				break;		

			default:
				break;
		}
	}

	return eStatus;
}

/**
  * @brief  ��/д��Ȧ�Ĵ���
  * @param  pucRegBuffer������ָ��
			usAddress	���Ĵ�����ʼ��ַ
			usNCoils	��Ҫ��������Ȧ��Ŀ	
			eMode		������ģʽ
  * @retval �������
  */
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,eMBRegisterMode eMode )
{	
	return MB_ENOREG;
}

/**
  * @brief  ����ɢ�Ĵ���
  * @param  pucRegBuffer����������ָ��
			usAddress	���Ĵ�����ʼ��ַ
			usNDiscrete	��Ҫ���ļĴ�����Ŀ		
  * @retval �������
  */
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{	
	return MB_ENOREG;
}

/******************* (C) COPYRIGHT 2015 AndyChen *******END OF FILE*************/