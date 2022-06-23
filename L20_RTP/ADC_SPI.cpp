/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/
#include <taskLib.h>
#include "ADC_SPI.h"
#include "Logger.h"
#include "stdio.h"
SPI_HARDWARE 	ADC_AD7689::m_Descriptor;
SPI_TRANSFER 	ADC_AD7689::m_Transfer;
ADC_AD7689::ADC	ADC_AD7689::m_Register;

unsigned int				ADC_AD7689::m_ADPowerAvg	[COUNTER] = {0};
unsigned int				ADC_AD7689::m_ADFreqAvg		[COUNTER] = {0};
unsigned int				ADC_AD7689::m_ADAmpAvg		[COUNTER] = {0};
unsigned int				ADC_AD7689::m_ADMemoryAvg	[COUNTER] = {0};
unsigned int				ADC_AD7689::m_ADRefAvg		[COUNTER] = {0};
unsigned int				ADC_AD7689::m_Counter				  = 0;

ADC_AD7689::ADC_AD7689() {
	// TODO Auto-generated constructor stub

}

ADC_AD7689::~ADC_AD7689() {
	// TODO Auto-generated destructor stub
}

unsigned int ADC_AD7689::getAverageFilter(const unsigned int* pointer)
{
	unsigned int lowest = 0, highest = 0, data = 0;
	for(int i = 0; i < COUNTER; i++)
	{
		if(i == 0)
		{
			lowest = pointer[i];
			highest = pointer[i];
		}
		else if(lowest > pointer[i])
		{
			lowest = pointer[i];
		}
		else if(highest < pointer[i])
		{
			highest = pointer[i];
		}
		else
		{
			
		}
		data += pointer[i];
	}
	data -= (lowest + highest);
	return data/(COUNTER - 2);
}

int ADC_AD7689::InitADConverter()
{
	m_Register.ADC_Reg.CFG 	= Overwrite;
	m_Register.ADC_Reg.INCC = UniRef2GND;
	m_Register.ADC_Reg.INx 	= CH_7;
	m_Register.ADC_Reg.BW 	= Full;
	m_Register.ADC_Reg.REF 	= ExtRef;
	m_Register.ADC_Reg.SEQ 	= ScanChannel;
	m_Register.ADC_Reg.RB 	= NoRead;
	
	m_Descriptor.chipSelect 	= SPI_SLAVE_0;
	m_Descriptor.bitWidth 		= NUM_BITS_PERTRANSFER_16;
	m_Descriptor.devFreq 		= 40000000;
	m_Descriptor.mode 			= SPI_MODE_3 | SPI_FULL_DUPLEX;
	m_Descriptor.dataLines 		= 1;
	return OK;
}

int ADC_AD7689::RunADCSample()
{
	unsigned short recvData = 0;
	m_Descriptor.mode 	= SPI_MODE_3 | SPI_FULL_DUPLEX | SPI_POLLING;
	m_Transfer.txBuf 	= (unsigned char*)&m_Register.ADC_Cfg;
	m_Transfer.txLen 	= 2;
	m_Transfer.rxBuf 	= (unsigned char*)&recvData;
	m_Transfer.rxLen 	= 2;
	
	if(vxbMcSpiTrans(SPI_PORT_1, &m_Descriptor, &m_Transfer) != OK)
	{
		LOGERR((char *)"failed to transfer date for setting ADC inputs", 0, 0, 0);
		return ERROR;
	}
//	taskDelay(1);
	
	unsigned short dummy = 0;
	m_Transfer.txBuf 	= (unsigned char*)&dummy;
	m_Descriptor.mode	= SPI_MODE_2 | SPI_FULL_DUPLEX | SPI_POLLING;
	recvData 			= 0;
	if(vxbMcSpiTrans(SPI_PORT_1, &m_Descriptor, &m_Transfer) != OK)
	{
		LOGERR((char *)"failed to transfer date for setting ADC inputs", 0, 0, 0);
		return ERROR;
	}

	
	for(int ch = 0; ch < CH_MAX; ch++)
	{
		dummy 				= 0;
		recvData 			= 0;
		if(vxbMcSpiTrans(SPI_PORT_1, &m_Descriptor, &m_Transfer) != OK)
		{
			LOGERR((char *)"failed to transfer date for setting ADC inputs", 0, 0, 0);
			return ERROR;
		}
		else
		{
			switch(ch)
			{
			case CH_0:
				m_ADFreqAvg[m_Counter] = recvData;
				break;
			case CH_1:
				m_ADAmpAvg[m_Counter] = recvData;
				break;
			case CH_2:
				m_ADPowerAvg[m_Counter] = recvData;
				break;
			case CH_3:
				m_ADMemoryAvg[m_Counter] = recvData;
				break;
			case CH_4:
				m_ADRefAvg[m_Counter] = recvData;
				break;
			default:
				break;
				
			}

//			LOG("CH[%d] = 0x%x ", ch, recvData);

			m_Counter++;
			if(m_Counter == COUNTER)
				m_Counter = 0;
		}
	}
	return OK;
}

unsigned short ADC_AD7689::GetAmplitude()
{
	return getAverageFilter(m_ADAmpAvg);
}

unsigned short ADC_AD7689::GetPower()
{
	return getAverageFilter(m_ADPowerAvg);
}
unsigned short ADC_AD7689::GetFrequency()
{
	return getAverageFilter(m_ADFreqAvg);
}

unsigned short ADC_AD7689::GetMemory()
{
	return getAverageFilter(m_ADMemoryAvg);
}

unsigned short ADC_AD7689::GetReference()
{
	return getAverageFilter(m_ADRefAvg);
}
