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
#include "ExternalEthernet.h"
#include "SocketReceiverExternal.h"
#include "Connectivity.h"
#include "CommunicationInterfaceTCP.h"
#include "CommunicationInterfaceDIG.h"
using namespace std;

ExternalEthernet* ExternalEthernet::ExternalEthobj = nullptr;
unsigned int ExternalEthernet::Tick1MS;
/**************************************************************************//**
* \brief   - Constructor 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ExternalEthernet::ExternalEthernet()
{	
	memcpy(&m_PrevEthernetConfig, &Connectivity::EthernetConfig, sizeof(ETHERNET));
	refreshExternalEthobj();
	Tick1MS = 0;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ExternalEthernet::~ExternalEthernet()
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
ExternalEthernet* ExternalEthernet:: GetInstance()
{
	return (ExternalEthobj != nullptr) ? ExternalEthobj : (ExternalEthobj = new ExternalEthernet());
}


/**************************************************************************//**
* \brief   - Return the CommunicationInterface pointer.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
CommunicationInterface* ExternalEthernet:: GetCommunicateObj()
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
int ExternalEthernet::closeSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_CLOSE_EVENT) != OK)
	{
		LOGERR((char*) "ExternalEthernet::closeSocket eventSend: Error\n", 0, 0, 0);
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
int ExternalEthernet::linkSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_LINK_EVENT) != OK)
	{
		LOGERR((char*) "ExternalEthernet::linkSocket eventSend: Error\n", 0, 0, 0);
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
int ExternalEthernet::readSocketEvent()
{
	unsigned int errid;
	TASK_ID tID;
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXTERNAL_SOCKET_T]), EXTERNAL_READ_EVENT) != OK)
	{
		LOGERR((char*) "ExternalEthernet::readSocket eventSend: Error\n", 0, 0, 0);
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
void ExternalEthernet::ProcessTaskMessage(MESSAGE& message)
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
void ExternalEthernet::refreshExternalEthobj()
{		
	if(Connectivity::EthernetConfig.EthernetType == TCP_IP)
		m_ptrCom = CommunicationInterfaceTCP::GetInstance();
	else if(Connectivity::EthernetConfig.EthernetType == GATEWAY)
		m_ptrCom = CommunicationInterfaceDIG::GetInstance();
	else 
		m_ptrCom = nullptr;
}

/**************************************************************************//**
* \brief   - This function is directly called by system 1ms timer to control the external Ethernet
* task
* \param   - None
* 
* \return  - None.
*
******************************************************************************/
int ExternalEthernet::Update()
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
bool ExternalEthernet::requireUpdateSocket()
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
	}
	return do_update;
}


