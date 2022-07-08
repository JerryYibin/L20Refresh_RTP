/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/
#include "PCState.h"
#include "PCStateMachine.h"

/**************************************************************************//**
* \brief  - Jump to the next state.
*
* \param  - NextState.
*			
* \return  - none.
*
******************************************************************************/
void PCState::ChangeState(PCSTATES nextState) 
{
	PCStateMachine::ChangeState(nextState);
}


