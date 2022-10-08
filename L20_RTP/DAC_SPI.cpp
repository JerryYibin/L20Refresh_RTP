/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
***************************************************************************/

#include "DAC_SPI.h"
#include "Logger.h"
#include "commons.h"
#include <semLib.h>
SPI_HARDWARE 		DAC_TLV5604::m_Descriptor;
SPI_TRANSFER 		DAC_TLV5604::m_Transfer;
DAC_TLV5604::DAC	DAC_TLV5604::m_Register;
SEM_ID 				DAC_TLV5604::m_semaphoreMutex = SEM_ID_NULL;
DAC_TLV5604::DAC_TLV5604() {
	// TODO Auto-generated constructor stub
}

DAC_TLV5604::~DAC_TLV5604() {
	// TODO Auto-generated destructor stub
}

int DAC_TLV5604::InitDAConverter()
{
#if INCLUDE_TI_AM5708_JN
	m_Descriptor.chipSelect 	= SPI_SLAVE_1;
#elif INCLUDE_TI_AM5708_BRANSON
	m_Descriptor.chipSelect 	= SPI_SLAVE_3;
#endif
	m_Descriptor.bitWidth 		= NUM_BITS_PERTRANSFER_16;
	m_Descriptor.devFreq 		= 1000000;
	m_Descriptor.mode 			= SPI_CLK_MODE1 | SPI_FULL_DUPLEX;
	m_Descriptor.dataLines 	= 1;
	
	/*create semaphore*/
	m_semaphoreMutex = semMCreate(SEM_Q_FIFO);
	return OK;
}

int DAC_TLV5604::setDACValue(const int channel, const unsigned short data)
{
	unsigned char recvBuf[2] = {0};
	int iResult = ERROR;
	semTake(m_semaphoreMutex, WAIT_FOREVER); //lock sending
	m_Register.DAC_Reg.Channel 	= channel;
	m_Register.DAC_Reg.Power	= NormalOperation;
	m_Register.DAC_Reg.Speed	= FastMode;
	m_Register.DAC_Reg.Value	= data;
	m_Register.DAC_Reg.Reserved = 0;
	m_Transfer.txBuf = (unsigned char*)&m_Register.DAC_Cfg;
	m_Transfer.txLen = 2;
	m_Transfer.rxBuf = recvBuf;
	m_Transfer.rxLen = 2;
	if(vxbMcSpiTrans(SPI_PORT_1, &m_Descriptor, &m_Transfer) != OK)
	{
		LOGERR((char *)"failed to transfer date for setting DAC outputs", 0, 0, 0);
		iResult = ERROR;
	}
	else
		iResult = OK;
	semGive(m_semaphoreMutex); //release sending
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - The function works on the amplitude setting.
*
* \param   - Data - DAC hexadecimal value (expected value 0x00 to 0x3ff)
*
* \return  - OK/ERROR.
******************************************************************************/
int DAC_TLV5604::SetAmplitude(const unsigned short data)
{
	
	return setDACValue(AMPLITUDE_IN, 0x3ff & data);
}

/**************************************************************************//**
* 
* \brief   - The function works on the tune point setting.
*
* \param   - Data - DAC hexadecimal value (expected value 0x00 to 0x3ff)
*
* \return  - OK/ERROR.
******************************************************************************/
int DAC_TLV5604::SetTunePoint(const unsigned short data)
{
	return setDACValue(DIGPOT_WIPER, 0x3ff & data);
}

/**************************************************************************//**
* 
* \brief   - The function works on the frequency offset setting.
*
* \param   - Data - DAC hexadecimal value (expected value 0x00 to 0x3ff)
*
* \return  - OK/ERROR.
******************************************************************************/
int DAC_TLV5604::SetFrequencyOffset(const unsigned short data)
{
	return setDACValue(FREQ_OFFSET, 0x3ff & data);
}

/**************************************************************************//**
* 
* \brief   - The function works on the pressure of air regulator setting.
*
* \param   - Data - DAC hexadecimal value (expected value 0x00 to 0x3ff)
*
* \return  - OK/ERROR.
******************************************************************************/
int DAC_TLV5604::SetPressure(const unsigned short data)
{
	return setDACValue(PRESSURE_SET, 0x3ff & data);
}

/**************************************************************************//**
* 
* \brief   - The function works on the semaphore mutex release.
*
* \param   - None
*
* \return  - OK/ERROR.
******************************************************************************/
int DAC_TLV5604::ReleaseDACConverter()
{
	if(m_semaphoreMutex != SEM_ID_NULL)
	{
		semDelete(m_semaphoreMutex);
		m_semaphoreMutex = SEM_ID_NULL;
	}
	return OK;
}
