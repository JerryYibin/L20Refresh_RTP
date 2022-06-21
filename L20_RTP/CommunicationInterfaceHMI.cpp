/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "CommunicationInterfaceHMI.h"
#include "commons.h"
#include "Logger.h"

CommunicationInterface_HMI* CommunicationInterface_HMI::m_obj = nullptr;
SEM_ID CommunicationInterface_HMI::m_semaphoreMutex = SEM_ID_NULL;
CommunicationInterface_HMI* CommunicationInterface_HMI::GetInstance()
{
	return m_obj ? m_obj : (m_obj = new CommunicationInterface_HMI());
}

/**************************************************************************//**
* 
* \brief   - Constructor Initialize current communication as the HMI socket connection.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterface_HMI::CommunicationInterface_HMI()
	:CommunicationInterface(CommunicationInterface::HMI_SOCKET_CONNECTION)
	,m_nDescriptor(ERROR)
	,m_objSSL(nullptr)
	,m_nStep(CommunicationInterface_HMI::LINK_TCPIP)
{
	m_objImplementor = (EmrsnSocket*)GetCommunicationImp();
	if(m_objImplementor == nullptr)
	{
		LOGERR((char *)"HMI Socket:-------TCP/IP initialization allocation failed-----------", 0, 0, 0);
	}
	m_objSSL = new SSLWrapper();
	if(m_objSSL->InitSSL() == false)
	{
		LOGERR((char *)"HMI Socket:-------SSL initialization allocation failed-----------", 0, 0, 0);
	}
	/*create semaphore*/
	m_semaphoreMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterface_HMI::~CommunicationInterface_HMI() 
{
	closeConnection();
	if(m_objImplementor != nullptr)
	{
		delete m_objImplementor;
		m_objImplementor = nullptr;
	}
	if(m_objSSL != nullptr)
	{
		delete m_objSSL;
		m_objSSL = nullptr;
	}
	
	if(m_semaphoreMutex != SEM_ID_NULL)
	{
		semDelete(m_semaphoreMutex);
		m_semaphoreMutex = SEM_ID_NULL;
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
int CommunicationInterface_HMI::ErrorHandling()
{
	int iResult = ERROR;
	switch(m_nStep)
	{
	case CommunicationInterface_HMI::LINK_TCPIP:
		if(establishTCPIPSocket() == true)
			m_nStep = CommunicationInterface_HMI::LINK_SSL;
		else
			m_nStep = CommunicationInterface_HMI::LINK_ERROR;
		break;
	case CommunicationInterface_HMI::LINK_SSL:
		if(establishSSLServer() == true)
			m_nStep = CommunicationInterface_HMI::LINK_DONE;
		else
			m_nStep = CommunicationInterface_HMI::LINK_ERROR;
		break;
	case CommunicationInterface_HMI::LINK_DONE:
		iResult = OK;
		break;
	default:
		closeConnection();
		m_nStep = CommunicationInterface_HMI::LINK_TCPIP;
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
int CommunicationInterface_HMI::Sending(const void* data)
{
	char 	tmpBuffer[MAX_SIZE_OF_MSG_LENGTH] 	= {0};
	int 	tmpBufLen							= 0;
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	semTake(m_semaphoreMutex, WAIT_FOREVER); //lock sending
	if(((CLIENT_MESSAGE*)data)->msgLen >= MAX_SIZE_OF_MSG_BUFF)
	{
		LOGERR( (char *)"HMI Socket SendMessageToClient, Socket message length exceeded : %d", ((CLIENT_MESSAGE*)data)->msgLen, 0, 0);
	}
	else
	{
		if(buildProtocolPackage((CLIENT_MESSAGE*)data, tmpBuffer, &tmpBufLen) == true)
		{
			if(m_objSSL->SendSSL(tmpBuffer, tmpBufLen) > 0)
				iResult = OK;
		}
	}
	semGive(m_semaphoreMutex); //release sending
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
int CommunicationInterface_HMI::Receiving(void* data)
{
	char 	tmpBuffer[MAX_SIZE_OF_MSG_LENGTH] 	= {0};
	int 	tmpBufLen							= 0;
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	tmpBufLen = m_objSSL->RecvSSL(tmpBuffer, MAX_SIZE_OF_MSG_LENGTH);
	if(tmpBufLen > 0)
	{
		if(parseProtocolPackage(tmpBuffer, tmpBufLen, (CLIENT_MESSAGE*)data) == false)
			LOGERR((char *)"HMI Socket Invalid message received (CRC Error): MsgID: %d", ((CLIENT_MESSAGE*)data)->msgID, 0, 0);
		else
			iResult = OK;
	}
	else
	{
		m_nStep = CommunicationInterface_HMI::LINK_ERROR;
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
bool CommunicationInterface_HMI::establishTCPIPSocket()
{
	INT32 iPort = HMI_SOCKET_PORT;
	bool bResult = false;
	
	if(m_objImplementor->Create() == false)
	{
		LOGERR((char*) "HMI Socket creation failed", 0, 0, 0);
		return false;
	}
	
	if(m_objImplementor->Bind(iPort) == false)
	{
		LOGERR((char*) "HMI Socket binding failed", 0, 0, 0);
		return false;
	}
	
	if(m_objImplementor->Listen() == false)
	{
		LOGERR((char*) "HMI Socket listening failed", 0, 0, 0);
		return false;
	}
	
	LOG("HMI Socket listening on Port: %d\n", iPort);
	
	// stay in accept mode until proper connection or error 
	m_nDescriptor = m_objImplementor->Accept(EmrsnSocket::HMI_SOCKET_CONNECTION);
	
	if(m_nDescriptor == ERROR)
	{
		LOGERR((char *) "HMI Socket Error on socket accept", 0, 0, 0);
		bResult = false;
	}
	else
	{
		LOG("HMI Socket connection established, Client_SockId: %d\n", m_nDescriptor);
		bResult = true;
	}
	
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - SSL link establishing that should include Creating, Binding and Accept three processes.
*            
* \param   - none
*
* \return  - If there is anyone process is false, it will return false.
*
******************************************************************************/
bool CommunicationInterface_HMI::establishSSLServer()
{
	if(m_objSSL->CreateSSL() == false)
		return false;
	if(m_objSSL->BindSSL(m_nDescriptor) == false)
		return false;
	if(m_objSSL->AcceptSSL() == false)
		return false;
	return true;
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
void CommunicationInterface_HMI::closeConnection()
{
	m_objSSL->ResetSSL();
	close(m_nDescriptor);
	m_objImplementor->Close();
	LOGERR((char *)"HMI Socket: CloseClientConnection: Client socket connection closed and waiting for client request",0,0,0);
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
bool CommunicationInterface_HMI::parseProtocolPackage(const char* buffer, const int buflen, CLIENT_MESSAGE* ptrMsg)
{
	bool bResult = false;
	int offset = 0, len = 0;
	if(buffer == nullptr)
		return false;
	if(buflen == 0)
		return false;
	if(ptrMsg == nullptr)
		return false;
	
	memcpy(&ptrMsg->msgID, buffer, sizeof(UINT32));
	offset += sizeof(UINT32);
	len += sizeof(UINT32);
	if(offset > buflen)
		return false;
	
	memcpy(&ptrMsg->msgLen, buffer + offset, sizeof(UINT32));
	offset += sizeof(UINT32);
	len += sizeof(UINT32);
	if(offset > buflen)
		return false;
	
	memcpy(ptrMsg->Buffer, buffer + offset, ptrMsg->msgLen);
	offset += ptrMsg->msgLen;
	len += ptrMsg->msgLen;
	if(offset > buflen)
		return false;
	
	memcpy(&ptrMsg->crc32, buffer + offset, sizeof(UINT32));
	if(ptrMsg->crc32 == QuickCRC32Algorithm::crc32_ccitt(buffer, len))
		bResult = true;
	return bResult;
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
bool CommunicationInterface_HMI::buildProtocolPackage(const CLIENT_MESSAGE* ptrMsg, char* buffer, int* buflen)
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
	
	memcpy(onePackage, &ptrMsg->msgID, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	msglen = sizeof(ptrMsg->rspCode) + ptrMsg->msgLen;
	memcpy(onePackage + offset, &msglen, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	memcpy(onePackage + offset, &ptrMsg->rspCode, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	memcpy(onePackage + offset, ptrMsg->Buffer, ptrMsg->msgLen);
	offset += ptrMsg->msgLen;
	
	crcResult = QuickCRC32Algorithm::crc32_ccitt(onePackage, offset);
	memcpy(onePackage + offset, &crcResult, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	memcpy(buflen, &offset, sizeof(UINT32));
	memcpy(buffer, onePackage, *buflen);
	
	return true;
}


