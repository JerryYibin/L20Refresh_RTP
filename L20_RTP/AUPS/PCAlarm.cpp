/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCAlarm.h"
#include "../PCStateMachine.h"
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
	
	if (PowerSupplyTask::GetCoreState() & ERR_POWER_OVERLOAD)
		LOG("\nPower supply Core State: POWER_OVERLOAD\n");
	if (PowerSupplyTask::GetCoreState() & ERR_VOLTAGE_OVERLOAD)
		LOG("\nPower supply Core State: VOLTAGE_OVERLOAD\n");
	if (PowerSupplyTask::GetCoreState() & ERR_CURRENT_OVERLOAD)
		LOG("\nPower supply Core State: CURRENT_OVERLOAD\n");
	if (PowerSupplyTask::GetCoreState() & ERR_FREQ_OVERLOAD)
		LOG("\nPower supply Core State: FREQUENCY_OVERLOAD\n");
	if (PowerSupplyTask::GetCoreState() & ERR_TEMP_OVERLOAD)
		LOG("\nPower supply Core State: TEMPERATURE_OVERLOAD\n");
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
		vxbGpioSetValue(GPIO::O_OL_RST_PSI, GPIO_VALUE_HIGH);
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

