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
#include "versions.h"
#include "HeightEncoder.h"
#include "HeightCalibration.h"
#include "ActuatorTask.h"
extern "C"
{
	#include "customSystemCall.h"	
	#include "hwif/drv/resource/vxbRtcLib.h"
}

auto _SystemConfig = SYSTEMCONFIG::GetSystemConfig();;

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
	ACT_MSG_Q_ID  = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]);
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
		message.msgID = ActuatorTask::TO_ACT_TASK_PRESSURE_SET;
		SendToMsgQ(message, ACT_MSG_Q_ID);
		break;
	case TO_UI_TASK_SETUP_QUALITY_PARAM:
		updateActiveRecipeQualityParam(message.Buffer);
		break;
	case TO_UI_TASK_SETUP_ADVANCED_PARAM:
		updateActiveRecipeAdvancedParam(message.Buffer);
		break;
	case TO_UI_TASK_SYSINFO_READ:
		responseSystemInfo();
		break;
	case TO_UI_TASK_LAST_WELD_RESULT:
		updateLastWeldResult();
		break;
	case TO_UI_TASK_SYSCONFIG_READ:
		responseSystemConfig();
		break;
	case TO_UI_TASK_SYSCONFIG_WRITE:
		updateSystemConfigData(message.Buffer);
		break;
	case TO_UI_TASK_INITIALIZATION:
		responseInitializationData();
		break;
	case TO_UI_TASK_HEIGHT_CALIBRATE_START:
		message.msgID = ControlTask::TO_CTRL_TRIGGER_HEIGHT_CALIBRATE;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_HEIGHT_CHECK:
		message.msgID = ControlTask::TO_CTRL_TRIGGER_HEIGHT_CHECK;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_HEIGHT_CORRECT:
		message.msgID = ControlTask::TO_CTRL_HEIGHT_CORRECT;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_HEIGHT_CALIBRATE_ACCEPT:
		//TODO send message to Database
		break;
	case TO_UI_TASK_HEIGHT_CALIBRATE_RESPONSE:
		responseHeightCalibration();
		break;
	case TO_UI_TASK_HEIGHT_CHECK_RESPONSE:
		responseHeightCheck();
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
	
	memcpy(sendMsg.Buffer, &m_stHeartbeat, sizeof(HEARTBEAT));
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
	message.msgID = ControlTask::TO_CTRL_OPERATE_MODE_SET;
	unsigned int screenIndex;
	memcpy(&screenIndex, messagebuf, sizeof(unsigned int));
	if(screenIndex < SCREEN_END)
	{
		memcpy(message.Buffer, &screenIndex, sizeof(unsigned int));
		SendToMsgQ(message, CTRL_MSG_Q_ID);
	}
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for system information request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseSystemInfo()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= REQ_SYSTEM_INFO_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	string timeStamp;
	char macaddr[6] = {0,0,0,0,0,0};
	char macAddr[20]={0x00};

	
	sprintf(CommonProperty::SystemInfo.version_SC,"%d.%d.%d",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR), 
											FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));

	sprintf(CommonProperty::SystemInfo.version_DB,"%d.%d.%d",FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));

	sprintf(CommonProperty::SystemInfo.version_HMI,"%d.%d.%d",FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
	
	snprintf(macAddr, sizeof(macAddr), "%02X:%02X:%02X:%02X:%02X:%02X",
			macaddr[0],
			macaddr[1],
			macaddr[2],
			macaddr[3],
			macaddr[4],
			macaddr[5]);

	sprintf(CommonProperty::SystemInfo.psMacID, "%s", macAddr);
	sprintf(CommonProperty::SystemInfo.psIP, "%s", "127.0.0.1");
	sprintf(CommonProperty::SystemInfo.modelName, "%s", "L20_Refresh");
	
	memset(CommonProperty::SystemInfo.dateTime, 0, SYSINFO_SIZE * 2);
	getDateAndTime(CommonProperty::SystemInfo.dateTime);
	
	memcpy(sendMsg.Buffer, &CommonProperty::SystemInfo, sizeof(SYSTEM_INFO));
	sendMsg.msgLen = sizeof(SYSTEM_INFO);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Get current date and time 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::getDateAndTime(char* timebuf)
{
	struct tm time;
	vxbRtcGet(&time);
	sprintf(timebuf,"%d/%02d/%02d",time.tm_year+1900,time.tm_mon+1,time.tm_mday);
}

/**************************************************************************//**
* 
* \brief   - response Height Calibration Result repCode: 0 successful; 1 failure
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseHeightCalibration()
{
	unsigned int iResult = 0;
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= REQ_HEIGHT_CALIBRATE_START_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &iResult, sizeof(unsigned int));
	sendMsg.msgLen = sizeof(unsigned int);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - response Height check result as following... 
* 	struct HEIGHT_PROPERTY
*	{
*		unsigned int ActualHeight;
*		unsigned int Pressure;
*		unsigned int Stroke;
*		unsigned int HornUpTime;
*		unsigned int HornDownTime;
*		unsigned int TotalTime;
*	};
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseHeightCheck()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= REQ_HEIGHT_CALIBRATE_CHECK_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &HeightEncoder::HeightProperty, sizeof(HEIGHT_PROPERTY));
	sendMsg.msgLen = sizeof(HEIGHT_PROPERTY);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
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

void UserInterface::updateLastWeldResult()
{
	m_stHeartbeat.AlarmCode = 0;
	m_stHeartbeat.Amplitude = CommonProperty::WeldResult.Amplitude;
	m_stHeartbeat.CycleCounter = CommonProperty::WeldResult.CycleCounter;
	m_stHeartbeat.PeakPower = CommonProperty::WeldResult.PeakPower;
	m_stHeartbeat.PostHeight = CommonProperty::WeldResult.PostHeight;
	m_stHeartbeat.PreHeight = CommonProperty::WeldResult.PreHeight;
	m_stHeartbeat.RecipeNumber = CommonProperty::WeldResult.RecipeNum;
	m_stHeartbeat.TotalEnergy = CommonProperty::WeldResult.TotalEnergy;
	m_stHeartbeat.TriggerPress = CommonProperty::WeldResult.TriggerPressure;
	m_stHeartbeat.WeldPress = CommonProperty::WeldResult.WeldPressure;
	m_stHeartbeat.WeldTime = CommonProperty::WeldResult.WeldTime;
}

/**************************************************************************//**
* 
* \brief   - Initialize system configuration. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseInitializationData()
{
	_SystemConfig->InitialValue();
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for system configuration request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseSystemConfig()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= TO_UI_TASK_SYSCONFIG_READ;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;

	if(_SystemConfig)
	{
		sendMsg.msgLen = _SystemConfig->Size();
		memcpy(sendMsg.Buffer, reinterpret_cast<char*>(_SystemConfig.get()) + V_PTR_SIZE, sendMsg.msgLen);
	}
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Write system configuration.  
*
* \param   - Message buffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateSystemConfigData(char* messagebuf)
{
	if(_SystemConfig)
	{
		memcpy(reinterpret_cast<char*>(_SystemConfig.get()) + V_PTR_SIZE, messagebuf, _SystemConfig->Size());
	}
}
