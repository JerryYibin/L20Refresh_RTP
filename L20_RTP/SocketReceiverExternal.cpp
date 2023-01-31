/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------

Socket Receiver task for the communication with Customer's Data Receiver(via eth port cpsw0).
Customer's Data Receiver may be TCP/IP Client or Gateway Server, hence Socket Receiver task 
need to handle the real role change according to the events receiving from other Tasks.
**********************************************************************************************************/


#include "SocketReceiverExternal.h"
#include "CommunicationInterfaceTCP.h"
#include "CommunicationInterfaceDIG.h"
#include "UserInterface.h"
#include "ControlTask.h"
#include "Connectivity.h"
#include "ExternalManager.h"
#include "ExternalData.h"
extern "C"
{
	#include "customSystemCall.h"	
}


/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of SocketReceiver_External.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SocketReceiver_External::SocketReceiver_External()
{
	EXT_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::EXT_MANAGER_T]);
}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SocketReceiver_External::~SocketReceiver_External()
{
	// TODO Auto-generated destructor stub
}

/*************************************************************************//**
 * \brief   - Process the received message from customer's data device
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void SocketReceiver_External::ProcessTaskMessage(MESSAGE& message)
{
	int iResult = OK;
	switch(message.msgID)
	{
	case REQ_WELD_DATA_IDX:
		message.msgID = ExternalManager::TO_EXT_TASK_WELD_DATA_REQ;
		break;
	case REQ_POWER_CURVE_IDX:
		message.msgID = ExternalManager::TO_EXT_TASK_POWER_CURVE_REQ;
		break;
	case REQ_HEIGHT_CURVE_IDX:
		message.msgID = ExternalManager::TO_EXT_TASK_HEIGHT_CURVE_REQ;
		break;
	case REQ_FREQUENCY_CURVE_IDX:
		message.msgID = ExternalManager::TO_EXT_TASK_FREQUENCY_CURVE_REQ;
		break;
	case REQ_RECALL_RECIPE_IDX:
		message.msgID = ExternalManager::TO_EXT_TASK_RECALL_RECIPE_REQ;
		break;
	
	default:
		iResult = ERROR;
		LOGERR((char *)"SocketReceiver_External_T : --------Unknown Message ID----------- : %d\n", message.msgID, 0, 0);
		break;
	}
	if(iResult == OK)
		SendToMsgQ(message, EXT_MSG_Q_ID);
}

/**************************************************************************//**
* 
* \brief   - External Socket receiver task entry point
* The Task have functions like: support receiving binary data from customer's device,
* react to Socket role change according to event flag, sending heart beat message 
* for DIG type, hence the Receiving function should be non-blocking
*  
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver_External::Socket_External_Task(void)
{
	UINT32		events;
	CommunicationInterface* _objCommunication = nullptr;
	SocketReceiver_External* SocketReceiver = new(nothrow)SocketReceiver_External();
	ExternalManager* EthernetObj = ExternalManager::GetInstance();
	MESSAGE				ProcessBuffer;
	if(nullptr != SocketReceiver)
	{
		while(SocketReceiver->bIsTaskRunStatus())
		{
			if(eventReceive(EXTERNAL_CLOSE_EVENT | EXTERNAL_LINK_EVENT | EXTERNAL_READ_EVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				_objCommunication = EthernetObj->GetCommunicateObj();
				if(events & EXTERNAL_CLOSE_EVENT)
				{
					_objCommunication->Close();
				}
				else if(events & EXTERNAL_LINK_EVENT)
				{
					_objCommunication->ErrorHandling();
				}
				else if(events & EXTERNAL_READ_EVENT)
				{
					if(_objCommunication->Receiving(&ProcessBuffer) == OK) // Using Non-Blocking receiving 
					{
						SocketReceiver->ProcessTaskMessage(ProcessBuffer);
					}
				}
			}
		}
		delete SocketReceiver;
	}
	else
	{
		LOGERR((char *)"SocketReceiver_External: -------Memory allocation failed-----------", 0, 0, 0);
	}
	
	SocketReceiver = nullptr;
	taskSuspend(taskIdSelf());
}

