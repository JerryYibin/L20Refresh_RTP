/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------
This Module handle the socket of TCP/IP Server being established,
and binary data Package Build & Send/Receive & processing
**********************************************************************************************************/

#include "CommunicationInterfaceTCP.h"
#include "commons.h"
#include "Logger.h"
#include "Connectivity.h"

SEM_ID CommunicationInterfaceTCP::TCPSendMutex = SEM_ID_NULL;
CommunicationInterfaceTCP* CommunicationInterfaceTCP::_objComTCP = nullptr;

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
CommunicationInterfaceTCP* CommunicationInterfaceTCP:: GetInstance()
{
	return (_objComTCP != nullptr) ? _objComTCP : (_objComTCP = new CommunicationInterfaceTCP());
}
/**************************************************************************//**
*
* \brief   - Constructor Initialize current communication as the TCP/IP socket connection.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterfaceTCP::CommunicationInterfaceTCP()
	:CommunicationInterface(CommunicationInterface::TCP_SOCKET_CONNECTION)
	,m_nDescriptor(ERROR),m_nStep(CommunicationInterface::LINK_CONFIG)
{
	m_objImplementor = (EmrsnSocket*)GetCommunicationImp();
	if(m_objImplementor == nullptr)
	{
		LOGERR((char *)"TCP Socket:-------TCP/IP initialization allocation failed-----------", 0, 0, 0);
	}
		/*create semaphore*/
	TCPSendMutex = semMCreate(SEM_Q_FIFO);
}

/**************************************************************************//**
*
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterfaceTCP::~CommunicationInterfaceTCP()
{
	closeConnection();
	if(TCPSendMutex != SEM_ID_NULL)
	{
		semDelete(TCPSendMutex);
		TCPSendMutex = SEM_ID_NULL;
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
int CommunicationInterfaceTCP::ErrorHandling()
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
		{
			m_nStep = CommunicationInterface::LINK_DONE;
		}
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
* \param   - data, it should be the specific data structure pointer passed by the recall function. .
*
* \return  - If the sending is successful, it will return 0; else it will return -1.
******************************************************************************/
int CommunicationInterfaceTCP::Sending(const void* data)
{
	char 	tmpBuffer[MAX_SIZE_OF_MSG_LENGTH] 	= {0};
	int 	tmpBufLen							= 0;
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	semTake(TCPSendMutex, WAIT_FOREVER); //lock sending
	if(((TX_MESSAGE*)data)->Length >= MAX_EXTERN_MSG_BUFF)
	{
		LOGERR( (char *)"TCP Socket SendMessageToClient, Socket message length exceeded : %d", ((TX_MESSAGE*)data)->Length, 0, 0);
	}
	else
	{
		if(buildProtocolPackage((TX_MESSAGE*)data, tmpBuffer, &tmpBufLen) == true)
		{
			if(m_objImplementor->Send(tmpBuffer, tmpBufLen) > 0)
				iResult = OK;
		}
	}
	semGive(TCPSendMutex); //release sending
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
int CommunicationInterfaceTCP::Receiving(void* data)
{
	char tmpBuffer[MAX_RX] 	= {0};
	int tmpRecvlen			= 0;
	int	iResult				= ERROR;
	if(data == nullptr)
		return iResult;
	if(m_nStep == CommunicationInterfaceTCP::LINK_DONE)
	{
		memset(tmpBuffer, 0x00, sizeof(tmpBuffer));
		tmpRecvlen = m_objImplementor->NBRead(tmpBuffer, MAX_RX,true); //Use Non-blocking reading
		if(tmpRecvlen > 0)
		{	
			memcpy(data, tmpBuffer, sizeof(tmpRecvlen));
#if TCP_Test
			for(int i=0;i<MSG_LEN;i++)
				LOG("data = %d\n",tmpBuffer[i]);
			LOG("\nReceive len  = %d\n",tmpRecvlen);
#endif
			if(parseProtocolPackage(tmpBuffer, tmpRecvlen, (char*)data) == false)
				LOGERR((char *)"TCP Socket Invalid message received\n", 0, 0, 0);
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
bool CommunicationInterfaceTCP::establishTCPIPSocket()
{
	INT32 iPort = Connectivity::EthernetConfig.TCP_ServerPort;
	char* ip = Connectivity::EthernetConfig.TCP_ServerIP;

	int bResult = false;

	if(m_objImplementor->Create() == false)
	{
		LOGERR((char*) "TCP Socket creation failed", 0, 0, 0);
		return false;
	}

	if(m_objImplementor->Bind(ip, iPort) == false)
	{
		LOGERR((char*) "TCP Socket binding failed", 0, 0, 0);
		return false;
	}

	if(m_objImplementor->Listen() == false)
	{
		LOGERR((char*) "TCP Socket listening failed", 0, 0, 0);
		return false;
	}

	LOG("TCP Socket listening on IP %s, Port: %d\n", ip, iPort);

	// stay in accept mode until proper connection or error
	m_nDescriptor = m_objImplementor->NBAccept(ip);

	if(m_nDescriptor == ERROR)
	{
		LOGERR((char *) "TCP Socket Error on socket accept\n", 0, 0, 0);
		return false;
	}
	else
	{
		LOG("TCP Socket connection established, Client_SockId: %d\n", m_nDescriptor);
		bResult = true;
	}
	m_nDescriptor = m_objImplementor->AliveCheck(true);
	return bResult;
}
/**************************************************************************//**
*
* \brief   - Closes the client socket descriptor
*
* \param   - None
* 
* \return  - None
*
******************************************************************************/
void CommunicationInterfaceTCP::closeConnection()
{
	m_objImplementor->CloseServer();
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

bool CommunicationInterfaceTCP::linkConfig()
{
	bool bResult = false;
	char ip_arr[16];
	char ip_temp[16];
	char mask_addr[16] = {"255.255.255.0"};
	memcpy(ip_arr, Connectivity::EthernetConfig.TCP_ServerIP, IP_ADD_LEN);

	m_objImplementor->GetIPAddress((char*)"cpsw0",ip_temp);

	if(strcmp(ip_temp,ip_arr)!=0)
	{
		if(m_objImplementor->SetIpConfig((char*)"cpsw0",ip_arr, mask_addr)== OK)
			bResult = true;
	}
	else
		bResult = true;
	
	return bResult;
}

/**************************************************************************//**
*
* \brief   - This method is used to parse the received data
* 			 and check if the received data has the integrated package.
*
* \param   - Input variable: const char* buffer, const int bufLen,
* 			 Output variable: CLIENT_MESSAGE *ptrMsg
*
* \return  - return bResult
*
******************************************************************************/
bool CommunicationInterfaceTCP::parseProtocolPackage(const char* buffer, const int buflen, char* ptrMsg)
{
	return true;
	//TODO: TO be implemented
}

/**************************************************************************//**
*
* \brief   - package building for the incoming message
*
* \param   - Client_Message* ptrMsg. buffer, buflen
*
* \return  - If the building process is successful, it will return true; else false.
*
******************************************************************************/
bool CommunicationInterfaceTCP::buildProtocolPackage(const TX_MESSAGE* ptrMsg, char* buffer, int* buflen)
{
	bool bResult = false;
	char onePackage[MAX_SIZE_OF_MSG_LENGTH] = {0};
	UINT32 crcResult = 0;
	int offset = 0, msglen = 0;
	if(ptrMsg == nullptr)
		return false;
	if(buffer == nullptr)
		return false;
	if(buflen == nullptr)
		return false;

	memcpy(onePackage, &ptrMsg->DLEHEAD, sizeof(UINT8));
	offset += sizeof(UINT8);

	memcpy(onePackage + offset, &ptrMsg->STXSTHEAD, sizeof(UINT8));
	offset += sizeof(UINT32);
	
	memcpy(onePackage + offset, &ptrMsg->FUNID, sizeof(UINT16));
	offset += sizeof(UINT16);
	
	memcpy(onePackage + offset, &ptrMsg->Length, sizeof(UINT16));
	offset += sizeof(UINT32);
	
	//TODO Put Real Data into Buffer
	
	memcpy(onePackage + offset, ptrMsg->Buffer, ptrMsg->Length);
	offset += ptrMsg->Length;

	//TODO Add Calculation of Checksum

	memcpy(onePackage + offset, &ptrMsg->Checksum, sizeof(UINT16));
	offset += sizeof(UINT16);

	memcpy(onePackage + offset, &ptrMsg->DLETAIL, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &ptrMsg->STXTAIL, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(buflen, &offset, sizeof(UINT32));
	memcpy(buffer, onePackage, *buflen);

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
void CommunicationInterfaceTCP::Close()
{
	closeConnection();
	printf("Socket Close successfully\n");
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
int CommunicationInterfaceTCP::GetLinkStepIndex()
{
	return m_nStep;
}


