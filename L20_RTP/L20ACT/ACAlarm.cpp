/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACAlarm.h"
#include "../ACStateMachine.h"
#include "../ActuatorTask.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACAlarm::ACAlarm() {
	stateType = ACState::AC_ALARM;

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
ACAlarm::~ACAlarm() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACAlarm::Enter()
{
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	if (ActuatorTask::GetCoreState() & ERR_PRESSURE_SET)
		LOG("\nACTCoreState: Pressure Set issue\n");
	if (ActuatorTask::GetCoreState() & ERR_HEIGHT_SYSTEM)
		LOG("\nACTCoreState: Height System issue\n");
	//TODO the hardware code will be replaced with the override function for the different system type.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
}

/**************************************************************************//**
* 
* \brief   - ACT Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACAlarm::Loop()
{
	//TODO need to consider which task can reset these alarms. 
	//wait for alarm clear from external
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_AC_CLEAR_ALARMS);
		ChangeState(ACState::AC_READY);
	}
}

/**************************************************************************//**
* 
* \brief   - ACT Alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACAlarm::Exit()
{
	// take actions here...
	//TODO the hardware code will be replaced with the override function for the different system type.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
}

