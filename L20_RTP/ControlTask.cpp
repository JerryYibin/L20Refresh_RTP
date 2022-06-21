/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

Changed the C code "Control_Task" to c++ static function. 
It's a good change for Control_Task so Control_Task can access all the source of
ControlTask owned using the class object pointer.    
 
***************************************************************************/

#include "ControlTask.h"

extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of ControlTask.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
ControlTask::ControlTask()
{
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_CTRL = CP->getMsgQId(Data_Task + "/Control");
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
ControlTask::~ControlTask() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief   - Process commands(WELD,SEEK,SETUP, SCAN or ALARM) received from MsgQ.
*
* \param   - struct Message&.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::ProcessTaskMessage(MESSAGE& message)
{
	switch(message.msgID)
	{
	default:
		LOGERR((char *)"CTRL_T : ----------Unknown Message ID------------- : %d",message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Controls state machine functionalities(TASK ROUTINE).  
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ControlTask::Control_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	
	ControlTask *OperateCtrl = new(nothrow) ControlTask();
	if(NULL != OperateCtrl)
	{
		/* Control Task loop and the bIsTaskRun flag enabled when task created */
		while(OperateCtrl->bIsTaskRunStatus())
		{
			if(msgQReceive(OperateCtrl->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR) 
			{
				OperateCtrl->Decode(MsgQBuffer,ProcessBuffer);
				OperateCtrl->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete OperateCtrl;
	}
	else
	{
		LOGERR((char *)"CTRL_T : -------------Memory allocation failed-----------",0,0,0);
	}
	
	OperateCtrl = NULL;
	taskSuspend(taskIdSelf());
}
