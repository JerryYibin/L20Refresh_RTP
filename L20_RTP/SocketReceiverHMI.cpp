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
		break;
	case REQ_SYSTEM_INFO_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SYSINFO_READ;
		break;
	case REQ_ACTIVE_RECIPE_INFO_IDX:
		message.msgID = UserInterface::TO_UI_TASK_ACTIVE_RECIPE_INFO;
		break;
	case REQ_SET_ACTIVE_RECIPE_IDX:
		message.msgID = UserInterface::TO_UI_SET_ACTIVE_RECIPE;
		break;
	case REQ_SETUP_WELD_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_WELD_PARAM;
		break;
	case REQ_SETUP_QUALITY_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_QUALITY_PARAM;
		break;
	case REQ_SETUP_ADVANCED_PARAM_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SETUP_ADVANCED_PARAM;
		break;
	case REQ_SYSTEM_CONGIF_READ_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SYSCONFIG_READ;
		break;
	case REQ_SYSTEM_CONGIF_WRITE_IDX:
		message.msgID = UserInterface::TO_UI_TASK_SYSCONFIG_WRITE;
		break;
	case REQ_INITIALIZATION_IDX:
		message.msgID = UserInterface::TO_UI_TASK_INITIALIZATION;
		break;
	case REQ_HEIGHT_CALIBRATE_START_IDX:
		message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CALIBRATE_START;
		break;
	case REQ_HEIGHT_CALIBRATE_CHECK_IDX:
		message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CHECK;
		break;
	case REQ_HEIGHT_CALIBRATE_CORRECT_IDX:
		message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CORRECT;
		break;
	case REQ_HEIGHT_CALIBRATE_ACCEPT_IDX:
		message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CALIBRATE_ACCEPT;
		break;
	case REQ_MAINTAIN_ACTUATOR_MOTION_IDX:
		message.msgID = UserInterface::TO_UI_TASK_AUX_MOTION_REQ;
		break;
	case REQ_USER_IO_INPUT_IDX:
		message.msgID = UserInterface::TO_UI_TASK_USERIO_INPUT_READ;
		break;
	case REQ_DEFAULT_RECIPE_IDX:
		message.msgID = UserInterface::TO_UI_DEFAULT_RECIPE_GET;
		break;
	case REQ_INSERT_RECIPE_WRITE_IDX:
		message.msgID = UserInterface::TO_UI_INSERT_RECIPE_WRITE;
		break;
	case REQ_RECIPE_LIBRARY_READ_IDX:
		message.msgID = UserInterface::TO_UI_RECIPE_LIBRARY_LATEST_PAGE_READ;
		break;
	case REQ_RECIPE_LIBRARY_NEXT_PAGE_IDX:
		message.msgID = UserInterface::TO_UI_RECIPE_LIBRARY_NEXT_PAGE_READ;
		break;
	case REQ_UPDATE_RECIPE_IDX:
		message.msgID = UserInterface::TO_UI_UPDATE_RECIPE;
		break;
	case REQ_DELETE_RECIPE_IDX:
		message.msgID = UserInterface::TO_UI_DELETE_RECIPE;
		break;
	case REQ_GET_SELECTED_RECIPE_INFO_REQ:
		message.msgID = UserInterface::TO_UI_RECIPE_DETAILS_GET;
		break;
	default:
		message.msgID = UserInterface::TO_UI_TASK_HEART_BEAT;
		LOGERR((char *)"SocketReceiver_HMI_T : --------Unknown Message ID----------- : %d\n", message.msgID, 0, 0);
		break;
	}
	SendToMsgQ(message, UI_MSG_Q_ID);
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
