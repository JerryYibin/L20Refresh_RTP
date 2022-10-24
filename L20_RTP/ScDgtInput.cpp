/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Database Interface task for DB related read or write operations  
 
**********************************************************************************************************/

#include "ScDgtInput.h"
#include "L20ScDgtInputTask.h"
#include "P1ScDgtInputTask.h"
#include "UserInterface.h"
extern "C"
{
	#include "customSystemCall.h"	
}
ScDgtInputTask* ScDgtInputTask::_DgtInputObj = nullptr;
/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of ScDgtInputTask.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
ScDgtInputTask::ScDgtInputTask()
	:SELF_MSG_Q_ID(0)
	,UI_MSG_Q_ID(0)
{
	// Load the data message Q name
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTIN_T]);
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	m_UserIO = 0;

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
ScDgtInputTask::~ScDgtInputTask()
{

}

/*************************************************************************//**
 * \brief   - Process the received message from INPUT_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void ScDgtInputTask::ProcessTaskMessage(MESSAGE& message)
{
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));
	while(msgQReceive(SELF_MSG_Q_ID, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(message.msgID)
		{
		case TO_DGT_INPUT_TASK_IO_GET:
			memcpy(message.Buffer, &m_UserIO, sizeof(unsigned int));
			message.msgID = UserInterface::TO_UI_TASK_USER_IO_RESPONSE;
			SendToMsgQ(message, UI_MSG_Q_ID);
			break;
		default:
			LOGERR((char *)"ScDgtInput_T : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
			break;
		}
	}
}

/**************************************************************************//**
* \brief  	- Get specific actuator object following system type
*
* \param	- None
*
* \return 	- ScDgtInputTask object
*
******************************************************************************/
ScDgtInputTask* ScDgtInputTask::GetInstance()
{
	return (_DgtInputObj != nullptr) ? _DgtInputObj : (_DgtInputObj = new(nothrow) L20ScDgtInputTask());
}

/**************************************************************************//**
* 
* \brief   - Processing the Digital Input Data for ecternal.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ScDgtInputTask::ScDgtInput_Task(void)
{
	MESSAGE		ProcessBuffer;
	UINT32		events;	

	_DgtInputObj = ScDgtInputTask::GetInstance();
	if(nullptr != _DgtInputObj)
	{
		/* ScDgtInput Task loop and the bIsTaskRun flag enabled when task created */
		while(_DgtInputObj->bIsTaskRunStatus())
		{
			// wait for any one event		
			if(eventReceive(CTRL_1MS | DGT_INPUT_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				if(events & CTRL_1MS)
				{
					_DgtInputObj->GetDgtInputBits();
				}
				if(events & DGT_INPUT_QEVENT)
				{
					// process incoming messages
					_DgtInputObj->ProcessTaskMessage(ProcessBuffer);
				}
			}
		}
		
		delete _DgtInputObj;
	}
	else
	{
		LOGERR((char *)"DIGITAL_INPUT_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	_DgtInputObj = nullptr;
	taskSuspend(taskIdSelf());
}
