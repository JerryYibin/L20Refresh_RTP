/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Wait for Ready Position State is the combination state for the 
 Horn up (AC State Machine) and After burst (PC State Machine).
 If the After burst time is more than zero, the after burst feature will be enabled. 
***************************************************************************/

#include "WaitForReadyPosition.h"
#include "SCStateMachine.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
#include "../SystemConfiguration.h"
#include "../Recipe.h"
#include "Utils.h"
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
	bool isLockAlarm = false;
	m_iABDuration = 0;
	m_iABTimeDelay = 0;
	m_iABAmplitude = 0;
	if(SCStateMachine::getInstance()->GetCoreState() != 0)
	{
		SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::LOCK_ON_ALARM, &isLockAlarm);
		if(isLockAlarm == false)
		{
			// Set AC Master State as WAITFORREAYPOSITION.
			ACStateMachine::AC_RX->MasterState = SCState::WAIT_FOR_READY_POSITION;
		}
		m_Actions = SCState::FAIL;
	}
	else
	{
		// Set AC Master State as WAITFORREAYPOSITION.
		ACStateMachine::AC_RX->MasterState = SCState::WAIT_FOR_READY_POSITION;
	}
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_ULS_ENABLE);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AFTER_BURST_TIME, &m_iABDuration);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AFTER_BURST_DELAY, &m_iABTimeDelay);
	//TODO it is the option for AfterBurst in case of customer need it.
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AFTER_BURST_AMPLITUDE, &m_iABAmplitude);
	if(m_iABDuration > 0)
		PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	m_Timeout = 0;
	m_TimeDelay = 0;
	// Set PC Master State as WAITFORREAYPOSITION.
	PCStateMachine::PC_RX->MasterState = SCState::WAIT_FOR_READY_POSITION;
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_AFTERBURST_ENABLE);
}

/**************************************************************************//**
*
* \brief   - Wait For Ready Position Loop. 
*            The State will go to next state until AC state goes to AC_READY. 
*            AC state machine for horn up and PC state machine for after burst should be parallel.
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
		m_Actions = SCState::FAIL;
	}
	
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		m_Actions = SCState::FAIL;
	}
	
	switch(PCStateMachine::PC_TX->PCState)
	{
	case PCState::PC_READY:
		//If After Burst is not equal to 0, it means the after burst is enabled. 
		if((m_iABDuration > 0) && (m_Timeout == 0))
		{
			//To make sure the horn has get away from the part before the after burst.
			if(((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE)) == 0) && (m_TimeDelay == 0))
			{
				//There is another setting for the after burst named After burst time delay.
				if(m_TimeDelay >= m_iABTimeDelay)
				{
					//If the time delay is reached out the setting, to set sonics on for after burst.
					PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(PCState::CTRL_AFTERBURST_ENABLE);
					LOG("AB Time Delay = %d\n", m_TimeDelay);
				}
				else
				{
					m_TimeDelay++;
				}
			}
			else if(m_TimeDelay > 0)
			{
				if(m_TimeDelay >= m_iABTimeDelay)
				{
					//If the time delay is reached out the setting, to set sonics on for after burst.
					PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(PCState::CTRL_AFTERBURST_ENABLE);
					LOG("AB Time Delay = %d\n", m_TimeDelay);
				}
				else
				{
					m_TimeDelay++;
				}
			}
		}
		else
		{
			//If after burst is not enabled, PC State should be always in the PC_READY for waiting for horn up only. 
			if(ACStateMachine::AC_TX->ACState == ACState::AC_READY)
				m_Actions = SCState::JUMP;
		}
		break;
	case PCState::PC_WELD_RUN:
		// If the PC State is PC_WELD_RUN, it means the PC is running after burst process.
		if(m_Timeout >= m_iABDuration)
			PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_AFTERBURST_ENABLE);
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
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
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
