/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Wait For Trigger State
***************************************************************************/

#include "WaitForTrigger.h"
#include "Utils.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"

/**************************************************************************//**
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
WaitForTrigger::WaitForTrigger() {
	m_Actions = SCState::INIT;
	m_State = SCState::WAIT_FOR_TRIGGER;
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
WaitForTrigger::~WaitForTrigger() {
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Enter. Notify AC that the current SC state is WAIT_FOR_TRIGGER.
*            , CTRL_AC_MOVE_DISABLE and to set/reset CTRL_PART_CONTACK_ENABLE following Height Encoder option.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Enter()
{
	ACStateMachine::AC_RX->MasterState = SCState::WAIT_FOR_TRIGGER;
	ACStateMachine::AC_RX->MasterEvents &= ~ BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	
//	if (SysConfig.m_SystemInfo.HeightEncoder == false)
	{
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_PART_CONTACT_ENABLE);
	}
//	else
//	{

//	}
}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Loop. Entry to Next State until ACState changes to AC_HOLD.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		//TODO Record Database alarm table
		CommonProperty::WeldResult.ALARMS.AlarmFlags.HeightSystemFailure = 1;
		m_Actions = SCState::FAIL;
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_HOLD)
	{
		m_Actions = SCState::JUMP;
	}
}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Exit. Reset current AC MasterState to NO_STATE.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Exit()
{
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Trigger Alarm happened!\n");
}

