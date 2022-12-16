/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#include "WeldResults.h"


std::vector<shared_ptr<WeldResults>>				WeldResult::WeldResultVector;

/**************************************************************************//**
* \brief   - Convert the Vector to a Vector available to the UI
*
* \param   - Nullptr
*
* \return  - vector<WeldResultsUI> - The vector that is sent to the UI
*
******************************************************************************/
std::vector<WeldResultsUI> WeldResult::TransformResultsVector()
{
	std::vector<WeldResultsUI> WeldResultsUIVector;
	WeldResultsUI _sysWeldResult;
	for(auto& i : WeldResult::WeldResultVector)
	{
		strncpy(_sysWeldResult.RecipeName,i->RecipeName, USER_NAME_SIZE);
		strncpy(_sysWeldResult.DateTime,i->DateTime, WELD_TIME_SIZE);
		
    	_sysWeldResult.WeldMode 	= i->WeldMode;//WeldMode
    	_sysWeldResult.Energy 		= i->Energy;//WeldEnergy
		_sysWeldResult.Amplitude 	= i->Amplitude;//WeldAmplitude
		_sysWeldResult.WeldTime 	= i->WeldTime;//WeldTime
		_sysWeldResult.Power 		= i->PeakPower;//WeldPeakPower
		_sysWeldResult.CycleCounter = i->CycleCounter;//CycleCounter
		
		i->Get(WeldResults::PARALIST::ALARM_ID, 			&_sysWeldResult.AlarmID);			//AlarmID
		i->Get(WeldResults::PARALIST::ENERGY_SETTING, 		&_sysWeldResult.EnergySetting);		//EnergySetting
    	i->Get(WeldResults::PARALIST::AMPLITUDE_SETTING, 	&_sysWeldResult.AmplitudeSetting);	//AmplitudeSetting
    	i->Get(WeldResults::PARALIST::W_PRESSURE_SETTING, 	&_sysWeldResult.WPressureSetting);	//WPpressureSetting
    	i->Get(WeldResults::PARALIST::T_PRESSURE_SETTING, 	&_sysWeldResult.TPressureSetting);	//TPpressureSetting
    	i->Get(WeldResults::PARALIST::MAX_WELD_TIME, 		&_sysWeldResult.MaxWeldTime);		//MaxWeldTime
    	i->Get(WeldResults::PARALIST::MIN_WELD_TIME, 		&_sysWeldResult.MinWeldTime);		//MinWeldTime
    	i->Get(WeldResults::PARALIST::MAX_POWER, 			&_sysWeldResult.MaxPower);			//MaxPower
    	i->Get(WeldResults::PARALIST::MIN_POWER, 			&_sysWeldResult.MinPower);			//MinPower
    	i->Get(WeldResults::PARALIST::MAX_PRE_HEIGHT, 		&_sysWeldResult.MaxPreHeight);		//MaxPreHeight
    	i->Get(WeldResults::PARALIST::MIN_PRE_HEIGHT, 		&_sysWeldResult.MinPreHeight);		//MinPreHeight
    	i->Get(WeldResults::PARALIST::MAX_POST_HEIGHT, 		&_sysWeldResult.MaxPostHeight);		//MaxPostHeight
    	i->Get(WeldResults::PARALIST::MIN_POST_HEIGHT, 		&_sysWeldResult.MinPostHeight);		//MinPostHeight
		i->Get(WeldResults::PARALIST::TRIGGER_PRESSURE, 	&_sysWeldResult.TPressure);			//TriggerPressure
		i->Get(WeldResults::PARALIST::WELD_PRESSURE, 		&_sysWeldResult.WPressure);			//WeldPressure
		i->Get(WeldResults::PARALIST::PRE_HEIGHT, 			&_sysWeldResult.PreHeight);			//TriggerHeight
		i->Get(WeldResults::PARALIST::POST_HEIGHT, 			&_sysWeldResult.PostHeight);		//WeldHeight
		
		WeldResultsUIVector.push_back(_sysWeldResult);;
	}
	return WeldResultsUIVector;
}
