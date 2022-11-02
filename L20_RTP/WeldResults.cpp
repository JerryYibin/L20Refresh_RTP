/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#include "WeldResults.h"

WeldResults* WeldResult::_WeldResults =  WeldResults::GetWeldResults().get();

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResult::WeldResult(){
	
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
WeldResult::~WeldResult(){
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Get SC Weld Results  to UI Weld Results 
*
* \param   - HEARTBEAT.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int WeldResult::Get(HEARTBEAT* _sysHearBeat)
{
	if(_sysHearBeat == nullptr)
		return ERROR;
	
	_sysHearBeat->RecipeNumber = _WeldResults->RecipeNumber;
	_sysHearBeat->CycleCounter = _WeldResults->CycleCounter;
	_sysHearBeat->TotalEnergy = _WeldResults->CycleCounter;
	_sysHearBeat->WeldTime = _WeldResults->WeldTime;
	_sysHearBeat->PeakPower = _WeldResults->PeakPower;
	_sysHearBeat->Amplitude = _WeldResults->Amplitude;
	
	_WeldResults->Get(WeldResults::PARALIST::ALARM_FLAGS, 		(void*)(&_sysHearBeat->AlarmCode));
	_WeldResults->Get(WeldResults::PARALIST::TRIGGER_PRESSURE, 	(void*)(&_sysHearBeat->TriggerPress));
	_WeldResults->Get(WeldResults::PARALIST::WELD_PRESSURE, 	(void*)(&_sysHearBeat->WeldPress));
	_WeldResults->Get(WeldResults::PARALIST::PRE_HEIGHT, 		(void*)(&_sysHearBeat->PreHeight));
	_WeldResults->Get(WeldResults::PARALIST::POST_HEIGHT, 		(void*)(&_sysHearBeat->PostHeight));
	
	return OK;
}
