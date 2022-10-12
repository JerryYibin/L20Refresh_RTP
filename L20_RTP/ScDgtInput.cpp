/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Database Interface task for DB related read or write operations  
 
**********************************************************************************************************/

#include "ScDgtInput.h"

extern "C"
{
	#include "customSystemCall.h"	
}

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

	switch(message.msgID)
	{
	default:
		LOGERR((char *)"ScDgtInput_T : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
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
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	

	ScDgtInputTask *DigitalInput = new(nothrow) ScDgtInputTask();
	if(nullptr != DigitalInput)
	{
		/* ScDgtInput Task loop and the bIsTaskRun flag enabled when task created */
		while(DigitalInput->bIsTaskRunStatus())
		{
			if(msgQReceive(DigitalInput->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
				DigitalInput->Decode(MsgQBuffer, ProcessBuffer);
				DigitalInput->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete DigitalInput;
	}
	else
	{
		LOGERR((char *)"DIGITAL_INPUT_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	DigitalInput = nullptr;
	taskSuspend(taskIdSelf());
}
