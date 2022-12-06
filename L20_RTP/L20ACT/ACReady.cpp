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
* \brief   - ACT Ready. To set MasterEvents with CTRL_AC_MOVE_DISABLE
*            To set AC_StatusEvent with STATUS_AC_MOVE_DISABLE.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACReady::Enter()
{
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_MOVE_DISABLE);
	ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_AC_MOVE_DISABLE);
}

/**************************************************************************//**
* 
* \brief   - ACT Ready. In AC Ready Loop there are only three SC states 
*            need to handle with as following...
*            1. PRE_READY
*            2. READY
*            3. WAIT_FOR_TRIGGER
*            4. DOWN_STROKE
*            The normal loop should be AC READY-> DOWN_STROCK-> HOLD -> UP_STROCK
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACReady::Loop()
{
	if(ACStateMachine::AC_RX->MasterState == SCState::PRE_READY)
	{
		if((ACStateMachine::AC_TX->ACInputs & BOTHSTARTSWITCHMASK) == 0)
			ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED);
	}
	else if(ACStateMachine::AC_RX->MasterState == SCState::READY)
	{
		if(ActuatorTask::GetInstance()->GetPB1() == true)
			ChangeState(ACState::AC_STARTSWICH);
	}
	else if((ACStateMachine::AC_RX->MasterState == SCState::WAIT_FOR_TRIGGER) ||
			(ACStateMachine::AC_RX->MasterState == SCState::DOWN_STROKE))
	{
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(CTRL_AC_MOVE_DISABLE);
		ChangeState(AC_DOWN_STROKE);
	}
	else
	{
		
	}

}

/**************************************************************************//**
* 
* \brief   - ACT Exit.
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
	}
}

