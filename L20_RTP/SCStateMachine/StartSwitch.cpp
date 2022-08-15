/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Start Switch State
***************************************************************************/

#include "StartSwitch.h"
#include "../ACStateMachine.h"
/**************************************************************************//**
*
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
StartSwitch::StartSwitch() {
	m_Actions = SCState::INIT;
	m_State = SCState::START_SWITCH;
	m_Timeout = 0;
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
StartSwitch::~StartSwitch() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - StartSwitch Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Enter()
{
	ACStateMachine::AC_RX->MasterState = SCState::START_SWITCH;
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
}

/**************************************************************************//**
*
* \brief   - StartSwitch Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		//TODO Record Database alarm table
		m_Actions = SCState::FAIL;
		return;
	}
	
	if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED)) == BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED))
		m_Actions = SCState::JUMP;
}

/**************************************************************************//**
*
* \brief   - StartSwitch Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Exit()
{
	
}

/**************************************************************************//**
*
* \brief   - StartSwitch Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Fail()
{
	m_Actions = SCState::PUSH;
	LOG("Start Switch Alarm happened!\n");
}
