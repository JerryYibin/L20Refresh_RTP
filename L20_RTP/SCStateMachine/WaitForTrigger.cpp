/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Wait For Trigger State is the combination state for the 
 Horn down (AC State Machine) and pre-burst (PC State Machine).
 If the Pre-burst time is more than zero, the pre-burst feature will be enabled. 
***************************************************************************/

#include "WaitForTrigger.h"
#include "Utils.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
#include "../SystemConfiguration.h"
#include "../Recipe.h"

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
	m_bHeightEncoder = true;
	m_iPreburst = 0;
	ACStateMachine::AC_RX->MasterState = SCState::WAIT_FOR_TRIGGER;
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::HGT_ENCODER, &m_bHeightEncoder);
	if(m_bHeightEncoder == false)
	{
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_PART_CONTACT_ENABLE);
	}
	else
	{
		ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_PART_CONTACT_ENABLE);
	}
	m_Timeout = 0;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PRE_BURST, &m_iPreburst);
	if(m_iPreburst > 0)
		PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	PCStateMachine::PC_RX->MasterState = SCState::WAIT_FOR_TRIGGER;
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PREBURST_ENABLE);
}

/**************************************************************************//**
*
* \brief   - Wait For Trigger Loop. Entry to Next State until ACState changes to AC_HOLD.
* 			 AC state machine for horn down and PC state machine for pre-burst should be parallel.
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
		//CommonProperty::WeldResult.ALARMS.AlarmFlags.HeightSystemFailure = 1;
		int AlarmFlags = 1;
		WeldResults::_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, &AlarmFlags);
		m_Actions = SCState::FAIL;
	}
	
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		//TODO Record Database alarm table
		//CommonProperty::WeldResult.ALARMS.AlarmFlags.Overload = 1;
		int AlarmFlags = 1;
		WeldResults::_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, &AlarmFlags);
		m_Actions = SCState::FAIL;
	}
	
	switch(PCStateMachine::PC_TX->PCState)
	{
	case PCState::PC_READY:
		//If Pre-burst is not equal to 0, it means the pre-burst is enabled. 
		if((m_iPreburst > 0) && (m_Timeout == 0))
		{
			//To make sure the horn has been moving before the pre-burst.
			if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE)) == 0)
			{
				PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(PCState::CTRL_PREBURST_ENABLE);
			}
		}
		else
		{
			//If pre-burst is not enabled, PC State should be always in the PC_READY for waiting for horn down only. 
			if(ACStateMachine::AC_TX->ACState == ACState::AC_HOLD)
				m_Actions = SCState::JUMP;
		}
		break;
	case PCState::PC_WELD_RUN:
		// If the PC State is PC_WELD_RUN, it means the PC is running pre-burst process.
		if(m_Timeout >= m_iPreburst)
			PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PREBURST_ENABLE);
		else
			m_Timeout++;
		break;
	default:
		m_Actions = SCState::FAIL;
		break;
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
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PREBURST_ENABLE);
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

