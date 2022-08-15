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
#include "SCStateMachine/SCStateMachine.h"
#include "Utils.h"
#include "SCStateMachine/SCState.h"
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
	:m_OperationMode(SCStateMachine::NO_OPERATION)
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
* \brief   - Load states following current operation mode
*
* \param   - message buffer
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::updateOperationMode(char* buff)
{
	unsigned int* tmpOperationMode = (unsigned int*)buff;
	if(SCStateMachine::getInstance()->GetStateMachineState() == false)
		m_OperationMode = SCStateMachine::NO_OPERATION;
	if(m_OperationMode != *tmpOperationMode)
	{
		if(SCStateMachine::getInstance()->LoadStatesHandler(*tmpOperationMode) == OK)
			m_OperationMode = *tmpOperationMode;
	}
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
	case TO_CTRL_OPERATE_MODE_IDX:
		updateOperationMode(message.Buffer);
		break;
	case TO_CTRL_CALIBRATE_START_IDX:
		SCStateMachine::getInstance()->ExecuteStateAction(SCState::READY_FOR_REQUEST);
		break;	
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
