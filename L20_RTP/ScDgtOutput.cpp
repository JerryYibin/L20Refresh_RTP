/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     SC Digtial Output task for all the output signals of external operations  
 
**********************************************************************************************************/

#include "ScDgtOutput.h"
#include "L20ScDgtOutputTask.h"
#include "P1ScDgtOutputTask.h"
ScDgtOutputTask* ScDgtOutputTask::_DgtOutputObj = nullptr;

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of ScDgtOutputTask.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
ScDgtOutputTask::ScDgtOutputTask()
	:SELF_MSG_Q_ID(0)
	,UI_MSG_Q_ID(0)
{
	// Load the data message Q name
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T]);
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
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
ScDgtOutputTask::~ScDgtOutputTask()
{

}

/*************************************************************************//**
 * \brief   - Process the received message from OUTPUT_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void ScDgtOutputTask::ProcessTaskMessage(MESSAGE& message)
{
	switch(message.msgID)
	{
	case TO_DGT_OUTPUT_TASK_READY_SET:
		_DgtOutputObj->SetDgtReadyOutput();
		break;
	case TO_DGT_OUTPUT_TASK_READY_RESET:
		_DgtOutputObj->ResetDgtReadyOutput();
		break;
	case TO_DGT_OUTPUT_TASK_SONICS_SET:
		_DgtOutputObj->SetDgtSonicsOutput();
		break;
	case TO_DGT_OUTPUT_TASK_SONICS_RESET:
		_DgtOutputObj->ResetDgtSonicsOutput();
		break;
	case TO_DGT_OUTPUT_TASK_ALARM_SET:
		_DgtOutputObj->SetDgtAlarmOutput();
		break;
	case TO_DGT_OUTPUT_TASK_ALARM_RESET:
		_DgtOutputObj->ResetDgtAlarmOutput();
		break;
	default:
		LOGERR((char *)"ScDgtOutput : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* \brief  	- Get specific actuator object following system type
*
* \param	- None
*
* \return 	- ScDgtOutputTask object
*
******************************************************************************/
ScDgtOutputTask* ScDgtOutputTask::GetInstance()
{
	return (_DgtOutputObj != nullptr) ? _DgtOutputObj : (_DgtOutputObj = new(nothrow) L20ScDgtOutputTask());
}

/**************************************************************************//**
* 
* \brief   - Processing the Digital Output Data for external.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ScDgtOutputTask::ScDgtOutput_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	

	_DgtOutputObj = ScDgtOutputTask::GetInstance();

	if(nullptr != _DgtOutputObj)
	{
		/* ScDgtOutput Task loop and the bIsTaskRun flag enabled when task created */
		while(_DgtOutputObj->bIsTaskRunStatus())
		{
			if(msgQReceive(_DgtOutputObj->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
				_DgtOutputObj->Decode(MsgQBuffer, ProcessBuffer);
				_DgtOutputObj->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete _DgtOutputObj;
	}
	else
	{
		LOGERR((char *)"DIGITAL_OUTPUT_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	_DgtOutputObj = nullptr;
	taskSuspend(taskIdSelf());
}
