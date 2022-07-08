/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCAlarm.h"
#include "PCStateMachine.h"
#include "../PowerSupplyTask.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PCAlarm::PCAlarm() {
	stateType = PC_ALARM;
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
PCAlarm::~PCAlarm() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - AUPS Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCAlarm::Enter()
{
	// turn off sonics
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_LOW);
	PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(CTRL_PC_SONIC_DISABLE);
	PCStateMachine::PC_RX->TargetAmplitude = 0;
	
	if (PowerSupplyTask::GetCoreState() & BIT_MASK(PowerSupplyTask::POWER_OVERLOAD))
		LOG("\nAUPSCoreStat: POWER_OVERLOAD\n");
//	if (PowerSupplyTask::GetCoreState() & BIT_MASK(VOLTAGE_OVERLOAD))
//		LOG("\ndspCoreStat: VOLTAGE_OVERLOAD\n");
//	if (PowerSupplyTask::GetCoreState() & BIT_MASK(CURRENT_OVERLOAD))
//		LOG("\ndspCoreStat: CURRENT_OVERLOAD\n");
//	if (PowerSupplyTask::GetCoreState() & BIT_MASK(TEMPERATURE_OVERLOAD))
//		LOG("\ndspCoreStat: TEMPERATURE_OVERLOAD\n");
}

/**************************************************************************//**
* 
* \brief   - AUPS Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCAlarm::Loop()
{
	// wait for alarm clear from AUSP
	if (PowerSupplyTask::GetCoreState() == NO_ERROR)
	{		
		ChangeState(PC_READY);
		PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	}
}

/**************************************************************************//**
* 
* \brief   - AUPS Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCAlarm::Exit()
{
	// take actions here...
}

