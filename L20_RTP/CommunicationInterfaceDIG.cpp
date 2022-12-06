/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------
This Module handle the socket of TCP/IP Server being established,
and binary data Package Build & Send/Receive & processing
**********************************************************************************************************/

#include "CommunicationInterfaceDIG.h"
#include "Connectivity.h"
SEM_ID CommunicationInterfaceDIG::DIGSendMutex = SEM_ID_NULL;
CommunicationInterfaceDIG* CommunicationInterfaceDIG::_objComDIG = nullptr;

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
CommunicationInterfaceDIG* CommunicationInterfaceDIG::GetInstance()
{
	return (_objComDIG != nullptr) ? _objComDIG : (_objComDIG = new CommunicationInterfaceDIG());
}


/**************************************************************************//**
*
* \brief   - Constructor Initialize current communication as the External socket connection.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterfaceDIG::CommunicationInterfaceDIG()
	:CommunicationInterface(CommunicationInterface::DIG_SOCKET_CONNECTION)
	,m_nStep(CommunicationInterfaceDIG::LINK_CONFIG)
{
	m_objImplementor = (EmrsnSocket*)GetCommunicationImp();
	if(m_objImplementor == nullptr)
	{
		LOGERR((char *)"DIG Socket:-------TCP/IP initialization allocation failed-----------", 0, 0, 0);
	}
	/*create semaphore*/
	DIGSendMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);
}


/**************************************************************************//**
*
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterfaceDIG::~CommunicationInterfaceDIG()
{
	closeConnection();

	if(DIGSendMutex != SEM_ID_NULL)
	{
		semDelete(DIGSendMutex);
		DIGSendMutex = SEM_ID_NULL;
	}
}

/**************************************************************************//**
*
* \brief   - Connection line rebuilding
*
* \param   - None.
*
* \return  - if the link connection is successful, it will return 0; else it will return -1;
******************************************************************************/
int CommunicationInterfaceDIG::ErrorHandling()
{
	int iResult = ERROR;
	switch(m_nStep)
	{
	case CommunicationInterface::LINK_CONFIG:
		if(linkConfig() == true)
			m_nStep = CommunicationInterface::LINK_TCPIP;
		break;
	case CommunicationInterface::LINK_TCPIP:
		if(establishTCPIPSocket() == true)
			m_nStep = CommunicationInterface::LINK_DONE;
		else
			m_nStep = CommunicationInterface::LINK_ERROR;
		break;
	case CommunicationInterface::LINK_DONE:
		iResult = OK;
		break;
	default:
		closeConnection();
		m_nStep = CommunicationInterface::LINK_TCPIP;
		break;
	}
	return iResult;
}

/**************************************************************************//**
*
* \brief   - Data Sending
*
* \param   - data, it should be the specific data structure pointer passed by the recall function. 
*
* \return  - If the sending is successful, it will return 0; else it will return -1.
******************************************************************************/
int CommunicationInterfaceDIG::Sending(const void* data)
{
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	semTake(DIGSendMutex, WAIT_FOREVER); //lock sending
	if(m_nStep == CommunicationInterface::LINK_DONE)
	{
		if(((DIG_MESSAGE*)data)->msgLen >= MAX_SIZE_OF_MSG_BUFF)
		{
			LOGERR( (char *)"DIG Socket SendMessageToServer, Socket message length exceeded : %d", ((DIG_MESSAGE*)data)->msgLen, 0, 0);
		}
		else
		{  	
			if(buildDIGSendingPackage(((DIG_MESSAGE*)data)->Buffer, (unsigned short *)&(((DIG_MESSAGE*)data)->msgLen),((DIG_MESSAGE*)data)->msgID))
			{
				if(m_objImplementor->Send(((DIG_MESSAGE*)data)->Buffer, ((DIG_MESSAGE*)data)->msgLen) > 0)
					iResult = OK;
			}
		}
	}
	semGive(DIGSendMutex); //release sending
	return iResult;
}

/**************************************************************************//**
*
* \brief   - Data Receiving
*
* \param   - data, it should be the specific data structure pointer passed by the recall function.
*
* \return  - If a integrated data is received, it will return 0; else it will return -1;.
******************************************************************************/
int CommunicationInterfaceDIG::Receiving(void* data)
{	
	char tmpBuffer[MAX_RX] 	= {0};
	int tmpRecvlen			= 0;
	int	iResult				= ERROR;
	if(data == nullptr)
		return iResult;
	if(m_nStep == CommunicationInterface::LINK_DONE)
	{
		memset(tmpBuffer, 0x00, sizeof(tmpBuffer));
		tmpRecvlen = m_objImplementor->NBRead(tmpBuffer, MAX_RX, false); //Use Non-blocking reading
		if(tmpRecvlen > 0)
		{	
			memcpy(data, tmpBuffer, sizeof(tmpRecvlen));
	#if DIG_Test
			for(int i=0;i<MSG_LEN;i++)
				LOG("data = %d\n",tmpBuffer[i]);
			LOG("\nReceive len  = %d\n",tmpRecvlen);
	#endif
			if(parseProtocolPackage(tmpBuffer, tmpRecvlen, (char*)data) == false)
				LOGERR((char *)"DIG Socket Invalid message received\n", 0, 0, 0);
			else
				iResult = OK;
		}
		else if(tmpRecvlen < 0)
		{
			m_nStep = CommunicationInterface::LINK_ERROR;
		}	
	}
	return iResult;
}

/**************************************************************************//**
*
* \brief   - TCP/IP socket link establishing.
*
* \param   - none
*
* \return  - return bResult
*
******************************************************************************/
bool CommunicationInterfaceDIG::establishTCPIPSocket()
{
	INT32 iPort = DIG_SOCKET_PORT;
	const char * address = DIG_SERVER_ADDRESS;
	const int timeout = 1;
	bool bResult = false;
	GATEWAY_MACHINE temp_machine;
		
	auto iter = Connectivity::_DIGMachinesSC->find(Connectivity::EthernetConfig.DIG_MachineID);
	if (iter != Connectivity::_DIGMachinesSC->end())
	{
		temp_machine = iter->second;
		iPort = temp_machine.DIG_MachinePort;
	}
	
	if(m_objImplementor->Create() == false)
	{
		LOGERR((char*) "DIG Socket creation failed\n", 0, 0, 0);
		return false;
	}

	if(m_objImplementor->NBConnect(address,iPort,timeout) == false)
	{
		LOGERR((char*) "DIG Socket Connect to Server failed\n", 0, 0, 0);
	}
	else
	{
		LOG("DIG Socket Connect to Server on Address %s with port %d\n", address, iPort);
		bResult = true;
	}
	m_objImplementor->AliveCheck();
	return bResult;
}


/**************************************************************************//**
*
* \brief   - Closes the socket descriptor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void CommunicationInterfaceDIG::closeConnection()
{
	m_objImplementor->Close();
}

/**************************************************************************//**
*
* \brief   - Configure the IP address and subMask address of cpsw0
*
* \param   - None
* 
* \return  - OK or ERROR
*
******************************************************************************/

bool CommunicationInterfaceDIG::linkConfig()
{
	bool bResult = false;
	char ip_temp[16];
	char ip_arr[16] = {"150.150.150.10"};
	char mask_addr[16] = {"255.255.255.0"};
	GATEWAY_MACHINE temp_machine;
	
	auto iter = Connectivity::_DIGMachinesSC->find(Connectivity::EthernetConfig.DIG_MachineID);
	if (iter != Connectivity::_DIGMachinesSC->end())
	{
		temp_machine = iter->second;
		memcpy(ip_arr, temp_machine.DIG_MachineIP, IP_ADD_LEN);
	}
	
	m_objImplementor->GetIPAddress(ip_temp);

	if(strcmp(ip_temp,ip_arr)!=0)
	{
		if(m_objImplementor->SetIpConfig((char *)"cpsw0",ip_arr, mask_addr)==OK)
			bResult = true;
	}
	else
		bResult = true;
	
	return bResult;
}

/**************************************************************************//**
*
* \brief   - Build DIG sending array According to the required data type
*
* \param   - None.
*
* \return  - if the link connection is successful, it will return 0; else it will return -1;
******************************************************************************/

bool CommunicationInterfaceDIG::buildDIGSendingPackage(char *transBuffer, unsigned short* buflen, int datatype)
{   
	//TODO:To be Implemented
	return true;
}

/**************************************************************************//**
*
* \brief   - Decode the DIG Receiving array According to the required data type
*
* \param   - None.
*
* \return  - if the link connection is successful, it will return 0; else it will return -1;
******************************************************************************/

bool CommunicationInterfaceDIG::parseProtocolPackage(const char* buffer, const int buflen,	const char* ptrMsg)
{
	//TODO:To be Implemented
	return true;
}

/**************************************************************************//**
*
* \brief   - Close Socket
*
* \param   - None.
*
* \return  - if the link connection is successful, it will return 0; else it will return -1;
******************************************************************************/
void CommunicationInterfaceDIG::Close()
{
	closeConnection();
	m_nStep = CommunicationInterface::LINK_CONFIG;
}


/**************************************************************************//**
*
* \brief   - Get the current step of the Communication Socket LINK 
*
* \param   - None.
*
* \return  - return the COMMUNICATION_LINK ID
******************************************************************************/
int CommunicationInterfaceDIG::GetLinkStepIndex()
{
	return m_nStep;
}

