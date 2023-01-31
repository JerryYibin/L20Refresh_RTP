/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC Seek Sonic State
***************************************************************************/

#include "SeekSonicOn.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
#include "../ACState.h"
#include "../PCState.h"

/**************************************************************************//**
*
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SeekSonicOn::SeekSonicOn(SEEK_TYPE Type) 
{
	m_Actions = SCState::INIT;
	m_State = SCState::SEEK_SONIC_ON;
	m_Timeout = 0;
	m_SeekTime = 0;
	m_SeekType = Type;
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
SeekSonicOn::~SeekSonicOn() {
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
*
* \brief   - SeekSonicOn Enter.
* 			Set current Master State as the SEEK_SONIC_ON and 
* 			Reset BIT CTRL_PC_SONIC_DISABLE of Master Events.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SeekSonicOn::Enter()
{
	PCStateMachine::PC_RX->MasterState = SCState::SEEK_SONIC_ON;
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_WELDTEST_ENABLE);
	m_Timeout = 0;
	if(m_SeekType == TEST_SEEK_ON)
	{
		PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(PCState::CTRL_WELDTEST_ENABLE);
		m_SeekTime = TEST_SEEK_ON_TIME;
	}
	else
	{
		PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_WELDTEST_ENABLE);
		m_SeekTime = WELD_SEEK_ON_TIME;
	}
}

/**************************************************************************//**
*
* \brief   - SeekSonicOn Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SeekSonicOn::Loop()
{
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{	
		m_Actions = SCState::FAIL;
		return;
	}
	
	if(CheckWeldAbort() == true)
	{
		m_Actions = SCState::FAIL;
		return;
	}
	
	if (m_Timeout < m_SeekTime)
	{
//		if (m_Timeout % 5 == 0)
//		{
//			if (DefeatWeldAbortHandler() == true)
//				m_Actions = SCState::ABORT;
//		}
		m_Timeout++;
	}
	else
	{
		//TODO need to consider if the timeout should be controlled by SC rather than PC
		PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
		m_Actions = SCState::JUMP;
		LOG("m_StepIndex: SeekSonicOn Loop Running! Timeout: %d\n", m_Timeout);
	}
}

/**************************************************************************//**
*
* \brief   - SeekSonicOn Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SeekSonicOn::Exit()
{
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - SeekSonicOn Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SeekSonicOn::Fail()
{
	ResetWeldAbort();
	m_Actions = SCState::ABORT;
	LOG("SeekSonicOn Alarm happened!\n");
}
