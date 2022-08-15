/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Socket Receiver task for the communication with UI controller
Changed the C code "Socket_HMI_Task" to c++ static function. 
It's a good change for Socket_HMI_Task so Socket_HMI_Task can access all the source of
SocketReceiver_HMI owned using the class object pointer. 
 
**********************************************************************************************************/


#include "SocketReceiverHMI.h"
#include "CommunicationInterfaceHMI.h"
#include "UserInterface.h"
extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of SocketReceiver.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SocketReceiver_HMI::SocketReceiver_HMI()
{
	// Load the data message Q name
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	INTERFACE_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID = CP->getMsgQId(Data_Task + "/Control");
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
SocketReceiver_HMI::~SocketReceiver_HMI()
{
	// TODO Auto-generated destructor stub
}

/*************************************************************************//**
 * \brief   - Process the received message from client socket
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void SocketReceiver_HMI::ProcessTaskMessage(MESSAGE& message)
{
	switch(message.msgID)
	{
	case REQ_HEART_BEAT_IDX:
		message.msgID = UserInterface::TO_UI_TASK_HEART_BEAT;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case REQ_SYSTEM_INFO_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SYSINFO_READ;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case REQ_ACTIVE_RECIPE_INFO_IDX:
		message.msgID = UserInterface::TO_UI_TASK_ACTIVE_RECIPE_INFO;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case REQ_SETUP_WELD_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_WELD_PARAM;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case REQ_SETUP_QUALITY_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_QUALITY_PARAM;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case REQ_SETUP_ADVANCED_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_ADVANCED_PARAM;
		SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	default:
		LOGERR((char *)"SocketReceiver_HMI_T : --------Unknown Message ID----------- : %d\n", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Socket receiver task entry point
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver_HMI::Socket_HMI_Task(void)
{
	CommunicationInterface_HMI* Communication = CommunicationInterface_HMI::GetInstance();
	SocketReceiver_HMI* SocketReceiver = new(nothrow)SocketReceiver_HMI();
	CommunicationInterface_HMI::CLIENT_MESSAGE		ClientBuffer;
	MESSAGE				ProcessBuffer;
	if(nullptr != SocketReceiver)
	{
		while(SocketReceiver->bIsTaskRunStatus())
		{
			if(Communication->ErrorHandling() == OK)
			{
				if(Communication->Receiving(&ClientBuffer) == OK)
				{
					ProcessBuffer.msgID = ClientBuffer.msgID;
					memset(ProcessBuffer.Buffer, 0x00, sizeof(ProcessBuffer.Buffer));
					memcpy(ProcessBuffer.Buffer, ClientBuffer.Buffer, ClientBuffer.msgLen);
					SocketReceiver->ProcessTaskMessage(ProcessBuffer);
				}
			}
		}
		delete SocketReceiver;
	}
	else
	{
		LOGERR((char *)"HMISocket_T: -------Memory allocation failed-----------", 0, 0, 0);
	}
	
	SocketReceiver = nullptr;
	taskSuspend(taskIdSelf());
}
