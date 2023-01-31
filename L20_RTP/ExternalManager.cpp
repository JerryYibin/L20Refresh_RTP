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
#include "DataTask.h"
#include "Recipe.h"
using namespace std;

ExternalManager* ExternalManager::ExternalEthobj = nullptr;
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
	m_ptrData = nullptr;
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_REQ  = CP->getMsgQId (Data_Task + "/Request");
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::EXT_MANAGER_T]);
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
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXT_SOCKET_T]), EXTERNAL_CLOSE_EVENT) != OK)
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
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXT_SOCKET_T]), EXTERNAL_LINK_EVENT) != OK)
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
	
	if(eventSend(CP->getTaskId(CommonProperty::cTaskName[CommonProperty::EXT_SOCKET_T]), EXTERNAL_READ_EVENT) != OK)
	{
		LOGERR((char*) "ExternalManager::readSocket eventSend: Error\n", 0, 0, 0);
		return ERROR;
	}
	
	return OK;
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


/**************************************************************************//**
* \brief   - Process commands sending to ExternalTask 
*
* \param   - struct Message&.
* 
* \return  - None.
*
******************************************************************************/
void ExternalManager::ProcessTaskMessage(MESSAGE& message)
{
	int ErrCode;
	int tmp;
	ExternalData::ETHMESSAGE sendMsg;
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));
	while(msgQReceive(SELF_MSG_Q_ID, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(tmpMsg.msgID)
		{
		case TO_EXT_TASK_AFTER_WELD_REQ:
			if(Connectivity::EthernetConfig.EthernetType == TCP_IP)
			{
				sendMsg.msgID = ExternalData::WELDDATA;
				senddata(&sendMsg);
				if(Connectivity::EthernetConfig.TCP_RemoteSignature == true)
				{
					sendMsg.msgID = ExternalData::POWERCURVE;
					senddata(&sendMsg);
					
					sendMsg.msgID = ExternalData::HEIGHTCURVE;
					senddata(&sendMsg);
					
					sendMsg.msgID = ExternalData::FREQUENCYCURVE;
					senddata(&sendMsg);
				}
			}
			break;
		case TO_EXT_TASK_WELD_DATA_REQ:
			sendMsg.msgID = ExternalData::WELDDATA;
			senddata(&sendMsg);
			break;
		case TO_EXT_TASK_POWER_CURVE_REQ:
			sendMsg.msgID = ExternalData::POWERCURVE;
			senddata(&sendMsg);
			break;
		case TO_EXT_TASK_HEIGHT_CURVE_REQ:
			sendMsg.msgID = ExternalData::HEIGHTCURVE;
			senddata(&sendMsg);
			break;
		case TO_EXT_TASK_FREQUENCY_CURVE_REQ:
			sendMsg.msgID = ExternalData::FREQUENCYCURVE;
			senddata(&sendMsg);
			break;
		case TO_EXT_TASK_RECALL_RECIPE_REQ:
			tmpMsg.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_QUERY_BY_NAME;
			SendToMsgQ(tmpMsg, DATA_MSG_Q_ID_REQ);
			break;
		case TO_EXT_TASK_RECALL_RECIPE_RESPONSE:
			memcpy(&ErrCode, tmpMsg.Buffer,sizeof(int));
			if(ErrCode!= ERROR)
			{
				if(Recipe::ActiveRecipeSC->m_RecipeID != ErrCode)
				{
					Recipe::ActiveRecipeSC->m_RecipeID = ErrCode;
					tmp = BATCH_SIZE_NUM;
					WeldResults::_WeldResults->CycleCounter = 0;
					Recipe::ActiveRecipeSC->Set(&tmp, WeldRecipeSC::BATCH_SIZE);
					tmpMsg.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
					SendToMsgQ(tmpMsg, DATA_MSG_Q_ID_REQ);
				}
			}
			sendMsg.msgID = ExternalData::ACTIVERECIPE;
			sendMsg.Length = sizeof(int);
			memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
			memcpy(sendMsg.Buffer,&ErrCode, sizeof(int));	
			m_ptrCom->Sending(&sendMsg);
			break;
		default:
			//TODO: To Handle some functions here like DIG heartbeat
			break;
		}
	}
}


/**************************************************************************//**
* 
* \brief   - External Manager task entry point
* The ExternalTask has the responsibility of managering the Ethernet Link of the 
  External Ethernet Port according to the current Ethernet type selected by user. 
  It also need handle the incoming event& MsgQ from other task.
*  
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ExternalManager::External_Manager_Task(void)
{
	MESSAGE			ProcessBuffer;
	UINT32			events;	
	ExternalManager *ExternalTask = ExternalManager::GetInstance();
	if(NULL != ExternalTask)
	{
		/* Control Task loop and the bIsTaskRun flag enabled when task created */
		while(ExternalTask->bIsTaskRunStatus())
		{
			if(eventReceive(EXT_500MS | EXTERNAL_TASK_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{

				if(events & EXT_500MS)
				{
					ExternalTask->Update();
				}

				if(events & EXTERNAL_TASK_QEVENT)
				{
					ExternalTask->ProcessTaskMessage(ProcessBuffer);
				}
			}	
		}
		delete ExternalTask;
	}
	else
	{
		LOGERR((char *)"External_T : -------------Memory allocation failed-----------",0,0,0);
	}
		
	ExternalTask = NULL;
	taskSuspend(taskIdSelf());

}
