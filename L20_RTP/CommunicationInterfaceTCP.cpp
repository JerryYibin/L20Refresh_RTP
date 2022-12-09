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
extern "C"
{
	#include "customSystemCall.h"	
	#include "hwif/drv/resource/vxbRtcLib.h"
}
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
	char 	tmpBuffer[MAX_LEN] 	= {0};
	int 	tmpBufLen							= 0;
	int		iResult								= ERROR;
	if(data == nullptr)
		return iResult;
	semTake(TCPSendMutex, WAIT_FOREVER); //lock sending
	if(buildProtocolPackage((ExternalData::ETHMESSAGE*)data, tmpBuffer, &tmpBufLen) == true)
		{
			if(m_objImplementor->Send(tmpBuffer, tmpBufLen, true) > 0)
				iResult = OK;
	}
	semGive(TCPSendMutex); //release sending
	return iResult;
}

/**************************************************************************//**
*
* \brief   - Data Receiving
* In this function we use dataVector to hold incoming message buffers to handle possible common TCP/IP problems
* like lose package, sticky package, wrong package 
* \param   - data, it should be the specific data structure pointer passed by the recall function.
* \return  - If a integrated data is received, it will return 0; else it will return -1;.
******************************************************************************/
int CommunicationInterfaceTCP::Receiving(void* data)
{
	char tmpBuffer[MAX_LEN] 	= {0};
	char SpareBuffer[MAX_LEN] = {0};
	int tmpRecvlen			= 0;
	int	iResult				= ERROR;
	if(data == nullptr)
		return iResult;
	if(m_nStep == CommunicationInterfaceTCP::LINK_DONE)
	{
		memset(tmpBuffer, 0x00, sizeof(tmpBuffer));
	/*Won't read more data from the TCP/IP buffer if the vector size exceed the MAX length to prevent the Client send 
	 message crazy and cause SC side cannot handle*/	
		if(dataVector.size() < MAX_VECTOR_LENGTH) 
		{
			tmpRecvlen = m_objImplementor->NBRead(tmpBuffer, MAX_LEN,true); //Use Non-blocking reading
			if(tmpRecvlen > 0)
			{	
	#if TCP_Test
				for(int i=0;i<MSG_LEN;i++)
					LOG("data = %d\n",tmpBuffer[i]);
				LOG("\nReceive len  = %d\n",tmpRecvlen);
	#endif
				for (int i=0; i<tmpRecvlen; i++)
				{
					dataVector.push_back(tmpBuffer[i]); 
				}
			}
			else if(tmpRecvlen < 0)
			{
				m_nStep = CommunicationInterface::LINK_ERROR;
			}
		}
	/*Won't decode and process the TCP/IP buffer if the vector size is less than the minimal length */	
		if(dataVector.size()>= MIN_RX_MSG_LEN)
		{
			if(parseProtocolPackage((MESSAGE*)data) == true)
				iResult = OK;
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
bool CommunicationInterfaceTCP::parseProtocolPackage(MESSAGE* ptrMsg)
{
	RX_DATA_STRUCT tmpBuffer;
	bool bresult = false;
	int offset = 0, len = 0;
	if(ptrMsg == nullptr)
		return false;
	memset(tmpBuffer.Buffer,0x00,sizeof(tmpBuffer.Buffer));
	if(findValidMessage(dataVector, &tmpBuffer) == true)
	{
		int calchecksum = checksumCalculate(&tmpBuffer.FUNID, tmpBuffer.Length/4+2);
		if(calchecksum == tmpBuffer.Checksum)
		{
			ptrMsg->msgID = tmpBuffer.FUNID;
			memcpy(&ptrMsg->Buffer, tmpBuffer.Buffer, tmpBuffer.Length);
			bresult = true;
		}				
	}
	return bresult;
	//TODO: TO be implemented
}

/**************************************************************************//**
*
* \brief   - package building for the incoming ExternalData::ETHMESSAGE data
*
* \param   - ExternalData::ETHMESSAGE* ptrMsg. buffer, buflen
*
* \return  - If the building process is successful, it will return true; else false.
*
******************************************************************************/
bool CommunicationInterfaceTCP::buildProtocolPackage(const ExternalData::ETHMESSAGE* ptrMsg, char* buffer, int* buflen)
{
	bool bResult = false;
	TX_MESSAGE msgTx;
	char onePackage[MAX_LEN] = {0};
	int offset = 0, csstart = 0;
	if(ptrMsg == nullptr)
		return false;
	if(buffer == nullptr)
		return false;
	if(buflen == nullptr)
		return false;
		
	switch(ptrMsg->msgID)
	{
	case ExternalData::WELDDATA:
		msgTx.FUNID = WELDDATAOUTPUT;
		break;
	case ExternalData::POWERCURVE:
		msgTx.FUNID = POWERCURVEOUTPUT;
		break;
	case ExternalData::HEIGHTCURVE:
		msgTx.FUNID = HEIGHTCURVEOUTPUT;
		break;
	case ExternalData::FREQUENCYCURVE:
		msgTx.FUNID = FREQUENCYCURVEOUTPUT;
		break;
	default:
		return false;
		break;		
	}
	msgTx.Length = ptrMsg->Length;
	msgTx._Buffer = ptrMsg->Buffer;
	memcpy(onePackage, &msgTx.DLEHEAD1, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &msgTx.DLEHEAD2, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &msgTx.STXSTHEAD1, sizeof(UINT8));
	offset += sizeof(UINT8);
	memcpy(onePackage + offset, &msgTx.STXSTHEAD2, sizeof(UINT8));
	offset += sizeof(UINT8);
	csstart = offset;
	memcpy(onePackage + offset, &msgTx.FUNID, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	memcpy(onePackage + offset, &msgTx.Length, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	memcpy(onePackage + offset, msgTx._Buffer, msgTx.Length);
	offset += msgTx.Length;

	msgTx.Checksum = checksumCalculate(onePackage+csstart, msgTx.Length/4+2);
	memcpy(onePackage + offset, &msgTx.Checksum, sizeof(UINT32));
	offset += sizeof(UINT32);
	
	GetTimeInfo(&msgTx.Time);	
	memcpy(onePackage + offset, &msgTx.Time, sizeof(TIME));
	offset += sizeof(TIME);
	
	memcpy(onePackage + offset, &msgTx.DLETAIL1, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &msgTx.DLETAIL2, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &msgTx.STXTAIL1, sizeof(UINT8));
	offset += sizeof(UINT8);
	
	memcpy(onePackage + offset, &msgTx.STXTAIL2, sizeof(UINT8));
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

/**************************************************************************//**
*
* \brief   - Calculate the checksum value of the TCP/IP Sending Message buffer
*
* \param   - const void *buf, const unsigned int len
*
* \return  - Checksum Value.
*
******************************************************************************/
unsigned int CommunicationInterfaceTCP::checksumCalculate(const void *buf,const unsigned int len)
{
	int counter = 0;
	UINT64 checksum = 0;
	for (counter = 0; counter < len; counter++)
	{
		checksum += *(unsigned int *)buf;
	    buf = ((int *) buf) + 1;
	}
	checksum &= 0xFFFFFFFF;
	return (unsigned int)checksum;
}
/**************************************************************************//**
*
* \brief   - Find an intact Message whose data format matched with the format defined at Client side
*  from the whole vtData list. If can find, then copy it to a separate array buffer and discard all bytes
* from the start of the vtData list to the end of the last founded byte. 
*
* \param  vector<char>&vtData, RX_DATA_STRUCT* ptrMsg
*
* \return  - whether found or not.
*
******************************************************************************/

bool CommunicationInterfaceTCP::findValidMessage(vector<char>&vtData, RX_DATA_STRUCT* ptrMsg )
{
	int i ;
	int len;
	for(i = 0; i <= vtData.size()-MIN_RX_MSG_LEN; ++i)
	{
		if (vtData[i+HEAD1] == ptrMsg->DLEHEAD1 && vtData[i+HEAD2] == ptrMsg->DLEHEAD2 && vtData[i+HEAD3] == ptrMsg->STXSTHEAD1 && vtData[i+HEAD4] == ptrMsg->STXSTHEAD2)
		{	
			len = (int)(vtData[i+LEN1] << 24 | vtData[i+LEN2] << 16 | vtData[i+LEN3] << 8 | vtData[i+LEN4]);
			if(i+len+MIN_RX_MSG_LEN-1 < vtData.size())
			{
				if(vtData[i+len+TAIL1] == ptrMsg->DLETAIL1 && vtData[i+len+TAIL2] == ptrMsg->DLETAIL2 && 
					vtData[i+len+TAIL3] == ptrMsg->STXTAIL1 && vtData[i+len+TAIL4] == ptrMsg->STXTAIL2)
				{
					break;
				}
			}
		}
	}
	if(i <= vtData.size()-MIN_RX_MSG_LEN)   // Once Found will delete the vector from begin to the last value of the found message.
	{
		memcpy(ptrMsg, &vtData[i], LEN4-HEAD1+1);
		memcpy(ptrMsg->Buffer,&vtData[i+LEN4-HEAD1+1],len);
		ptrMsg->Checksum = (int)(vtData[i+len+CS4] << 24 | vtData[i+len+CS3] << 16 | vtData[i+len+CS2] << 8 | vtData[i+len+CS1]);
		vtData.erase(vtData.begin(), vtData.begin()+i+len+MIN_RX_MSG_LEN);
		return true;
	}
	if(vtData.size() >= MAX_VECTOR_LENGTH-MIN_RX_MSG_LEN)
	{
		vtData.erase(vtData.begin(), vtData.end()-MIN_RX_MSG_LEN);
	}
	return false;
}
/**************************************************************************//**
*
* \brief   - Get the current system Time Information
*
* \param   - TIME* _timedata
*
* \return  - None.
*
******************************************************************************/
void CommunicationInterfaceTCP::GetTimeInfo(TIME* _timedata)
{
	struct tm time;
	vxbRtcGet(&time);
	_timedata->Year = time.tm_year+1900;
	_timedata->Month = time.tm_mon+1;
	_timedata->Day = time.tm_mday;
	_timedata->Hour = time.tm_hour;
	_timedata->Minute = time.tm_min;
	_timedata->Second = time.tm_sec;
}
