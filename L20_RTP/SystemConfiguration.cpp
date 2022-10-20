/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#include "SystemConfiguration.h"
#include "commons.h"
SYSTEMCONFIG* SystemConfiguration::_SystemConfig = SYSTEMCONFIG::GetSystemConfig().get();
vector<POWER_SUPPLY_TYPE> SystemConfiguration::PowerSupplyType;
vector<TEACH_MODE_SETTING> SystemConfiguration::TeachModeSetting;

/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
SystemConfiguration::SystemConfiguration() {

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
SystemConfiguration::~SystemConfiguration() 
{
}

/**************************************************************************//**
* 
* \brief   - Set UI system configuration to SC system configuration 
*
* \param   - SYSTEMCONFIGFORUI.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int SystemConfiguration::Set(const SYSTEMCONFIGFORUI* _sysConfigUI)
{
	if(_sysConfigUI == nullptr)
		return ERROR;
	_SystemConfig->Set(SYSTEMCONFIG::LANGUAGE_OPT, 		(void*)(&_sysConfigUI->Language));
	_SystemConfig->Set(SYSTEMCONFIG::POWER_OPT, 		(void*)(&_sysConfigUI->PowerSupply));
	_SystemConfig->Set(SYSTEMCONFIG::FREQUENCY_OPT, 	(void*)(&_sysConfigUI->Frequency));
	_SystemConfig->Set(SYSTEMCONFIG::HGT_ENCODER, 		(void*)(&_sysConfigUI->bHeightEncoder));
	_SystemConfig->Set(SYSTEMCONFIG::FOOT_PEDAL_ABORT, 	(void*)(&_sysConfigUI->bFootPedalAbort));
	_SystemConfig->Set(SYSTEMCONFIG::LOCK_ON_ALARM, 	(void*)(&_sysConfigUI->bLockOnAlarm));
	
	_SystemConfig->Set(SYSTEMCONFIG::COOLING_OPTION, 	(void*)(&_sysConfigUI->Cooling));
	_SystemConfig->Set(SYSTEMCONFIG::COOLING_DURATION, 	(void*)(&_sysConfigUI->CoolingDuration));
	_SystemConfig->Set(SYSTEMCONFIG::COLLING_DELAY, 	(void*)(&_sysConfigUI->CoolingDelay));
	_SystemConfig->Set(SYSTEMCONFIG::AMP_UNIT_OPT, 		(void*)(&_sysConfigUI->Amplitude_Unit));
	_SystemConfig->Set(SYSTEMCONFIG::PRESSURE_UNIT_OPT, (void*)(&_sysConfigUI->Pressure_Unit));
	_SystemConfig->Set(SYSTEMCONFIG::HGT_UNIT_OPT, 		(void*)(&_sysConfigUI->Height_Unit));
	_SystemConfig->Set(SYSTEMCONFIG::MAX_AMPLITUDE,		(void*)(&_sysConfigUI->MaxAmplitude));
	
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_QTY, 		(void*)(&_sysConfigUI->TeachMode.Quantity));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_TYPE, 		(void*)(&_sysConfigUI->TeachMode.Teach_mode));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_TIME_PL, 	(void*)(&_sysConfigUI->TeachMode.Time_Upper));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_TIME_MS, 	(void*)(&_sysConfigUI->TeachMode.Time_Lower));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_POWER_PL, 	(void*)(&_sysConfigUI->TeachMode.Power_Upper));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_POWER_MS, 	(void*)(&_sysConfigUI->TeachMode.Power_Lower));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_PREHGT_PL, 	(void*)(&_sysConfigUI->TeachMode.PreHeight_Upper));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_PREHGT_MS, 	(void*)(&_sysConfigUI->TeachMode.PreHeight_Lower));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_HGT_PL, 		(void*)(&_sysConfigUI->TeachMode.PostHeight_Upper));
	_SystemConfig->Set(SYSTEMCONFIG::TEACH_HGT_MS, 		(void*)(&_sysConfigUI->TeachMode.PostHeight_Lower));
	
	_SystemConfig->Set(SYSTEMCONFIG::DATE_TIME, 		(void*)(_sysConfigUI->DateTime));
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get SC system configuration to UI system configuration
*
* \param   - SYSTEMCONFIGFORUI.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int SystemConfiguration::Get(SYSTEMCONFIGFORUI* _sysConfigUI)
{
	if(_sysConfigUI == nullptr)
		return ERROR;
	_SystemConfig->Get(SYSTEMCONFIG::LANGUAGE_OPT, 		(void*)(&_sysConfigUI->Language));
	_SystemConfig->Get(SYSTEMCONFIG::POWER_OPT, 		(void*)(&_sysConfigUI->PowerSupply));
	_SystemConfig->Get(SYSTEMCONFIG::FREQUENCY_OPT, 	(void*)(&_sysConfigUI->Frequency));
	_SystemConfig->Get(SYSTEMCONFIG::HGT_ENCODER, 		(void*)(&_sysConfigUI->bHeightEncoder));
	_SystemConfig->Get(SYSTEMCONFIG::FOOT_PEDAL_ABORT, 	(void*)(&_sysConfigUI->bFootPedalAbort));
	_SystemConfig->Get(SYSTEMCONFIG::LOCK_ON_ALARM, 	(void*)(&_sysConfigUI->bLockOnAlarm));
	
	_SystemConfig->Get(SYSTEMCONFIG::COOLING_OPTION, 	(void*)(&_sysConfigUI->Cooling));
	_SystemConfig->Get(SYSTEMCONFIG::COOLING_DURATION, 	(void*)(&_sysConfigUI->CoolingDuration));
	_SystemConfig->Get(SYSTEMCONFIG::COLLING_DELAY, 	(void*)(&_sysConfigUI->CoolingDelay));
	_SystemConfig->Get(SYSTEMCONFIG::AMP_UNIT_OPT, 		(void*)(&_sysConfigUI->Amplitude_Unit));
	_SystemConfig->Get(SYSTEMCONFIG::PRESSURE_UNIT_OPT, (void*)(&_sysConfigUI->Pressure_Unit));
	_SystemConfig->Get(SYSTEMCONFIG::HGT_UNIT_OPT, 		(void*)(&_sysConfigUI->Height_Unit));
	_SystemConfig->Get(SYSTEMCONFIG::MAX_AMPLITUDE,		(void*)(&_sysConfigUI->MaxAmplitude));
	
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_QTY, 		(void*)(&_sysConfigUI->TeachMode.Quantity));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_TYPE, 		(void*)(&_sysConfigUI->TeachMode.Teach_mode));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_TIME_PL, 	(void*)(&_sysConfigUI->TeachMode.Time_Upper));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_TIME_MS, 	(void*)(&_sysConfigUI->TeachMode.Time_Lower));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_POWER_PL, 	(void*)(&_sysConfigUI->TeachMode.Power_Upper));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_POWER_MS, 	(void*)(&_sysConfigUI->TeachMode.Power_Lower));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_PREHGT_PL, 	(void*)(&_sysConfigUI->TeachMode.PreHeight_Upper));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_PREHGT_MS, 	(void*)(&_sysConfigUI->TeachMode.PreHeight_Lower));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_HGT_PL, 		(void*)(&_sysConfigUI->TeachMode.PostHeight_Upper));
	_SystemConfig->Get(SYSTEMCONFIG::TEACH_HGT_MS, 		(void*)(&_sysConfigUI->TeachMode.PostHeight_Lower));
	
	_SystemConfig->Get(SYSTEMCONFIG::DATE_TIME, 		(void*)(_sysConfigUI->DateTime));
	
	return OK;
}
