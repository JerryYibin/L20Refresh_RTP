/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/
#include "ACState.h"
#include "ACStateMachine.h"
#include "HeightEncoder.h"
/**************************************************************************//**
* \brief  - Jump to the next state.
*
* \param  - NextState.
*			
* \return  - none.
*
******************************************************************************/
void ACState::ChangeState(ACSTATES nextState)
{
	ACStateMachine::ChangeState(nextState);
}

