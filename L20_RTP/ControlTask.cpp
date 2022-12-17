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
#include "TestSonicsWorkFlow.h"
#include "WeldWorkFlow.h"
#include "UserInterface.h"
#include "DataTask.h"
#include "Recipe.h"
#include "SystemConfiguration.h"
#include "ExternalManager.h"
#include "ActuatorTask.h"
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
	ACT_MSG_Q_ID  = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]);
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
void ControlTask::updateWorkFlow(char* buff)
{
	int tmpOperationMode = SCStateMachine::NO_OPERATION;
	//Parse OperationMode following screen index
	unsigned int screenIndex;
	memcpy(&screenIndex, buff, sizeof(unsigned int));
	switch(screenIndex)
	{
	case DASHBORD_SCREEN:
		tmpOperationMode = SCStateMachine::BATCH_WELD;
		break;
	case SETUP_SCREEN:
		tmpOperationMode = SCStateMachine::WELD;
		break;
	case HEIGHT_CALIBRATION_SCREEN:
		tmpOperationMode = SCStateMachine::HEIGHT_CALIBRATE_READY;
		break;
	case MAINTAIN_SCREEN:
		//Intentional fall through
	case MAINTAIN_SONICS_SCREEN:
		//Intentional fall through
	case AMPLITUDE_CALIBRATINON_SCREEN:
		tmpOperationMode = SCStateMachine::TEST;
		break;
	default:
		tmpOperationMode = SCStateMachine::NO_OPERATION;
		break;
	}
	if((m_OperationMode == SCStateMachine::BATCH_WELD) || (m_OperationMode == SCStateMachine::WELD)) 
	{
		if((tmpOperationMode == SCStateMachine::WELD) || (tmpOperationMode == SCStateMachine::BATCH_WELD))
		{
			if(SCStateMachine::getInstance()->GetStateMachineState() == false)
				m_OperationMode = SCStateMachine::NO_OPERATION;
			else
			{
				m_OperationMode = tmpOperationMode;
				if(_WorkFlowObj != nullptr)
				{
					if(m_OperationMode == SCStateMachine::BATCH_WELD)
						((WeldWorkFlow*)_WorkFlowObj)->SetOperationMode(WeldWorkFlow::BATCH);
					else
						((WeldWorkFlow*)_WorkFlowObj)->SetOperationMode(WeldWorkFlow::SETUP);
				}
			}
			return;
		}
	}
	
	if(tmpOperationMode < SCStateMachine::END_OPERATION)
	{
		//To check if the new coming operation mode is equal to current operation mode. 
		if(m_OperationMode != tmpOperationMode)
		{
			//To load sequence following the new coming operation mode.
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
				case SCStateMachine::TEST:
					_WorkFlowObj = new TestSonicsWorkFlow();
					break;
				case SCStateMachine::WELD:
					_WorkFlowObj = new WeldWorkFlow();
					((WeldWorkFlow*)_WorkFlowObj)->SetOperationMode(WeldWorkFlow::SETUP);
					break;
				case SCStateMachine::BATCH_WELD:
					_WorkFlowObj = new WeldWorkFlow();
					((WeldWorkFlow*)_WorkFlowObj)->SetOperationMode(WeldWorkFlow::BATCH);
					break;
				default:
					_WorkFlowObj = nullptr;
					break;
				}
			}
			else
			{
				//TODO If the result is ERROR that means the current Operation Mode and Work Flow is not able to be changed through the screen change.
				//TODO The control task to send the message to UI task and UI task will notify HMI then.
			}
		}
	}
	else
		LOGERR((char *)"CTRL_T : ----------tmpOperationMode------------- : %d",tmpOperationMode, 0, 0);
}

/**************************************************************************//**
* \brief   - Run the specific work flow when the message is received from SC state machine
*
* \param   - None
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::responseStateMachineProcess(MESSAGE& message)
{
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
				else 
					message.msgID = UserInterface::TO_UI_TASK_HEIGHT_CHECK_RESPONSE;
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			case SCStateMachine::TEST:
				message.msgID = UserInterface::TO_UI_TASK_SONICS_TEST_RESPONSE;
				memcpy(message.Buffer, &TestSonicsWorkFlow::TestResult, sizeof(SONICS_TEST));
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			case SCStateMachine::WELD:
				_WorkFlowObj->UpdateResult();
				message.msgID = UserInterface::TO_UI_TASK_LAST_WELD_RESULT;
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			case SCStateMachine::BATCH_WELD:
				_WorkFlowObj->UpdateResult();
				if(((SCStateMachine::getInstance()->GetCoreState() & ERR_PRE_HEIGHT_PL) != ERR_PRE_HEIGHT_PL) &&
						((SCStateMachine::getInstance()->GetCoreState() & ERR_PRE_HEIGHT_MS) != ERR_PRE_HEIGHT_MS))
				{
					message.msgID = DataTask::TO_DATA_TASK_WELD_RESULT_INSERT;
					SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
					message.msgID = DataTask::TO_DATA_TASK_WELD_SIGN_INSERT;
					SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
					//TODO the operation should be added in power down checking in future.
					message.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
					int recipeID = Recipe::ActiveRecipeSC->m_RecipeID;
					memcpy(message.Buffer, &recipeID, sizeof(int));
					SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
				}
				message.msgID = UserInterface::TO_UI_TASK_LAST_WELD_RESULT;
				SendToMsgQ(message, UI_MSG_Q_ID);
					
				break;
			default:
				
				break;
			}
		}
		else if(status == WorkFlow::ONGOING)
		{
			switch(m_OperationMode)
			{
			case SCStateMachine::HEIGHT_CALIBRATE_READY:
				_WorkFlowObj->TriggerProcess();
				break;
			case SCStateMachine::TEST:
				message.msgID = UserInterface::TO_UI_TASK_SONICS_TEST_RESPONSE;
				memcpy(message.Buffer, &TestSonicsWorkFlow::TestResult, sizeof(SONICS_TEST));
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			case SCStateMachine::WELD:
				_WorkFlowObj->UpdateResult();
				message.msgID = UserInterface::TO_UI_TASK_LAST_WELD_RESULT;
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			case SCStateMachine::BATCH_WELD:
				_WorkFlowObj->UpdateResult();
				if(((SCStateMachine::getInstance()->GetCoreState() & ERR_PRE_HEIGHT_PL) != ERR_PRE_HEIGHT_PL) &&
						((SCStateMachine::getInstance()->GetCoreState() & ERR_PRE_HEIGHT_MS) != ERR_PRE_HEIGHT_MS))
				{
					message.msgID = DataTask::TO_DATA_TASK_WELD_RESULT_INSERT;
					SendToMsgQ(message,DATA_MSG_Q_ID_CTRL);
					message.msgID = DataTask::TO_DATA_TASK_WELD_SIGN_INSERT;
					SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
					//TODO the operation should be added in power down checking in future.
					message.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
					int recipeID = Recipe::ActiveRecipeSC->m_RecipeID;
					memcpy(message.Buffer, &recipeID, sizeof(int));
					SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
				}
				message.msgID = UserInterface::TO_UI_TASK_LAST_WELD_RESULT;
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
			default:
				break;
			}
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
		updateWorkFlow(message.Buffer);
		if((m_OperationMode == SCStateMachine::BATCH_WELD) || (m_OperationMode == SCStateMachine::WELD)) 
			tmp = TRUE;
		else
			tmp = FALSE;
		memcpy(message.Buffer, &tmp, sizeof(UINT32));
		message.msgID = ActuatorTask::TO_ACT_TASK_COOLING_TIMER_ENABLE;
		SendToMsgQ(message, ACT_MSG_Q_ID);
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
	case TO_CTRL_SONICS_TEST:
		if(_WorkFlowObj != nullptr)
		{
			TestSonicsWorkFlow* _TestSonicsProcess = (TestSonicsWorkFlow*)_WorkFlowObj;
			Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AMPLITUDE, &tmp);
			_TestSonicsProcess->SetAmplitude(tmp);
			_TestSonicsProcess->TriggerProcess();
		}
		break;
	case TO_CTRL_SONICS_100TEST:
		if(_WorkFlowObj != nullptr)
		{
			TestSonicsWorkFlow* _TestSonicsProcess = (TestSonicsWorkFlow*)_WorkFlowObj;
			SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::MAX_AMPLITUDE, &tmp);
			_TestSonicsProcess->SetAmplitude(tmp);
			_TestSonicsProcess->TriggerProcess();
		}
		break;
	case TO_CTRL_SC_RESPONSE:
		responseStateMachineProcess(message);
		break;
	case TO_CTRL_ALARM_EVENT:
		if(_WorkFlowObj != nullptr)
			_WorkFlowObj->ResetProcess();
		message.msgID = DataTask::TO_DATA_TASK_ALARM_LOG_INSERT;
		SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
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
