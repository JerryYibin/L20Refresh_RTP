/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

Changed the C code "Control_Task" to c++ static function. 
It's a good change for Control_Task so Control_Task can access all the source of
ControlTask owned using the class object pointer. 
For control task it shall be responsible for the basic weld cycle by SC state machine.
The work flow of sequence, teach mode, batch size, height calibration shall be implemented in the control task.    
The SC state machine shall run the generic work flow. 
***************************************************************************/

#include "ControlTask.h"
#include "SCStateMachine/SCStateMachine.h"
#include "Utils.h"
#include "SCStateMachine/SCState.h"
#include "ACStateMachine.h"
#include "HeightCalibrateWorkFlow.h"
#include "UserInterface.h"
extern "C"
{
	#include "customSystemCall.h"	
}


WorkFlow* ControlTask::_WorkFlowObj = nullptr;
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
	int tmpOperationMode = SCStateMachine::NO_OPERATION;
	unsigned int screenIndex;
	memcpy(&screenIndex, buff, sizeof(unsigned int));
	switch(screenIndex)
	{
	case DASHBORD_SCREEN:
	case SETUP_SCREEN:
		tmpOperationMode = SCStateMachine::WELD;
		break;
//	case HEIGHT_CALIBRATION_SCREEN:
// TODO It is the HMI software bug, it is not able to send the correct to SC so far. In order to follow with the HMI current status, The magic code has to 
// add to instead of the index.
	case 9:
		tmpOperationMode = SCStateMachine::HEIGHT_CALIBRATE_READY;
		break;
	default:
		tmpOperationMode = SCStateMachine::NO_OPERATION;
		break;
	}

	if(SCStateMachine::getInstance()->GetStateMachineState() == false)
		m_OperationMode = SCStateMachine::NO_OPERATION;
	if((tmpOperationMode > SCStateMachine::NO_OPERATION) && (tmpOperationMode < SCStateMachine::END_OPERATION))
	{
		if(m_OperationMode != tmpOperationMode)
		{
			if(SCStateMachine::getInstance()->LoadStatesHandler(tmpOperationMode) == OK)
			{
				m_OperationMode = tmpOperationMode;
				if(_WorkFlowObj != nullptr)
					delete _WorkFlowObj;
				switch(m_OperationMode)
				{
				case SCStateMachine::HEIGHT_CALIBRATE_READY:
					_WorkFlowObj = new HeightCalibrateWorkFlow();
					break;
				default:
					_WorkFlowObj = nullptr;
					break;
				}
			}
		}
	}
}

/**************************************************************************//**
* \brief   - Run the specific work flow when the message is received from SC state machine
*
* \param   - None
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::responseStateMachineProcess()
{
	MESSAGE message;
	HeightCalibrateWorkFlow* _HeightCalibrateProcess = nullptr;
	if(_WorkFlowObj != nullptr)
	{
		int status = _WorkFlowObj->RunProcess();
		if(status == WorkFlow::FINISH)
		{
			switch(m_OperationMode)
			{
			case SCStateMachine::HEIGHT_CALIBRATE_READY:
				_HeightCalibrateProcess = (HeightCalibrateWorkFlow*)_WorkFlowObj;
				if(_HeightCalibrateProcess->GetOperationMode() == HeightCalibrateWorkFlow::HEIGHT_CALIBRATE)
					message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CALIBRATE_RESPONSE;
				else if(_HeightCalibrateProcess->GetOperationMode() == HeightCalibrateWorkFlow::HEIGHT_CHECK)
					message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CHECK_RESPONSE;
				else
				{
					
				}
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			default:
				break;
			}
		}
		else if(status == WorkFlow::ONGOING)
		{
			_WorkFlowObj->TriggerProcess();
		}
		else
		{
			
		}
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
	unsigned int tmp;
	switch(message.msgID)
	{
	case TO_CTRL_OPERATE_MODE_SET:
		updateOperationMode(message.Buffer);
		break;
	case TO_CTRL_TRIGGER_HEIGHT_CALIBRATE:
		if(_WorkFlowObj != nullptr)
		{
			HeightCalibrateWorkFlow* _HeightCalibrateProcess = (HeightCalibrateWorkFlow*)_WorkFlowObj;
			_HeightCalibrateProcess->SetOperationMode(HeightCalibrateWorkFlow::HEIGHT_CALIBRATE);
			_HeightCalibrateProcess->InitProcess();
			_HeightCalibrateProcess->TriggerProcess();
		}
		break;
	case TO_CTRL_TRIGGER_HEIGHT_CHECK:
		memcpy(&tmp, message.Buffer, sizeof(unsigned int));
		if(_WorkFlowObj != nullptr)
		{
			HeightCalibrateWorkFlow* _HeightCalibrateProcess = (HeightCalibrateWorkFlow*)_WorkFlowObj;
			_HeightCalibrateProcess->SetOperationMode(HeightCalibrateWorkFlow::HEIGHT_CHECK);
			_HeightCalibrateProcess->SetPressure(tmp);
			_HeightCalibrateProcess->TriggerProcess();
		}
		break;
	case TO_CTRL_HEIGHT_CORRECT:
		memcpy(&tmp, message.Buffer, sizeof(unsigned int));
		if(_WorkFlowObj != nullptr)
		{
			HeightCalibrateWorkFlow* _HeightCalibrateProcess = (HeightCalibrateWorkFlow*)_WorkFlowObj;
			_HeightCalibrateProcess->SetCorrectHeight(tmp);
			_HeightCalibrateProcess->UpdateResult();
		}
		break;
	case TO_CTRL_SC_RESPONSE:
		responseStateMachineProcess();
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
