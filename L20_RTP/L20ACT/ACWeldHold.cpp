/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACWeldHold.h"
#include "../ActuatorTask.h"
#include "../SCStateMachine/SCState.h"
#include "../ACStateMachine.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACWeldHold::ACWeldHold() {
	stateType = ACState::AC_HOLD;

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
ACWeldHold::~ACWeldHold() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT weld hold.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACWeldHold::Enter()
{
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_MOVE_DISABLE);
	ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_AC_MOVE_DISABLE);
}

/**************************************************************************//**
* 
* \brief   - ACT weld hold.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACWeldHold::Loop()
{
	if((ACStateMachine::AC_RX->MasterState == SCState::RETURN_STROKE) || 
			(ACStateMachine::AC_RX->MasterState == SCState::WAIT_FOR_READY_POSITION))
		ChangeState(AC_RETURN_STROKE);
	else if(ACStateMachine::AC_RX->MasterState == SCState::PRE_READY)
		ChangeState(AC_READY);
	else
	{
		// do more something here...
	}
}

/**************************************************************************//**
* 
* \brief   - ACT weld hold.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACWeldHold::Exit()
{
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(CTRL_AC_MOVE_DISABLE);
}

