/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------
This module handles the External Ethernet related configuration, and it provides function Interface 
for outside to call. It works like a Task while it won't need to be a real task

**********************************************************************************************************/
#include  <sys/socket.h>
#include  <sys/ioctl.h>
#include  <arpa/inet.h>
#include  <net/route.h>
#include  <iostream>
#include "Logger.h"
#include <net/ipioctl_var.h>
#include "ExternalManager.h"
#include "SocketReceiverExternal.h"
#include "Connectivity.h"
#include "CommunicationInterfaceTCP.h"
#include "CommunicationInterfaceDIG.h"
#include "ExternalDataTCP.h"

using namespace std;

ExternalManager* ExternalManager::ExternalEthobj = nullptr;
unsigned int ExternalManager::Tick1MS;
/**************************************************************************//**
* \brief   - Constructor 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ExternalManager::ExternalManager()
{	
	memcpy(&m_PrevEthernetConfig, &Connectivity::EthernetConfig, sizeof(ETHERNET));
	refreshExternalEthobj();
	Tick1MS = 0;
	m_ptrData = nullptr;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ExternalManager::~ExternalManager()
{	
	// TODO Auto-generated destructor stub
}


/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
ExternalManager* ExternalManager:: GetInstance()
{
	return (ExternalEthobj != nullptr) ? ExternalEthobj : (ExternalEthobj = new ExternalManager());
}


/**************************************************************************//**
* \brief   - Return the CommunicationInterface pointer.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
CommunicationInterface* ExternalManager:: GetCommunicateObj()
{
	return m_ptrCom;
}

/**************************************************************************//**
* \brief   - Send EXTERNAL_CLOSE_EVENT flag to Socket_External_Task to close corresponding socket
*
* \param   - None.
*
* \return  - OK or ERROR.
*
******************************************************************************/
int ExternalManager::closeSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_CLOSE_EVENT) != OK)
	{
		LOGERR((char*) "ExternalManager::closeSocket eventSend: Error\n", 0, 0, 0);
		return ERROR;
	}
	
	return OK;
}


/**************************************************************************//**
* \brief   - Send EXTERNAL_LINK_EVENT flag to Socket_External_Task to create socket link
*
* \param   - None.
*
* \return  - OK or ERROR.
*
******************************************************************************/
int ExternalManager::linkSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_LINK_EVENT) != OK)
	{
		LOGERR((char*) "ExternalManager::linkSocket eventSend: Error\n", 0, 0, 0);
		return ERROR;
	}
	
	return OK;
}


/**************************************************************************//**
* \brief   - Send EXTERNAL_READ_EVENT flag to Socket_External_Task to trigger socket Receving
*
* \param   - None.
*
* \return  - OK or ERROR.
*
******************************************************************************/
int ExternalManager::readSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_READ_EVENT) != OK)
	{
		LOGERR((char*) "ExternalManager::readSocket eventSend: Error\n", 0, 0, 0);
		return ERROR;
	}
	
	return OK;
}


/**************************************************************************//**
* \brief   - Process commands(Ethernet Close, Ethernet New, Ethernet Reset) 
*
* \param   - struct Message&.
* 
* \return  - None.
*
******************************************************************************/
void ExternalManager::ProcessTaskMessage(MESSAGE& message)
{
	unsigned int tmp;
	switch(message.msgID)
	{
	default:
		//TODO: To Handle some functions here like DIG heartbeat
		break;
	}
}

/**************************************************************************//**
* \brief   - Update the pointer to CommunicationInterface according to current ethernet type
*
* \param   - None
* 
* \return  - None.
*
******************************************************************************/
void ExternalManager::refreshExternalEthobj()
{		
	if(Connectivity::EthernetConfig.EthernetType == TCP_IP)
	{
		m_ptrCom = CommunicationInterfaceTCP::GetInstance();
		m_ptrData = ExternalDataTCP::GetInstance();
	}
	else if(Connectivity::EthernetConfig.EthernetType == GATEWAY)
	{
		m_ptrCom = CommunicationInterfaceDIG::GetInstance();
	}
	else 
	{
		m_ptrCom = nullptr;
		m_ptrData = nullptr;
	}

}

/**************************************************************************//**
* \brief   - This function is directly called by system 1ms timer to control the external Ethernet
* task
* \param   - None
* 
* \return  - None.
*
******************************************************************************/
int ExternalManager::Update()
{
	int result = FALSE;
	Tick1MS++;
	if(Tick1MS % SAMPLE500MSEC!= 0)
	{
		return OK;
	}
	else
	{	
		if(requireUpdateSocket() == true) 
		{
			closeSocketEvent();
			if(m_ptrCom->GetLinkStepIndex()== CommunicationInterface::LINK_CONFIG)
			{
				memcpy(&m_PrevEthernetConfig, &Connectivity::EthernetConfig, sizeof(ETHERNET));
			}
			return OK;
		}
		refreshExternalEthobj();//update the m_ptrCom value according to the latest Ethernet type
		if(m_ptrCom == nullptr)
			return OK;
		
		switch(m_ptrCom->GetLinkStepIndex())
		{
		case CommunicationInterface::LINK_CONFIG:
		case CommunicationInterface::LINK_TCPIP:
		case CommunicationInterface::LINK_ERROR:
			if(linkSocketEvent() == OK)
				result = OK;
			break;
		case CommunicationInterface::LINK_DONE:
			if(readSocketEvent() == OK)
				result = OK;
			break;
		default:
			break;
		}	
	}
	return result;
}

/**************************************************************************//**
* \brief   - Compare the current EthernetConfig struct and the backup EthernetConfig struct
to see whether the socket related setting changes 
*
* \param   - None
* 
* \return  - true or false.
*
******************************************************************************/
bool ExternalManager::requireUpdateSocket()
{
	bool do_update = false;	

	if(Connectivity::EthernetConfig.EthernetType == m_PrevEthernetConfig.EthernetType)
	{
		if(Connectivity::EthernetConfig.EthernetType == INVALID_TYPE)
		{
			return false;
		}
		else if(Connectivity::EthernetConfig.EthernetType == TCP_IP)
		{
			if((m_PrevEthernetConfig.TCP_ServerPort!= Connectivity::EthernetConfig.TCP_ServerPort)
						||(strcmp(m_PrevEthernetConfig.TCP_ServerIP,Connectivity::EthernetConfig.TCP_ServerIP)!=0))
			{   
				do_update = true;
				m_ptrCom = CommunicationInterfaceTCP::GetInstance();
			}
		}
		else if(Connectivity::EthernetConfig.EthernetType == GATEWAY)
		{
			if(m_PrevEthernetConfig.DIG_MachineID!= Connectivity::EthernetConfig.DIG_MachineID)
			{
				do_update = true;
				m_ptrCom = CommunicationInterfaceDIG::GetInstance();
			}
		}
	}
	else
	{
		if(m_PrevEthernetConfig.EthernetType  == TCP_IP)
		{
			do_update = true;
			m_ptrCom = CommunicationInterfaceTCP::GetInstance();
		}
		else if(m_PrevEthernetConfig.EthernetType  == GATEWAY)
		{
			do_update = true;
			m_ptrCom = CommunicationInterfaceDIG::GetInstance();
		}	
		else
		{
			memcpy(&m_PrevEthernetConfig, &Connectivity::EthernetConfig, sizeof(ETHERNET));		
		}
	}
	return do_update;
}

/**************************************************************************//**
* \brief   - Send Weld Data via External Ethernet
* 			 Interface to be called every time after welding is completed.
* \param   - None
* 
* \return  - OK or not.
*
******************************************************************************/
int ExternalManager::Send(int cmd)
{
	int result = ERROR;
	if(m_ptrCom == nullptr)
		return ERROR;
	if(m_ptrCom->GetLinkStepIndex()!= CommunicationInterface::LINK_DONE)
		return ERROR;
	ExternalData::ETHMESSAGE sendMsg;
	switch(cmd)
	{
	case AFTERWELD:
		if(Connectivity::EthernetConfig.EthernetType == TCP_IP)
		{
			sendMsg.msgID = ExternalData::WELDDATA;
			result = senddata(&sendMsg);
			if(Connectivity::EthernetConfig.TCP_RemoteSignature == true)
			{
				sendMsg.msgID = ExternalData::POWERCURVE;
				result = senddata(&sendMsg);
					
				sendMsg.msgID = ExternalData::HEIGHTCURVE;
				result = senddata(&sendMsg);
					
				sendMsg.msgID = ExternalData::FREQUENCYCURVE;
				result = senddata(&sendMsg);
			}
		}
		break;
	case REPLYWELDDATA:
		sendMsg.msgID = ExternalData::WELDDATA;
		result = senddata(&sendMsg);
		break;	
	case REPLYPOWERCURVE:
		sendMsg.msgID = ExternalData::POWERCURVE;
		result = senddata(&sendMsg);
		break;
	case REPLYHEIGHTCURVE:
		sendMsg.msgID = ExternalData::HEIGHTCURVE;
		result = senddata(&sendMsg);
		break;
	case REPLYFREQUENCYCURVE:
		sendMsg.msgID = ExternalData::FREQUENCYCURVE;
		result = senddata(&sendMsg);
		break;
	default:
		break;
	}
	return result;
}
/**************************************************************************//**
* \brief   - Send specific Weld Data via External Ethernet
* 			 Interface to be called after receiving inquiry message from Client 
* \param   - ExternalData::ETHMESSAGE* message
* 
* \return  - OK or not.
*
******************************************************************************/

int ExternalManager::senddata(ExternalData::ETHMESSAGE* message)
{
	int result = ERROR; 
	if(m_ptrCom == nullptr)
		return ERROR;
	if(m_ptrCom->GetLinkStepIndex()!= CommunicationInterface::LINK_DONE)
		return ERROR;
	memset(message->Buffer, 0x00, sizeof(message->Buffer));
	result = m_ptrData->BuildProtocolData(message->msgID,message->Buffer);
	if(result >= 0)
	{
		message->Length = result;
		m_ptrCom->Sending(message);
	}
	else
	{
		LOGERR((char*) "ExternalManager -- BuildProtocolData failed\n", 0, 0, 0);
	}	
	return result;
}


