/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 CAN interface for the data exchange between SC and actuator of L20 refresh.    
 
***************************************************************************/

#include "CommunicationInterfaceCAN.h"
#include "commons.h"
#include "Logger.h"

CommunicationInterface_CAN* CommunicationInterface_CAN::m_obj = nullptr;
SEM_ID CommunicationInterface_CAN::m_semaphoreMutex = SEM_ID_NULL;
CommunicationInterface_CAN* CommunicationInterface_CAN::GetInstance()
{
	return m_obj ? m_obj : (m_obj = new CommunicationInterface_CAN());
}

CommunicationInterface_CAN::CommunicationInterface_CAN() 
	:CommunicationInterface(CommunicationInterface::SENSOR_CAN_CONNECTION)
	,m_nStep(CommunicationInterface_CAN::LINK_DCAN)
{
	m_objImplementor = (EmrsnDCan*)GetCommunicationImp();
	if(m_objImplementor == nullptr)
	{
		LOGERR((char *)"DCan Socket:-------DCan initialization allocation failed-----------", 0, 0, 0);
	}
	/*create semaphore*/
	m_semaphoreMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);
}

CommunicationInterface_CAN::~CommunicationInterface_CAN() 
{
	if(m_objImplementor != nullptr)
	{
		closeConnection();
		delete m_objImplementor;
		m_objImplementor = nullptr;
	}
	
	if(m_semaphoreMutex != SEM_ID_NULL)
	{
		semDelete(m_semaphoreMutex);
		m_semaphoreMutex = SEM_ID_NULL;
	}
}

int CommunicationInterface_CAN::ErrorHandling()
{
	int iResult = ERROR;
	switch(m_nStep)
	{
	case CommunicationInterface_CAN::LINK_DCAN:
		if(establishDCANSocket() == true)
			m_nStep = CommunicationInterface_CAN::LINK_DONE;
		else
			m_nStep = CommunicationInterface_CAN::LINK_ERROR;
		break;
	case CommunicationInterface_CAN::LINK_DONE:
		iResult = OK;
		break;
	default:
		m_objImplementor->Close();
		m_nStep = CommunicationInterface_CAN::LINK_DCAN;
		break;
	}
	return iResult;
}

void CommunicationInterface_CAN::closeConnection()
{
	m_objImplementor->Close();
	LOGERR((char *)"DCan Socket: Close DCan Connection: DCAN socket close connection error",0,0,0);
}

bool CommunicationInterface_CAN::establishDCANSocket()
{
	if(m_objImplementor->Create() == ERROR)
		return false;
	if(m_objImplementor->Bind(1) == ERROR)
		return false;
	return true;
}

int CommunicationInterface_CAN::Sending(const void* data)
{
	char 	tmpBuffer[2] 	= {0};
	int 	tmpBufLen							= 0;
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	semTake(m_semaphoreMutex, WAIT_FOREVER); //lock sending

	if(buildProtocolPackage((TX_MESSAGE*)data, tmpBuffer, &tmpBufLen) == true)
	{
		if(m_objImplementor->Send(tmpBuffer, tmpBufLen) > 0)
			iResult = OK;
		else
		{
			LOGERR((char *)"DACN_Sending : --------Send Fail----------- : %d ", 0, 0, 0);
			iResult = ERROR;
		}
	}
	else
	{
		LOGERR((char *)"DACN_Sending : --------Build Fail----------- : %d ", 0, 0, 0);
		iResult = ERROR;
	}
	semGive(m_semaphoreMutex); //release sending
	return iResult;
}

int CommunicationInterface_CAN::Receiving(void* data)
{
	char 	tmpBuffer[6] = {0};
	int 	tmpBufLen	= 0;
	int		iResult		= ERROR;
	tmpBufLen = m_objImplementor->Read(tmpBuffer, 8);
	if(tmpBufLen > 0)
	{
		if(parseProtocolPackage(tmpBuffer, tmpBufLen, (RX_MESSAGE*)data) == false)
			LOGERR((char *)"DCAN Socket Invalid message received (length Error)", 0, 0, 0);
		else
			iResult = OK;
	}
	else
	{
		m_nStep = CommunicationInterface_CAN::LINK_ERROR;
	}

	return OK;
}

bool CommunicationInterface_CAN::parseProtocolPackage(const char* buffer, const int buflen, RX_MESSAGE* ptrMsg)
{
	unsigned short tmpData;
	bool bResult = false;
	int offset = 0, len = 0;
	if(buffer == nullptr)
		return false;
	if(buflen == 0)
		return false;
	if(ptrMsg == nullptr)
		return false;
	
	len += sizeof(UINT16);
	if(len > buflen)
		return false;
	for(int i = 0; i< buflen; i++)
	{
		LOG("data[%d] = 0x%x\n", i, buffer[i]);
	}
	memcpy(&ptrMsg->ADC_Height, buffer, sizeof(UINT16));
	offset += sizeof(UINT16);

	len += sizeof(UINT16);
	if(len > buflen)
		return false;
	memcpy(&ptrMsg->ADC_Width, buffer + offset, sizeof(UINT16));
	offset += sizeof(UINT16);

	len += sizeof(UINT16);
	if(len > buflen)
		return false;
	memcpy(&ptrMsg->ADC_Pressure, buffer + offset, sizeof(UINT));
	offset += sizeof(UINT16);
	
	ptrMsg->ADC_Height >>= 2;
	ptrMsg->ADC_Width >>= 2;
	ptrMsg->ADC_Pressure >>= 2;
	return true;
}

bool CommunicationInterface_CAN::buildProtocolPackage(const TX_MESSAGE* ptrMsg, char* buffer, int* buflen)
{
	DAC tmpDAC;
	tmpDAC.DAC_Cfg = 0;
	tmpDAC.DAC_Reg.Value = ptrMsg->DAC_Pressure & 0x03ff; ///* Actuator Board AD = 10BIT, This formula use HARD-CODED so that easy to understand*/
	tmpDAC.DAC_Reg.Speed = 0;
	tmpDAC.DAC_Reg.Power = 0;
	tmpDAC.DAC_Reg.Channel = 0;
	buffer[0] = tmpDAC.DAC_Cfg>>8;
	buffer[1] = tmpDAC.DAC_Cfg & 0xff;
	*buflen = sizeof(unsigned short);
	return true;
}
