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
#include "HeightCalibrationUI.h"
#include "ActuatorTask.h"
#include "DataTask.h"
#include "RecipeWrapper.h"
#include "ScDgtInput.h"
#include "SystemConfiguration.h"
#include "SystemConfigurationUI.h"
extern "C"
{
	#include "customSystemCall.h"	
	#include "hwif/drv/resource/vxbRtcLib.h"
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
	ACT_MSG_Q_ID  = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]);
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_CTRL = CP->getMsgQId (Data_Task + "/Control");
	DATA_MSG_Q_ID_DATA = CP->getMsgQId (Data_Task + "/Data");
	DATA_MSG_Q_ID_REQ  = CP->getMsgQId (Data_Task + "/Request");

	INTERFACE_MSG_Q_ID	= CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	
	OUTPUT_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T]);
	INPUT_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTIN_T]);
	
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
	case TO_UI_TASK_USERIO_INPUT_READ:
		message.msgID = ScDgtInputTask::TO_DGT_INPUT_TASK_IO_GET;
		SendToMsgQ(message, INPUT_MSG_Q_ID);
		break;
	case TO_UI_TASK_HEIGHT_CALIBRATE_RESPONSE:
		responseHeightCalibration();
		break;
	case TO_UI_TASK_HEIGHT_CHECK_RESPONSE:
		responseHeightCheck();
		break;
	case TO_UI_TASK_USER_IO_RESPONSE:
		responseUserIORequest(message.Buffer);
		break;
	case TO_UI_RECIPE_LIBRARY_LATEST_PAGE_READ:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_RECIPE_LIBRARY_DATA_RESPONSE:
		responseWeldRecipeLib();
		break;
	case TO_UI_DEFAULT_RECIPE_GET:
		responseWeldRecipeData();
		break;
	case TO_UI_INSERT_RECIPE_WRITE:
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC);
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_INSERT;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_RECIPE_LIBRARY_NEXT_PAGE_READ:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_UPDATE_RECIPE:
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC);
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_DELETE_RECIPE:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_RECIPE_DETAILS_GET:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_QUERY;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_WELD_RECIPE_QUERY:
		responseWeldRecipeDetails();
		break;
	case TO_UI_RECIPE_ERRCODE:
		sendWeldRecipeErrCode(message.Buffer);
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
	sendMsg.msgLen = sizeof(WeldRecipeSCForUI);
	RecipeWrapper::Get(Recipe::ActiveRecipeSC, sendMsg.Buffer);
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

	
	sprintf(CommonProperty::SystemInfo.version_SC,"%d.%d.%d.%d",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR), 
											FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),
											FWVersion::Get(FW_VERSION_SC, VERSION_AUTOS));

	sprintf(CommonProperty::SystemInfo.version_DB,"%d.%d.%d.%d",FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),
											FWVersion::Get(FW_VERSION_PC, VERSION_AUTOS));

	sprintf(CommonProperty::SystemInfo.version_HMI,"%d.%d.%d.%d",FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),
											FWVersion::Get(FW_VERSION_AC, VERSION_AUTOS));
	
	snprintf(macAddr, sizeof(macAddr), "%02X:%02X:%02X:%02X:%02X:%02X",
			macaddr[0],
			macaddr[1],
			macaddr[2],
			macaddr[3],
			macaddr[4],
			macaddr[5]);

	sprintf(CommonProperty::SystemInfo.psMacID, "%s", macAddr);
	sprintf(CommonProperty::SystemInfo.psIP, "%s", "127.0.0.1");
	sprintf(CommonProperty::SystemInfo.modelName, "%s", "L20 Refresh");
	
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
* \brief   - response current User IO status ... 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseUserIORequest(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= SCBL_USER_IO_INPUT_REQ;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, messagebuf, sizeof(unsigned int));
	sendMsg.msgLen = sizeof(unsigned int);
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
* \brief   - Sends response to UIC for Weld Recipe Defaults request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseWeldRecipeData()
{
	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_DEFAULT_RECIPE_IDX;
	sendMsg.msgLen = sizeof(WeldRecipeSCForUI);
	sendMsg.rspCode = 0;
	RecipeWrapper::Get(Recipe::RecipeSC, sendMsg.Buffer);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	Recipe::RecipeSC = nullptr;
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for Weld Recipe details request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseWeldRecipeDetails()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_SELECTED_RECIPE_INFO_REQ;
	sendMsg.msgLen = sizeof(WeldRecipeSCForUI);
	sendMsg.rspCode = 0;
	RecipeWrapper::Get(Recipe::RecipeSC, sendMsg.Buffer);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	Recipe::RecipeSC = nullptr;
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for Weld Recipe library request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseWeldRecipeLib()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_RECIPE_LIBRARY_READ_IDX;
	sendMsg.msgLen = Recipe::WeldRecipeLibraryForUI.size() * sizeof(WeldRecipeLibForUI);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &Recipe::WeldRecipeLibraryForUI[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	Recipe::WeldRecipeLibraryForUI.clear();
}

/**************************************************************************//**
* 
* \brief   - Send the error code obtained by operating the data to the UI. 
*
* \param   - Message buffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::sendWeldRecipeErrCode(char* recipebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	sendMsg.msgID	= REQ_RECIPE_ERROR_CODE_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	sendMsg.msgLen = sizeof(int);
	memcpy(sendMsg.Buffer, recipebuf, sizeof(int));
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
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
	SystemConfiguration::_SystemConfig->Init();
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

	SYSTEMCONFIGFORUI sysConfigUI;
	SystemConfiguration::Get(&sysConfigUI);
	sendMsg.msgLen = sizeof(SYSTEMCONFIGFORUI);
	memcpy(sendMsg.Buffer, reinterpret_cast<char*>(&sysConfigUI), sendMsg.msgLen);
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
	SYSTEMCONFIGFORUI sysConfigUI;
	memcpy(reinterpret_cast<char*>(&sysConfigUI), messagebuf, sizeof(SYSTEMCONFIGFORUI));
	SystemConfiguration::Set(&sysConfigUI);
}
