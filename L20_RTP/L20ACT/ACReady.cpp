/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACReady.h"
#include "../ACStateMachine.h"
#include "../ActuatorTask.h"
#include "../SCStateMachine/SCState.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACReady::ACReady() {
	stateType = ACState::AC_READY;

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
ACReady::~ACReady() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACReady::Enter()
{
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_MOVE_DISABLE);
	ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_AC_MOVE_DISABLE);
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_WELD_CYCLE_ENABLE)) == BIT_MASK(CTRL_WELD_CYCLE_ENABLE))
	{
		//TODO the hardware code will be replaced with the override function for the different system type.
		vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_HIGH);
	}
}

/**************************************************************************//**
* 
* \brief   - ACT Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACReady::Loop()
{
	if((ACStateMachine::AC_RX->MasterState == SCState::WAIT_FOR_TRIGGER) ||
			(ACStateMachine::AC_RX->MasterState == SCState::DOWN_STROKE))
	{
		ChangeState(AC_DOWN_STROKE);
	}
	else
	{
		
	}
}

/**************************************************************************//**
* 
* \brief   - ACT Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACReady::Exit()
{
	// check ACT status
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms
		ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_CLEAR_ALARMS);
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(CTRL_AC_MOVE_DISABLE);
	}
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_WELD_CYCLE_ENABLE)) == BIT_MASK(CTRL_WELD_CYCLE_ENABLE))
	{
		//TODO the hardware code will be replaced with the override function for the different system type.
		vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
	}
}

