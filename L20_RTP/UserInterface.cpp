/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Interface task 
     Changed the C code "UserInterface_Task" to c++ static function. 
It's a good change for UserInterface_Task so UserInterface_Task can access all the source of
UserInterface owned using the class object pointer. 
 
**********************************************************************************************************/

#include "UserInterface.h"
#include "Recipe.h"
#include "CommunicationInterfaceHMI.h"
#include "ControlTask.h"
#include "SCStateMachine/SCStateMachine.h"
extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of UserInterface.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
UserInterface::UserInterface()
{
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_CTRL = CP->getMsgQId (Data_Task + "/Control");
	DATA_MSG_Q_ID_DATA = CP->getMsgQId (Data_Task + "/Data");
	DATA_MSG_Q_ID_REQ  = CP->getMsgQId (Data_Task + "/Request");

	INTERFACE_MSG_Q_ID	= CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
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
UserInterface::~UserInterface() {
	// TODO Auto-generated destructor stub
}

/*************************************************************************//**
 * \brief   - Process the received message from UI_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void UserInterface::ProcessTaskMessage(MESSAGE& message)
{
	UINT32 msgLen = 0;
	UINT8 status;
	INT32 socketStatus = 0;
	string details;
	char signal;
	
	switch(message.msgID)
	{
	case TO_UI_TASK_HEART_BEAT:
		updateOperationMode(message.Buffer);
		responseHeartbeat();
		break;
	case TO_UI_TASK_ACTIVE_RECIPE_INFO:
		getActiveRecipe();
		break;
	case TO_UI_TASK_SETUP_WELD_PARAM:
		updateActiveRecipeGeneralParam(message.Buffer);
		break;
	case TO_UI_TASK_SETUP_QUALITY_PARAM:
		updateActiveRecipeQualityParam(message.Buffer);
		break;
	case TO_UI_TASK_SETUP_ADVANCED_PARAM:
		updateActiveRecipeAdvancedParam(message.Buffer);
		break;
	default:
		LOGERR((char *)"UI_T : --------Unknown Message ID----------- : %d",message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for heart beat request with weld result
* 			 counter. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseHeartbeat()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	
	sendMsg.msgID	= REQ_HEART_BEAT_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	
	HEARTBEAT stHeartbeat;
	stHeartbeat.AlarmCode = 0;
	stHeartbeat.Amplitude = CommonProperty::WeldResult.Amplitude;
	stHeartbeat.CycleCounter = CommonProperty::WeldResult.CycleCounter;
	stHeartbeat.PeakPower = CommonProperty::WeldResult.PeakPower;
	stHeartbeat.PostHeight = CommonProperty::WeldResult.PostHeight;
	stHeartbeat.PreHeight = CommonProperty::WeldResult.PreHeight;
	stHeartbeat.RecipeNumber = CommonProperty::WeldResult.RecipeNum;
	stHeartbeat.TotalEnergy = CommonProperty::WeldResult.TotalEnergy;
	stHeartbeat.TriggerPress = CommonProperty::WeldResult.TriggerPressure;
	stHeartbeat.WeldPress = CommonProperty::WeldResult.WeldPressure;
	stHeartbeat.WeldTime = CommonProperty::WeldResult.WeldTime;
	
	memcpy(sendMsg.Buffer, &stHeartbeat, sizeof(HEARTBEAT));
	sendMsg.msgLen = sizeof(HEARTBEAT);
	
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for active recipe getting request
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::getActiveRecipe()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID	= REQ_ACTIVE_RECIPE_INFO_IDX;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &CommonProperty::ActiveRecipeSC, sizeof(WeldRecipeSC));
	sendMsg.msgLen = sizeof(WeldRecipeSC);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Update active weld recipe general parameter which is received from UIC. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateActiveRecipeGeneralParam(char* recipebuf)
{
	Recipe::SetActiveRecipeGeneralParam((WeldParameterSetting*)recipebuf);
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID	= REQ_SETUP_WELD_PARAM_IDX;
	sendMsg.rspCode = 0;
	sendMsg.msgLen = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Update active weld recipe quality parameter which is received from UIC. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateActiveRecipeQualityParam(char* recipebuf)
{
	Recipe::SetActiveRecipeQualityParam((QualityWindowSetting*)recipebuf);
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID	= REQ_SETUP_QUALITY_PARAM_IDX;
	sendMsg.rspCode = 0;
	sendMsg.msgLen = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Update active weld recipe advanced parameter which is received from UIC. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateActiveRecipeAdvancedParam(char* recipebuf)
{
	Recipe::SetActiveRecipeAdvancedParam((AdvancedWeldSetting*)recipebuf);
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID	= REQ_SETUP_ADVANCED_PARAM_IDX;
	sendMsg.rspCode = 0;
	sendMsg.msgLen = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Update operate mode in control task following screen index that is from UIC. 
*
* \param   - Message buffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateOperationMode(char* messagebuf)
{
	MESSAGE message;
	message.msgID = ControlTask::TO_CTRL_OPERATE_MODE_IDX;
	unsigned int* screenIndex = (unsigned int*)messagebuf;
	unsigned int operationMode = SCStateMachine::NO_OPERATION;
	switch(*screenIndex)
	{
	case DASHBORD_SCREEN:
		operationMode = SCStateMachine::WELD;
		break;
	default:
		operationMode = SCStateMachine::NO_OPERATION;
		break;
	}
	memcpy(message.Buffer, &operationMode, sizeof(unsigned int));
	SendToMsgQ(message, CTRL_MSG_Q_ID);
}

/**************************************************************************//**
* 
* \brief   - Processing the User data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UserInterface_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	
	UserInterface *UI = new(nothrow) UserInterface();
	
	if(NULL != UI)
	{
//		UI->PowerOnRequest(FW_VERSION);
//		UI->PowerOnRequest(POWER_ON_RECIPE_READ);
//		UI->PowerOnRequest(CALIBRATION_TIME_STAMP);
//		Counters::loadCounters();

		/* UserInterface Task loop and the bIsTaskRun flag enabled when task created */
		while(UI->bIsTaskRunStatus())
		{
			if(msgQReceive(UI->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR) 
			{
				UI->Decode(MsgQBuffer, ProcessBuffer);
				UI->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete UI;
	}
	else
	{
		LOGERR((char *)"UI_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	
	UI = NULL;	
	taskSuspend(taskIdSelf());
}
