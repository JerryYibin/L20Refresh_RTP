/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Ready state
***************************************************************************/

#include "Ready.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Ready::Ready() {
	m_Actions = SCState::INIT;
	m_State = SCState::READY;
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
Ready::~Ready() {
	m_Actions = SCState::INIT;
	m_State = NO_STATE;
}

/**************************************************************************//**
* 
* \brief   - Ready Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Enter()
{

}

/**************************************************************************//**
* 
* \brief   - Ready Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Loop()
{
	if((ACStateMachine::AC_TX->ACState == ACState::AC_ALARM) || (PCStateMachine::PC_TX->PCState == PCState::PC_ALARM))
		m_Actions = SCState::FAIL;
	else
		m_Actions = SCState::JUMP;
}

/**************************************************************************//**
* 
* \brief   - Ready Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Exit()
{
	
}

/**************************************************************************//**
* 
* \brief   - Ready Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Ready Alarm happened!\n");
}
