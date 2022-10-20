/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Wait for Ready Position State
***************************************************************************/

#include "WaitForReadyPosition.h"
#include "../ACStateMachine.h"

/**************************************************************************//**
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
WaitForReadyPosition::WaitForReadyPosition() {
	m_Actions = SCState::INIT;
	m_State = SCState::WAIT_FOR_READY_POSITION;
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
WaitForReadyPosition::~WaitForReadyPosition() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Wait For Ready Position Enter. Set AC Master State as WAIT_FOR_READY_POSITION.
*            Reset BIT CTRL_AC_MOCE_DISABLE of MasterEvents
*            If Height Encoder option is disabled, reset CTRL_HOME_POSITION_ENABLE of Master Events. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Enter()
{
	ACStateMachine::AC_RX->MasterState = SCState::WAIT_FOR_READY_POSITION;
	ACStateMachine::AC_RX->MasterEvents &= ~ BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);

//	if (SysConfig.m_SystemInfo.HeightEncoder == false)
	{
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_HOME_POSITION_ENABLE);
	}
//	else
//	{
//		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_READY_CHECK_ENABLE);
//		ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_READY_CHECK_ENABLE);
//	}
	

}

/**************************************************************************//**
*
* \brief   - Wait For Ready Position Loop. 
*            The State will go to next state until AC state goes to AC_READY. 
*            
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		//TODO Record Database alarm table
		CommonProperty::WeldResult.ALARMS.AlarmFlags.HeightSystemFailure = 1;
		m_Actions = SCState::FAIL;
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_READY)
		m_Actions = SCState::JUMP;
}

/**************************************************************************//**
*
* \brief   - Wait For Ready Position Exit.
*            Reset MasterState to NO_STATE.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Exit()
{
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Wait For Ready Position Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Release Part Alarm happened!\n");
}
