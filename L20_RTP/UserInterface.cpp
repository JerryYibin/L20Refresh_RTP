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
#include "PowerSupplyTask.h"
#include "DataTask.h"
#include "RecipeWrapper.h"
#include "ScDgtInput.h"
#include "SystemConfiguration.h"
#include "SystemConfigurationUI.h"
#include "SonicsTestUI.h"
#include "Connectivity.h"
#include "RecipeUI.h"
#include "WeldResultSignature.h"
#include "AlarmManager.h"
#include "AlarmLog.h"
#include "UserAuthority.h"
#include "UserPrivilege.h"
extern "C"
{
	#include "customSystemCall.h"	
	#include "hwif/drv/resource/vxbRtcLib.h"
	#include "net/if.h"
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
	PS_MSG_Q_ID   = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]);
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
	int tmp = 0;
	switch(message.msgID)
	{
	case TO_UI_TASK_HEART_BEAT:
		updateOperationMode(message.Buffer);
		responseHeartbeat();
		break;
	case TO_UI_TASK_ACTIVE_RECIPE_INFO:
		message.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_QUERY;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_ACTIVE_RECIPE_INFO_RESPONSE:
		getActiveRecipe();
		break;
	case TO_UI_TASK_SETUP_WELD_PARAM:
		updateActiveRecipeGeneralParam(message.Buffer);
		message.msgID = ActuatorTask::TO_ACT_TASK_PRESSURE_SET;
		SendToMsgQ(message, ACT_MSG_Q_ID);
		message.msgID = PowerSupplyTask::TO_PS_TASK_AMPLITUDE_UPDATE;
		SendToMsgQ(message, PS_MSG_Q_ID);
		break;
	case TO_UI_TASK_SETUP_QUALITY_PARAM:
		updateActiveRecipeQualityParam(message.Buffer);
		break;
	case TO_UI_TASK_SETUP_ADVANCED_PARAM:
		updateActiveRecipeAdvancedParam(message.Buffer);
		break;
	case TO_UI_ACTIVE_RECIPE_VALIDATE:
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC.get());
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
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
		message.msgID = DataTask::TO_DATA_TASK_SYS_CONFIG_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_INITIALIZATION:
		responseInitializationData();
		message.msgID = DataTask::TO_DATA_TASK_SYS_CONFIG_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
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
		message.msgID = DataTask::TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_HEIGHT_CALIBRATE_EXIT:
		message.msgID = DataTask::TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY;
		SendToMsgQ(message, DATA_MSG_Q_ID_REQ);
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
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC.get());
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_INSERT;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_RECIPE_LIBRARY_NEXT_PAGE_READ:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_UPDATE_RECIPE:
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC.get());
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
	case TO_UI_SET_ACTIVE_RECIPE:
		//The Cycle Counter needs to be reset when the current active recipe is changed.
		//The Batch Size needs to be reset with default setting when the current active recipe is changed.
		tmp = BATCH_SIZE_NUM;
		WeldResults::_WeldResults->CycleCounter = 0;
		Recipe::ActiveRecipeSC->Set(&tmp, WeldRecipeSC::BATCH_SIZE);
		message.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		// TODO: GET ACTIVE
		break;
	case TO_UI_TASK_AUX_MOTION_REQ:
		message.msgID = ActuatorTask::TO_ACT_TASK_AUX_MOTION;
		SendToMsgQ(message, ACT_MSG_Q_ID);
		break;
	case TO_UI_TASK_SONICS_TEST_REQ:
		message.msgID = ControlTask::TO_CTRL_SONICS_TEST;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_SONICS_100TEST_REQ:
		message.msgID = ControlTask::TO_CTRL_SONICS_100TEST;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_SONICS_STOP_REQ:
		message.msgID = ControlTask::TO_CTRL_SONICS_STOP;
		SendToMsgQ(message, CTRL_MSG_Q_ID);
		break;
	case TO_UI_TASK_SETUP_CLICKET_PARAM:
		message.msgID = ActuatorTask::TO_ACT_TASK_PRESSURE_SET;
		SendToMsgQ(message, ACT_MSG_Q_ID);
		message.msgID = PowerSupplyTask::TO_PS_TASK_AMPLITUDE_UPDATE;
		SendToMsgQ(message, PS_MSG_Q_ID);
		break;
	case TO_UI_TASK_SONICS_TEST_RESPONSE:
		responseSonicsTest(message.Buffer);
		break;
	case TO_UI_TASK_DETECT_USB_DEVICE:
		responseDetectUSBdevice();
		break;
	case TO_UI_TASK_FIRMWARE_UPGRADE_RESPONSE:
		responseFirmwareUPgradeHandler();
		break;
	case TO_UI_TASK_FIRMWARE_UPGRADE_ERROR_MESSADE:
	case TO_UI_TASK_FIRMWARE_UPGRADE_PROGRESS_MESSADE:
		responseFirmwareUpgradeProgress(message.Buffer);
		break;
	case TO_UI_TASK_ETHERNET_CONFIG_SET:
		setEthernetConfigData(message.Buffer);
		message.msgID = DataTask::TO_DATA_TASK_CONNECTIVITY_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_ETHERNET_CONFIG_GET:
		responseEthernetConfigData();
		break;
	case TO_UI_TASK_GATEWAY_SERVER_GET:
		responseGatewayServerData();
		break;	
	case TO_UI_TASK_RECIPE_TOTAL_NUMBER_GET:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_TOTAL_NUMBER;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_RECIPE_TOTAL_NUMBER_RESPONSE:
		responseWeldRecipeTotalNumber(message.Buffer);
		break;
	case TO_UI_TASK_WELD_RESULT_HISTORY_LOG:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_WELD_RESULT_HISTORY_NEXT_PAGE:
		message.msgID = DataTask::TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_RESULT_LIBRARY_DATA_RESPONSE:
		responseWeldResultLib();
		break;
	case TO_UI_TASK_WELD_RECIPE_RENAME:
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
		RecipeWrapper::Set(message.Buffer, Recipe::RecipeSC.get());
		message.msgID = DataTask::TO_DATA_TASK_WELD_RECIPE_RENAME;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_READ_POWER_GRAPH_RESPONSE:
		responseReadPowerGraphRequest();
		break;
	case TO_UI_TASK_READ_HEIGHT_GRAPH_RESPONSE:
		responseReadHeightGraphRequest();
		break;
	case TO_UI_TASK_READ_FREQUENCY_GRAPH_RESPONSE:
		responseReadFrquencyGraphRequest();
		break;
	case TO_UI_TASK_GET_CURRENT_ALARM_IDX:
		responseCurrentAlarmEventRequest();
		break;
	case TO_UI_TASK_RESET_CURRENT_ALARM_IDX:
		responseCurrentAlarmEventReset();
		break;	
	case TO_UI_TASK_ALARM_LOG_LATEST_PAGE_IDX:
		message.msgID = DataTask::TO_DATA_TASK_ALARM_LOG_QUERY_LATEST_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_ALARM_LOG_NEXT_PAGE_IDX:
		message.msgID = DataTask::TO_DATA_TASK_ALARM_LOG_QUERY_NEXT_PAGE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_ALARM_LOG_LIBRARY_RESPONSE:
		responseAlarmLogLib();
		break;	
	case TO_UI_TASK_PERMISSION_SCREEN_GET:
		responsePermissionScreenGetRequest();
		break;
	case TO_UI_TASK_PERMISSION_SCREEN_SET:
		setScreenPermission(message.Buffer);
		message.msgID = DataTask::TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_PASSCODE_LIST_GET:
		responsePasscodeListGetRequest();
		break;
	case TO_UI_TASK_PASSCODE_UPDATE:
		updateUserPasscode(message.Buffer);
		//Send message to data task for updating DB.
		message.msgID = DataTask::TO_DATA_TASK_USER_PROFILE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_VALIDATE_PASSCODE:
		responseCheckPasscodeRequest(message.Buffer);
		break;
	case TO_UI_TASK_PERMISSION_SCREEN_SET_RESPOMSE:
		responsePermissionScreenSetRequest(message.Buffer);
		break;
	case TO_UI_TASK_PASSCODE_UPDATE_RESPOMSE:
		responseUpdatePasscodeRequest(message.Buffer);
		break;
	case TO_UI_TASK_AMPLITUDE_CALIBRATE_SAVE_IDX:
		SystemConfiguration::_SystemConfig->Set(SYSTEMCONFIG::MAX_AMPLITUDE, message.Buffer);
		message.msgID = DataTask::TO_DATA_TASK_SYS_CONFIG_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
		break;
	case TO_UI_TASK_READ_WELDRESULTHISTORY_POWER_GRAPH:
		getWeldReslutHistoryPowerDataByID(message.Buffer);
		break;
	case TO_UI_TASK_READ_WELDRESULTHISTORY_HEIGHT_GRAPH:
		getWeldReslutHistoryHeightDataByID(message.Buffer);
		break;
	case TO_UI_TASK_READ_WELDRESULTHISTORY_FREQUENCY_GRAPH:
		getWeldReslutHistoryFrequencyDataByID(message.Buffer);
		break;
	case TO_UI_TASK_GET_WELDRESULTHISTORY_POWER_DATA:
		responseReadHistoryPowerGraphRequest();
		break;
	case TO_UI_TASK_GET_WELDRESULTHISTORY_HEIGHT_DATA:
		responseReadHistoryHeightGraphRequest();
		break;
	case TO_UI_TASK_GET_WELDRESULTHISTORY_FREQUENCY_DATA:
		responseReadHistoryFrquencyGraphRequest();
		break;
	case TO_UI_TASK_CLEAR_WELD_COUNTER_IDX:
		WeldResults::_WeldResults->CycleCounter = 0;
		memcpy(message.Buffer, &Recipe::ActiveRecipeSC->m_RecipeID, sizeof(int));
		message.msgID = DataTask::TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
		SendToMsgQ(message, DATA_MSG_Q_ID_DATA);
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
	m_stHeartbeat.AlarmCode = AlarmManager::GetInstance()->GetAlarmType();
	m_stHeartbeat.CycleCounter = WeldResults::_WeldResults->CycleCounter;
	m_stHeartbeat.RecipeNumber = Recipe::ActiveRecipeSC->m_RecipeID;
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
	RecipeWrapper::Get(Recipe::ActiveRecipeSC.get(), sendMsg.Buffer);
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

	char macaddr[6] = {0x00,0x40,0x47,0xE0,0xA8,0x32};
	char macAddr[20]={0x00};
	char ipAddr[SYSINFO_SIZE] = {0};
	//Get IP address
	getIPaddress(ipAddr);
	sprintf(CommonProperty::SystemInfo.version_SC,"%d.%d.%d.%d",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR), 
											FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),
											FWVersion::Get(FW_VERSION_SC, VERSION_AUTOS));

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
	memcpy(&CommonProperty::SystemInfo.psIP, &ipAddr, SYSINFO_SIZE);
	sprintf(CommonProperty::SystemInfo.modelName, "%s", "GMX-L20A");
	
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
* \brief   - Set current system date and time 
*
* \param   - char* datetime
*
* \return  - None
*
******************************************************************************/
void UserInterface::setSystemTime(char* datetime)
{
	struct tm time;
	sscanf_s(datetime, "%d/%02d/%02d %02d:%02d:%02d", &time.tm_year, &time.tm_mon,&time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon  -=1;
    vxbRtcSet(&time);
}
/**************************************************************************//**
* 
* \brief   - Get current system date and time 
*
* \param   - char* datetime
*
* \return  - None
*
******************************************************************************/
void UserInterface::getSystemTime(char* datetime)
{
	struct tm time;
	vxbRtcGet(&time);
	sprintf_s(datetime,TIME_SIZE,"%d/%02d/%02d %02d:%02d:%02d",time.tm_year+1900,time.tm_mon+1,time.tm_mday,time.tm_hour,
			time.tm_min,time.tm_sec);
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
* \brief   - response Sonics Test current Power and Frequency 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseSonicsTest(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= REQ_MAINTAIN_SONICS_TEST_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, messagebuf, sizeof(SONICS_TEST));
	sendMsg.msgLen = sizeof(SONICS_TEST);
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
	m_stHeartbeat.Amplitude = WeldResults::_WeldResults->Amplitude;
	m_stHeartbeat.CycleCounter = WeldResults::_WeldResults->CycleCounter;
	m_stHeartbeat.PeakPower = WeldResults::_WeldResults->PeakPower;
	WeldResults::_WeldResults->Get(WeldResults::PARALIST::POST_HEIGHT, &m_stHeartbeat.PostHeight);
	WeldResults::_WeldResults->Get(WeldResults::PARALIST::PRE_HEIGHT, &m_stHeartbeat.PreHeight);
	m_stHeartbeat.RecipeNumber = WeldResults::_WeldResults->RecipeID;
	m_stHeartbeat.TotalEnergy = WeldResults::_WeldResults->Energy;
	WeldResults::_WeldResults->Get(WeldResults::PARALIST::TRIGGER_PRESSURE, &m_stHeartbeat.TriggerPress);
	WeldResults::_WeldResults->Get(WeldResults::PARALIST::WELD_PRESSURE, &m_stHeartbeat.WeldPress);
	m_stHeartbeat.WeldTime = WeldResults::_WeldResults->WeldTime;
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
	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_DEFAULT_RECIPE_IDX;
	sendMsg.msgLen = sizeof(WeldRecipeSCForUI);
	sendMsg.rspCode = 0;
	RecipeWrapper::Get(Recipe::RecipeSC.get(), sendMsg.Buffer);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
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
	RecipeWrapper::Get(Recipe::RecipeSC.get(), sendMsg.Buffer);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	//Recipe::RecipeSC = nullptr;
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
* \brief   - Sends response to UIC for Weld Result library request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseWeldResultLib()
{	
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_WELD_RESULT_HISTORY_LOG_IDX;
	sendMsg.msgLen = WeldResult::TransformResultsVector().size() * sizeof(WeldResultsUI);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &WeldResult::TransformResultsVector()[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	WeldResult::WeldResultVector.clear();
	WeldResult::TransformResultsVector().clear();
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for Alarm Log library request. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseAlarmLogLib()
{	
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_ALARM_LOG_IDX;
	auto AlarmLogUIVector = AlarmLog::TransformAlarmLogVector();
	sendMsg.msgLen = AlarmLogUIVector->size() * sizeof(UI_ALARM_LOG);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &AlarmLogUIVector->front(), sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	AlarmLog::AlarmVector.clear();
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

	sendMsg.msgID	= REQ_SYSTEM_CONGIF_READ_IDX;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;

	SYSTEMCONFIGFORUI sysConfigUI;
	SystemConfiguration::Get(&sysConfigUI);
	getSystemTime(sysConfigUI.DateTime);
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
	setSystemTime(sysConfigUI.DateTime);
}

/**************************************************************************//**
 * \brief   - Excute usb detect request
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void UserInterface::responseDetectUSBdevice()
{
	int iUSBdetectRes = USB_DETECT_FAIL;
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	
	iUSBdetectRes = FirmwareUpgrade::USBDetect();
	sendMsg.msgID	= REQ_DETECT_SC_USB_DEVICE;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &iUSBdetectRes, sizeof(int));
	sendMsg.msgLen = sizeof(int);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* \brief   - Response to UI Firware upgrade progress.  
*
* \param   - Message buffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseFirmwareUpgradeProgress(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= REQ_EXCUTE_FIRMWARE_UPGRADE_FROM_USB;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, messagebuf, sizeof(int));
	sendMsg.msgLen = sizeof(int);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
 * \brief   - Create Message Queue and spaw Firmwrae Upgrade task
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void UserInterface::firmWareUpgradeHandler() 
{
	/* Create message Queue */
	MSG_Q_ID 		FW_TID;
	MSG_Q_ID 		FW_MSG_Q_ID;
	FW_MSG_Q_ID = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
	if(MSG_Q_ID_NULL != FW_MSG_Q_ID) 
	{
		
		CP->setMsgQId(CommonProperty::cTaskName[CommonProperty::FWUPGRADE_T], FW_MSG_Q_ID);
	}
	else
	{
		LOGERR("HMISocket_T: FirmWareUpgradeHandler: FW upgrade message queue failed",0,0,0);
	}

	//Spawn Firmware Task
	FW_TID = taskSpawn((char *)CommonProperty::cTaskName[CommonProperty::FWUPGRADE_T],FW_UPGRADE_T_PRIORITY, 0, FW_UPGRADE_T_STACK_SIZE,(FUNCPTR)FirmwareUpgrade::FW_Upgrade_Task, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if(TASK_ID_NULL != FW_TID) 
	{
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::FWUPGRADE_T], FW_TID);
		bIsFWTask = true;
	}
	else
	{
		LOGERR("HMISocket_T: FirmWareUpgradeHandler: FW upgrade task spawn failed",0,0,0);
	}
}

/**************************************************************************//**
 * \brief   - Excute firmware upgrade request
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void UserInterface::responseFirmwareUPgradeHandler()
{
	int iExcuteFirmwareStatus = 1;
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	
	if(!bIsFWTask)
	{
		firmWareUpgradeHandler();
	}
	
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID	= REQ_EXCUTE_FIRMWARE_UPGRADE_FROM_USB;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &iExcuteFirmwareStatus, sizeof(int));
	sendMsg.msgLen = sizeof(int);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	bIsFWTask = false;
}

/**************************************************************************//**
* 
* \brief   - Write Configure for the External Ethernet
*
* \param   - Message buffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::setEthernetConfigData(char* messagebuf)
{
	memcpy(&Connectivity::EthernetConfig, messagebuf, sizeof(ETHERNET));
}


/**************************************************************************//**
* 
* \brief   - Response the Current Configuration of Ethernet to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseEthernetConfigData()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_ETHERNET_CONFIG_IDX;
	sendMsg.msgLen = sizeof(ETHERNET);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &Connectivity::EthernetConfig, sizeof(ETHERNET));
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the list of possible DIG Server to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseGatewayServerData()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_GATEWAY_SERVER_IDX;
	sendMsg.msgLen = Connectivity::_DIGMachinesUI->size()*sizeof(GATEWAY_MACHINE);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &Connectivity::_DIGMachinesUI->front(), Connectivity::_DIGMachinesUI->size()*sizeof(GATEWAY_MACHINE));
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the Weld Recipe Total Number to HMI
*
* \param   - Total Number
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseWeldRecipeTotalNumber(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_RECIPE_TOTAL_NUMBER_IDX;
	sendMsg.msgLen = sizeof(int);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, messagebuf, sizeof(int));
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the power graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadPowerGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_POWER_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_POWER);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the height graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadHeightGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_HEIGHT_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_HEIGHT);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the frquency graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadFrquencyGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_FREQUENCY_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_FRQUENCY);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response UIController request for the current alarm events getting.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseCurrentAlarmEventRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	AlarmManager::GetInstance()->SyncUIAlarmList();
	sendMsg.msgID = REQ_GET_CURRENT_ALARM_IDX;
	sendMsg.msgLen = AlarmManager::_AlarmListUI->size() * sizeof(UI_ALARM_LOG);
	sendMsg.rspCode = 0;
	memcpy(sendMsg.Buffer, &AlarmManager::_AlarmListUI->front(), sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response UIController request for the current alarm events reset.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseCurrentAlarmEventReset()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	AlarmManager::GetInstance()->ClearAlarmList();
	sendMsg.msgID = REQ_RESET_CURRENT_ALARM_IDX;
	sendMsg.msgLen = 0;
	sendMsg.rspCode = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response get the permission of screen
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responsePermissionScreenGetRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_PERMISSION_SCREEN;
	sendMsg.rspCode = 0;
	UserAuthority::GetInstance()->UpdateUserPrivileges(UserAuthority::GetInstance()->_UserPrivilegesSC);
	sendMsg.msgLen = UserAuthority::GetInstance()->_UserPrivilegesUI->size() * sizeof(USER_PRIVILEGE);
	memcpy(sendMsg.Buffer, &UserAuthority::GetInstance()->_UserPrivilegesUI->front(), sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - set screen permission
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::setScreenPermission(char* messagebuf)
{	
	int i = 0;
	//1. Update _UserPrivilegesUI.
	UserAuthority::GetInstance()->_UserPrivilegesUI->clear();
	for(int i = 0; i < UserAuthority::GetInstance()->_UserPrivilegesSC->size(); i++)
	{
		USER_PRIVILEGE  tmpPrivilege;
		memcpy(&tmpPrivilege, messagebuf + (i *  sizeof(USER_PRIVILEGE)), sizeof(USER_PRIVILEGE));
		UserAuthority::GetInstance()->_UserPrivilegesUI->push_back(tmpPrivilege);
	}
	UserAuthority::GetInstance()->UpdateUserPrivileges(UserAuthority::GetInstance()->_UserPrivilegesUI);
}

/**************************************************************************//**
* 
* \brief   - Response to get the password list
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responsePasscodeListGetRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_GET_PASSCODE_LIST;
	sendMsg.rspCode = 0;
	UserAuthority::GetInstance()->UpdateUserProfiles(UserAuthority::GetInstance()->_UserProfilesSC);
	sendMsg.msgLen = UserAuthority::GetInstance()->_UserProfilesUI->size() * sizeof(USER_PROFILE);
	memcpy(sendMsg.Buffer, &UserAuthority::GetInstance()->_UserProfilesUI->front(), sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - excute update user passcode
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::updateUserPasscode(char* messagebuf)
{
	//1. Update _UserProfilesSC
	UserAuthority::GetInstance()->_UserProfilesUI->clear();
	for(int i = 0; i < UserAuthority::GetInstance()->_UserProfilesSC->size(); i++)
	{
		USER_PROFILE tmpUser;
		memcpy(&tmpUser, messagebuf + (i *  sizeof(USER_PROFILE)), sizeof(USER_PROFILE));
		UserAuthority::GetInstance()->_UserProfilesUI->push_back(tmpUser);
	}
	UserAuthority::GetInstance()->UpdateUserProfiles(UserAuthority::GetInstance()->_UserProfilesUI);
}

/**************************************************************************//**
* 
* \brief   - Response to check password request
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseCheckPasscodeRequest(char* messagebuf)
{
	int iCheckStatus = PASSWORD_CHECK_FAIL;
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_VALIDATE_PASSCODE;
	sendMsg.rspCode = 0;
	sendMsg.msgLen = sizeof(iCheckStatus);
	USER_PAGE_PASSWORD userPagePasswordTmp;
	memcpy(&userPagePasswordTmp, messagebuf, sizeof(USER_PAGE_PASSWORD));
	iCheckStatus = UserAuthority::GetInstance()->CheckScreenAccessAuthority(userPagePasswordTmp.Password, userPagePasswordTmp.PageIndex);
	memcpy(sendMsg.Buffer, &iCheckStatus, sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);	
}

/**************************************************************************//**
* 
* \brief   - Response to set screen permission request
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::responsePermissionScreenSetRequest(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	sendMsg.msgLen = sizeof(int);
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	memcpy(sendMsg.Buffer, messagebuf, sizeof(int));
	sendMsg.msgID = REQ_SET_PERMISSION_SCREEN;
	sendMsg.rspCode = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response to update password request
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseUpdatePasscodeRequest(char* messagebuf)
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	sendMsg.msgLen = sizeof(int);
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	memcpy(sendMsg.Buffer, messagebuf, sizeof(int));
	sendMsg.msgID = REQ_UPDATE_PASSCODE;
	sendMsg.rspCode = 0;
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	
}

/**************************************************************************//**
* 
* \brief   - Get machine ip address
*
* \param   - char* ipbuffer
*
* \return  - None
*
******************************************************************************/
void UserInterface::getIPaddress(char* ipbuffer)
{

    int iSocketId;
    sockaddr_in stuSin;
    ifreq stuIfr;
    string strEthName = "cpsw0";
    iSocketId = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSocketId >= 0)
    {
    	memcpy(stuIfr.ifr_name, strEthName.c_str(), strEthName.size());
    	if(ioctl(iSocketId, SIOCGIFADDR, &stuIfr) >= 0)
    	{
    		 memcpy(&stuSin, &stuIfr.ifr_addr, sizeof(sockaddr_in));
    		 memcpy(ipbuffer, inet_ntoa(stuSin.sin_addr), 16);
    	}
    }
    close(iSocketId);
}

/**************************************************************************//**
* 
* \brief   - Get history power data from WeldResult table by ID
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::getWeldReslutHistoryPowerDataByID(char* messagebuf)
{
	MESSAGE message;
	message.msgID = DataTask::TO_DATA_TASK_READ_WELDRESULTHISTORY_POWER_DATA;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	memcpy(message.Buffer, messagebuf, sizeof(int));
	SendToMsgQ(message, DATA_MSG_Q_ID_DATA);	
}

/**************************************************************************//**
* 
* \brief   - Get history height data from WeldResult table by ID
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::getWeldReslutHistoryHeightDataByID(char* messagebuf)
{
	MESSAGE message;
	message.msgID = DataTask::TO_DATA_TASK_READ_WELDRESULTHISTORY_HEIGHT_DATA;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	memcpy(message.Buffer, messagebuf, sizeof(int));
	SendToMsgQ(message, DATA_MSG_Q_ID_DATA);	
}

/**************************************************************************//**
* 
* \brief   - Get history frequency data from WeldResult table by ID
*
* \param   - char* messagebuf
*
* \return  - None
*
******************************************************************************/
void UserInterface::getWeldReslutHistoryFrequencyDataByID(char* messagebuf) 
{
	MESSAGE message;
	message.msgID = DataTask::TO_DATA_TASK_READ_WELDRESULTHISTORY_FREQUENCY_DATA;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	memcpy(message.Buffer, messagebuf, sizeof(int));
	SendToMsgQ(message, DATA_MSG_Q_ID_DATA);	
}

/**************************************************************************//**
* 
* \brief   - Response the history power graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadHistoryPowerGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_WELDRESULTHISTORY_POWER_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_POWER);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the history height graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadHistoryHeightGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_WELDRESULTHISTORY_HEIGHT_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_HEIGHT);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}

/**************************************************************************//**
* 
* \brief   - Response the history frquency graph points to HMI
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::responseReadHistoryFrquencyGraphRequest()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	sendMsg.msgID = REQ_READ_WELDRESULTHISTORY_FREQUENCY_GRAPH;
	sendMsg.rspCode = 0;
	WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_FRQUENCY);
	sendMsg.msgLen = WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
	memcpy(sendMsg.Buffer, &WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector[0], sendMsg.msgLen);
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
}



