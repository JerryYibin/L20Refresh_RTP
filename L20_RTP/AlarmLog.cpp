/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#include "AlarmLog.h"

AlarmLogDefine* AlarmLog::_ptrAlarm = AlarmLogDefine::GetAlarmLog().get();

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AlarmLog::AlarmLog(){
	
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
AlarmLog::~AlarmLog(){
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Set UI Alarm log to SC Weld Alarm log
*
* \param   - HMI_WELD_SIGNATURE.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int AlarmLog::Set(const UI_ALARM_LOG* _sysAlarmLog)
{
	if(_sysAlarmLog == nullptr)
		return ERROR;
	
	_ptrAlarm->Set(AlarmLogDefine::PARALIST::WELD_COUNT, 	(void*)(&_sysAlarmLog->WeldCount));
	_ptrAlarm->Set(AlarmLogDefine::PARALIST::ALARM_TYPE, 	(void*)(&_sysAlarmLog->AlarmType));
	_ptrAlarm->Set(AlarmLogDefine::PARALIST::DATE_TIME, 	(void*)(_sysAlarmLog->DateTime));
	_ptrAlarm->Set(AlarmLogDefine::PARALIST::RECIPE_NAME, 	(void*)(_sysAlarmLog->RecipeName));
	
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get SC Alarm log to UI Weld Alarm log
*
* \param   - HMI_WELD_SIGNATURE.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int AlarmLog::Get(UI_ALARM_LOG* _sysAlarmLog)
{
	if(_sysAlarmLog == nullptr)
		return ERROR;
	
	_ptrAlarm->Get(AlarmLogDefine::PARALIST::WELD_COUNT, 	(void*)(&_sysAlarmLog->WeldCount));
	_ptrAlarm->Get(AlarmLogDefine::PARALIST::ALARM_TYPE, 	(void*)(&_sysAlarmLog->AlarmType));
	_ptrAlarm->Get(AlarmLogDefine::PARALIST::DATE_TIME, 	(void*)(_sysAlarmLog->DateTime));
	_ptrAlarm->Get(AlarmLogDefine::PARALIST::RECIPE_NAME, 	(void*)(_sysAlarmLog->RecipeName));
	
	return OK;
}
