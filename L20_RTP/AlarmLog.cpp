/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#include "AlarmLog.h"

std::shared_ptr<AlarmLogDefine> AlarmLog::_ptrAlarm = AlarmLogDefine::GetAlarmLog();
std::vector<shared_ptr<AlarmLogDefine>> AlarmLog::AlarmVector;

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
* \brief   - Convert the Vector to a Vector available to the UI
*
* \param   - Nullptr
*
* \return  - vector<WeldResultsUI> - The vector that is sent to the UI
*
******************************************************************************/
unique_ptr<vector<UI_ALARM_LOG>> AlarmLog::TransformAlarmLogVector()
{
	auto ptr_Vector = std::make_unique<std::vector<UI_ALARM_LOG>>();
	UI_ALARM_LOG _sysAlarmLog;
	for(auto& i : AlarmLog::AlarmVector)
	{
		i->Get(AlarmLogDefine::PARALIST::WELD_COUNT, &_sysAlarmLog.WeldCount);
		i->Get(AlarmLogDefine::PARALIST::ALARM_TYPE, &_sysAlarmLog.AlarmType);
		i->Get(AlarmLogDefine::PARALIST::RECIPE_NAME, _sysAlarmLog.RecipeName);
		i->Get(AlarmLogDefine::PARALIST::DATE_TIME, _sysAlarmLog.DateTime);
		
		ptr_Vector->push_back(_sysAlarmLog);
	}
	return ptr_Vector;
}
