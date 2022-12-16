/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Database task for DB related read or write operations  
     
Changed the C code "Data_Task" to c++ static function. 
It's a good change for Data_Task so Data_Task can access all the source of
DataTask class owned using the class object pointer. 
 
**********************************************************************************************************/
#undef PERFORMANCE_MEASURE
#ifdef PERFORMANCE_MEASURE
#include <timerDev.h>
#endif
#include "DataTask.h"
#include "Database/DBAccess_l20_db.h"
#include "UserInterface.h"
#include "SystemConfiguration.h"
#include "EEPROM.h"
#include "Utility.h"
extern "C"
{
	#include "customSystemCall.h"	
}

DBAccess* DataTask::_ObjDBConn = nullptr;

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of UserInterface.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
DataTask::DataTask()
{
	ConnectDB();
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	SELF_MSG_Q_ID_CTRL = CP->getMsgQId(Data_Task + "/Control");
	SELF_MSG_Q_ID_DATA = CP->getMsgQId(Data_Task + "/Data");
	SELF_MSG_Q_ID_REQUEST = CP->getMsgQId(Data_Task + "/Request");
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	INTERFACE_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	

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
DataTask::~DataTask() 
{
	CloseDB();
}

/*************************************************************************//**
 * \brief   - Connect Database.
 *
 * \param   - None.
 *
 * \return  - None.
 *
 ******************************************************************************/
int DataTask::ConnectDB()
{
	int nErrCode = SQLITE_OK;
    if(_ObjDBConn == nullptr)
	{
    	switch(GetSystemType())
    	{
    	case L20:
    		_ObjDBConn = new DBAccessL20DB();
        	break;
		default:
			LOGERR((char* )"MODEL_T : --------NO System Detected-----------", 0, 0, 0);
			return SQLITE_ERROR;
    	}
    	nErrCode = _ObjDBConn->ConnectDB();
    	if(nErrCode != SQLITE_OK)
    		LOGERR("DB Connection Open Error! ErrCode = %d\n",nErrCode, 0, 0);
	}
	return nErrCode;
}

/*************************************************************************//**
 * \brief   - Close Database
 *
 * \param   - None.
 *
 * \return  - None.
 *
 ******************************************************************************/
int DataTask::CloseDB()
{
	int nErrCode = SQLITE_OK;
	if(_ObjDBConn != nullptr)
	{
		nErrCode = _ObjDBConn->CloseDataBaseConnection();
    	if(nErrCode > SQLITE_OK)
    		LOGERR("DB Close Error! ErrCode = %d\n",nErrCode, 0, 0);
        else
            _ObjDBConn = nullptr;
	}
	return nErrCode;
}


/**************************************************************************//**
* 
* \brief   - Used to send storage failure information for UI
*
* \param   - error code.
*
* \return  - None.
*
******************************************************************************/
void DataTask::sendErrorCode(int ErrCode)
{
	MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= UserInterface::TO_UI_RECIPE_ERRCODE;
	memcpy(sendMsg.Buffer, &ErrCode, sizeof(int));
	SendToMsgQ(sendMsg, UI_MSG_Q_ID);
}

/**************************************************************************//**
* 
* \brief   - Send Weld Recipe Total Number for UI
*
* \param   - Total Number.
*
* \return  - None.
*
******************************************************************************/
void DataTask::sendTotalNumber(int Number)
{
	MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));

	sendMsg.msgID	= UserInterface::TO_UI_TASK_RECIPE_TOTAL_NUMBER_RESPONSE;
	memcpy(sendMsg.Buffer, &Number, sizeof(int));
	SendToMsgQ(sendMsg, UI_MSG_Q_ID);
}

/*************************************************************************//**
 * \brief   - Process the received message from DATA_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::ProcessTaskMessage(MESSAGE& message)
{
	int ErrCode = 0;
	int TotalNum = 0;
    if((_ObjDBConn == nullptr)&&(message.msgID != TO_DATA_TASK_OPEN_DB))
    {
    	LOGERR((char* )"DataTask: --------Database has not been open, Message ID = --- : ", message.msgID, 0, 0);
        return;
    }

	switch(message.msgID)
	{
	case TO_DATA_TASK_OPEN_DB:
	    ConnectDB();
        break;
	case TO_DATA_TASK_CLOSE_DB:
	    CloseDB();
        break;
	case TO_DATA_TASK_WELD_RECIPE_INSERT:
#ifdef PERFORMANCE_MEASURE
		{
		UINT32 m_startTime = sysTimestampLock();
#endif 
		ErrCode = _ObjDBConn->StoreWeldRecipe(message.Buffer);
		sendErrorCode(ErrCode);
#ifdef PERFORMANCE_MEASURE
		UINT32 m_endTime = sysTimestampLock();
        printf("single StoreWeldRecipe took %u microseconds\n",
            (m_endTime-m_startTime)*1000000/sysTimestampFreq());
        }
#endif
		break;
	case TO_DATA_TASK_WELD_RESULT_INSERT:
#ifdef PERFORMANCE_MEASURE
		{
		UINT32 m_startTime = sysTimestampLock();
#endif 
		_ObjDBConn->StoreWeldResult(message.Buffer);
#ifdef PERFORMANCE_MEASURE
		UINT32 m_endTime = sysTimestampLock();
        printf("single StoreWeldResult took %u microseconds\n",
            (m_endTime-m_startTime)*1000000/sysTimestampFreq());
        }
#endif
		break;
	case TO_DATA_TASK_WELD_SIGN_INSERT:
		ErrCode = _ObjDBConn->StoreWeldSignature(message.Buffer);
		break;
	case TO_DATA_TASK_ALARM_LOG_INSERT:
		_ObjDBConn->StoreAlarmLog(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_RECIPE_TOTAL_NUMBER:
		TotalNum = _ObjDBConn->QueryWeldRecipeTotalNumber(message.Buffer);
		sendTotalNumber(TotalNum);
		break;
	case TO_DATA_TASK_WELD_RECIPE_QUERY_ALL:
        _ObjDBConn->QueryWeldRecipeAll(message.Buffer);
        message.msgID = UserInterface::TO_UI_RECIPE_LIBRARY_DATA_RESPONSE;
        SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE:
		_ObjDBConn->QueryWeldRecipeLatestPage();
	    message.msgID = UserInterface::TO_UI_RECIPE_LIBRARY_DATA_RESPONSE;
	    SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE:
		_ObjDBConn->QueryWeldRecipeNextPage();
	    message.msgID = UserInterface::TO_UI_RECIPE_LIBRARY_DATA_RESPONSE;
	    SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case TO_DATA_TASK_WELD_RECIPE_QUERY:
        _ObjDBConn->QueryWeldRecipe(message.Buffer);
        message.msgID = UserInterface::TO_UI_TASK_WELD_RECIPE_QUERY;
        SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case TO_DATA_TASK_WELD_RECIPE_UPDATE:
		ErrCode = _ObjDBConn->UpdateWeldRecipe(message.Buffer);
        sendErrorCode(ErrCode);
		break;
	case TO_DATA_TASK_WELD_RESULT_QUERY:
        _ObjDBConn->QueryBlockWeldResult(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_SIGN_QUERY:
        _ObjDBConn->QueryWeldSignature(message.Buffer);
		break;
	case TO_DATA_TASK_ALARM_LOG_QUERY:
        _ObjDBConn->QueryBlockAlarmLog(message.Buffer);
        break;
	case TO_DATA_TASK_ALARM_LOG_QUERY_LATEST_PAGE:
		if (!_ObjDBConn->QueryAlarmLogLatestPage(message.Buffer)) {
			message.msgID = UserInterface::TO_UI_TASK_ALARM_LOG_LIBRARY_RESPONSE;
			SendToMsgQ(message, UI_MSG_Q_ID);
		}
		break;
	case TO_DATA_TASK_ALARM_LOG_QUERY_NEXT_PAGE:
		if (!_ObjDBConn->QueryAlarmLogNextPage(message.Buffer)) {
			message.msgID = UserInterface::TO_UI_TASK_ALARM_LOG_LIBRARY_RESPONSE;
			SendToMsgQ(message, UI_MSG_Q_ID);
		}
		break;
	case TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY:
		ErrCode = _ObjDBConn->QueryHeightCalibration(message.Buffer);
		sendErrorCode(ErrCode);
		break;
	case TO_DATA_TASK_DB_VERSION_QUERY:
        _ObjDBConn->QueryDbVersion(message.Buffer);
		break;
	case TO_DATA_TASK_USER_PROFILE_QUERY:
        _ObjDBConn->QueryBlockUserProfiles(message.Buffer);
		break;
	case TO_DATA_TASK_PRIVILEGE_CONFIG_QUERY:
        _ObjDBConn->QueryBlockPrivilegeConfigure(message.Buffer);
		break;
	case TO_DATA_TASK_PWR_SUPPLY_QUERY:
        _ObjDBConn->QueryBlockPowerSupply(message.Buffer);
		break;
	case TO_DATA_TASK_TEACH_MODE_SET_QUERY:
        _ObjDBConn->QueryBlockTeachModeSetting(message.Buffer);
		break;
	case TO_DATA_TASK_SYS_CONFIG_QUERY:
        _ObjDBConn->QuerySystemConfigure(message.Buffer);
		break;
	case TO_DATA_TASK_ACTIVE_RECIPE_QUERY:
        _ObjDBConn->QueryActiveRecipe(message.Buffer);
        message.msgID = UserInterface::TO_UI_TASK_SETUP_CLICKET_PARAM;
        SendToMsgQ(message, UI_MSG_Q_ID);
        message.msgID = UserInterface::TO_UI_TASK_ACTIVE_RECIPE_INFO_RESPONSE;
        SendToMsgQ(message, UI_MSG_Q_ID);
		break;
	case TO_DATA_TASK_CONNECTIVITY_QUERY:
        _ObjDBConn->QueryConnectivity(message.Buffer);
		break;
	case TO_DATA_TASK_GATEWAY_MACHINE_QUERY:
        _ObjDBConn->QueryGatewayMachine(message.Buffer);
		break;
	case TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE:
		ErrCode = _ObjDBConn->UpdateHeightCalibration(message.Buffer);
		sendErrorCode(ErrCode);
		break;
	case TO_DATA_TASK_USER_PROFILE_UPDATE:
        _ObjDBConn->UpdateUserProfiles(message.Buffer);
		break;
	case TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE:
        _ObjDBConn->UpdatePrivilegeConfigure(message.Buffer);
		break;
	case TO_DATA_TASK_PWR_SUPPLY_UPDATE:
        _ObjDBConn->UpdateBlockPowerSupply(message.Buffer);
		break;
	case TO_DATA_TASK_TEACH_MODE_SET_UPDATE:
        _ObjDBConn->UpdateBlockTeachModeSetting(message.Buffer);
		break;
	case TO_DATA_TASK_SYS_CONFIG_UPDATE:
	{
		SYSTEMCONFIG::POWER power = SYSTEMCONFIG::POWER_5500W; 
		SYSTEMCONFIG::FREQUENCY frequency = SYSTEMCONFIG::FREQ_20KHZ;
		UINT32 amplitude = 72;
		ErrCode = _ObjDBConn->UpdateSystemConfigure(message.Buffer);
		SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::POWER_OPT, &power);
		SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::FREQUENCY_OPT, &frequency);
		if((power != EEPROM::_System->Power) || (frequency != EEPROM::_System->Frequency))
		{
			EEPROM::_System->Power 		= power;
			EEPROM::_System->Frequency 	= frequency;
			EEPROM::GetInstance()->Write();
			if(power != EEPROM::_System->Power)
				Utility::SetSystemPower(power);
			if(frequency != EEPROM::_System->Frequency)
				Utility::SetSystemFrequency(frequency);
		}
		SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::MAX_AMPLITUDE, &amplitude);
		if(amplitude != Utility::GetSystemAmplitude())
			Utility::SetSystemAmplitude(amplitude);
		sendErrorCode(ErrCode);
		break;
	}
	case TO_DATA_TASK_ACTIVE_RECIPE_UPDATE:
        _ObjDBConn->UpdateActiveRecipe(message.Buffer);
		break;
	case TO_DATA_TASK_CONNECTIVITY_UPDATE:
        _ObjDBConn->UpdateConnectivity(message.Buffer);
		break;

	case TO_DATA_TASK_WELD_RECIPE_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_RECIPE);
		break;
	case TO_DATA_TASK_WELD_RESULT_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_RESULT);
		break;
	case TO_DATA_TASK_WELD_SIGN_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_SIGNATURE);
		break;
	case TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC:
		ErrCode = _ObjDBConn->DeleteSpecificRecipe(message.Buffer);
		sendErrorCode(ErrCode);
		break;
	case TO_DATA_TASK_WELD_RECIPE_CLEAR:
        _ObjDBConn->DeleteAllTableRows(TABLE_WELD_RECIPE);
		break;
	case TO_DATA_TASK_WELD_RESULT_CLEAR:
        _ObjDBConn->DeleteAllTableRows(TABLE_WELD_RESULT);
		break;
	case TO_DATA_TASK_WELD_SIGN_CLEAR:
        _ObjDBConn->DeleteAllTableRows(TABLE_WELD_SIGNATURE);
		break;
	case TO_DATA_TASK_ALARM_LOG_CLEAR:
        _ObjDBConn->DeleteAllTableRows(TABLE_ALARM_LOG);
		break;
	case TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE:
		if(!_ObjDBConn->QueryWeldResultLatestPage(message.Buffer)){
			message.msgID = UserInterface::TO_UI_TASK_RESULT_LIBRARY_DATA_RESPONSE;
			SendToMsgQ(message, UI_MSG_Q_ID);
		}
		break;
	case TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE:
		if(!_ObjDBConn->QueryWeldResultNextPage(message.Buffer)){
			message.msgID = UserInterface::TO_UI_TASK_RESULT_LIBRARY_DATA_RESPONSE;
			SendToMsgQ(message, UI_MSG_Q_ID);
		}
		break;
	case TO_DATA_TASK_WELD_RECIPE_RENAME:
		ErrCode = _ObjDBConn->RenameWeldRecipe(message.Buffer);
		sendErrorCode(ErrCode);
		break;

	case TO_DATA_TASK_WELD_RESULT_ID:
        _ObjDBConn->getLatestID(TABLE_WELD_RESULT, nullptr);
		break;
	case TO_DATA_TASK_WELD_SIGN_ID:
        _ObjDBConn->getLatestID(TABLE_WELD_SIGNATURE, nullptr);
		break;
	case TO_DATA_TASK_ALARM_LOG_ID:
        _ObjDBConn->getLatestID(TABLE_ALARM_LOG, nullptr);
		break;
	default:
		LOGERR((char* )"DataTask: --------Unknown Message ID----------- : %d", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Initialize the data required for startup.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int DataTask::InitData()
{
    if(_ObjDBConn == nullptr)
    {
    	LOGERR((char* )"DataTask: --------Init Data Error--- : ", 0, 0, 0);
        return ERROR;
    }
    if(_ObjDBConn->QueryDbVersion(nullptr) != OK)
    {
    	return ERROR;
    }
    if(_ObjDBConn->QueryBlockPowerSupply(nullptr) <= 0)
    {
    	return ERROR;
    }
    if(_ObjDBConn->QuerySystemConfigure(nullptr) != OK)
    {
    	return ERROR;
    }
    else
    {
    	EEPROM::GetInstance()->Read();
    	SystemConfiguration::_SystemConfig->Set(SYSTEMCONFIG::POWER_OPT, &EEPROM::_System->Power);
    	Utility::SetSystemPower(EEPROM::_System->Power);
    	SystemConfiguration::_SystemConfig->Set(SYSTEMCONFIG::FREQUENCY_OPT, &EEPROM::_System->Frequency);
    	UINT32 amplitude = 72;
    	SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::MAX_AMPLITUDE, &amplitude);
    	Utility::SetSystemAmplitude(amplitude);
    }
	if(_ObjDBConn->QueryBlockTeachModeSetting(nullptr) <= 0)
	{
		return ERROR;
	}
	if(_ObjDBConn->QueryHeightCalibration(nullptr) <= 0)
	{
		return ERROR;
	}
	if(_ObjDBConn->QueryBlockUserProfiles(nullptr) <= 0)
	{
		return ERROR;
	}
	//TODO need to do further testing
//	if(_ObjDBConn->QueryConnectivity(nullptr) != OK)
//	{
//		return ERROR;
//	}
//	if(_ObjDBConn->QueryGatewayMachine(nullptr) <= 0)
//	{
//		return ERROR;
//	}
    return OK;		
}

/**************************************************************************//**
* 
* \brief   - Processing the Database Data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void DataTask::Data_Task(void)
{
	MESSAGE ProcessBuffer;
	UINT32 events;
	bool isRunning = true;

	char MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = { 0x00 };

	DataTask *DBInit = new (nothrow) DataTask();
	if (NULL != DBInit)
	{
		if (!DBInit->InitData())
		{
			while (DBInit->bIsTaskRunStatus())
			{
				// wait for any one event
				if (eventReceive(DATA_TASK_EVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
				{
					isRunning = true;
					do {
						if (msgQReceive(DBInit->SELF_MSG_Q_ID_CTRL, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
						{
							DBInit->Decode(MsgQBuffer, ProcessBuffer);
							DBInit->ProcessTaskMessage(ProcessBuffer);
						}
						else if (msgQReceive(DBInit->SELF_MSG_Q_ID_DATA, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
						{
							DBInit->Decode(MsgQBuffer, ProcessBuffer);
							DBInit->ProcessTaskMessage(ProcessBuffer);
						}
						else if (msgQReceive(DBInit->SELF_MSG_Q_ID_REQUEST, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
						{
							DBInit->Decode(MsgQBuffer, ProcessBuffer);
							DBInit->ProcessTaskMessage(ProcessBuffer);
						}
						else
							isRunning = false;

					} while (isRunning);
				}
			}
		} else
			LOGERR((char* )"DATA_T : ----------------Init Data failed----------------", 0, 0, 0);//TODO Need to notify UI
		delete DBInit;
	} else {
		LOGERR((char* )"DATA_T : ----------------Memory allocation failed----------------", 0, 0, 0);
	}
	DBInit = NULL;
	taskSuspend(taskIdSelf());
}
