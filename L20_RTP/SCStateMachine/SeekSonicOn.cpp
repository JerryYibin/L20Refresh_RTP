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
SeekSonicOn::SeekSonicOn() {
	m_Actions = SCState::INIT;
	m_State = SCState::SEEK_SONIC_ON;
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
SeekSonicOn::~SeekSonicOn() {
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
*
* \brief   - SeekSonicOn Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SeekSonicOn::Enter()
{
	PCStateMachine::PC_RX->MasterState = SCState::SEEK_SONIC_ON;
	PCStateMachine::PC_RX->MasterEvents &= ~ BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	m_Timeout = 0;
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
	if (m_Timeout < DELAY25MSEC)
	{
		if (m_Timeout % 5 == 0)
		{
			if (DefeatWeldAbortHandler() == true)
				m_Actions = SCState::ABORT;
		}
		
		if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
		{	
			m_Actions = SCState::FAIL;
		}
		
		m_Timeout++;
	}
	else
	{
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
	m_Actions = SCState::ABORT;
	LOG("SeekSonicOn Alarm happened!\n");
}
