/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACEstop.h"
#include "../ACStateMachine.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACEstop::ACEstop() {
	stateType = ACState::AC_ESTOP;

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
ACEstop::~ACEstop() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT E-stop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACEstop::Enter()
{
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_MOVE_DISABLE);
}

/**************************************************************************//**
* 
* \brief   - ACT E-stop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACEstop::Loop()
{
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_ESTOP)) != BIT_MASK(CTRL_ESTOP))
	{
		// take actions here...
		ChangeState(AC_READY);
	}
	else
	{
		// do nothing while in E-stop
	}
}

/**************************************************************************//**
* 
* \brief   - ACT E-stop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACEstop::Exit()
{
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(CTRL_AC_MOVE_DISABLE);
}

