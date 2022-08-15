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
* \brief   - Wait For Trigger Enter.
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
* \brief   - Wait For Trigger Loop.
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
	else if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_PART_CONTACT_FOUND)) == BIT_MASK(ACState::STATUS_PART_CONTACT_FOUND))
		m_Actions = SCState::JUMP;

}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Exit()
{
	
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

