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
#include "../ScDgtOutput.h"

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
* \brief   - Ready Enter. Set READY as the current Master State of AC.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Enter()
{
	ACStateMachine::AC_RX->MasterState = SCState::READY;
}

/**************************************************************************//**
* 
* \brief   - Ready Loop. Waiting for both start switches has been already pressed. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_READY)
	{
		if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED)) == BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED))
			m_Actions = SCState::JUMP;
	}
	else if((ACStateMachine::AC_TX->ACState == ACState::AC_ALARM) || (PCStateMachine::PC_TX->PCState == PCState::PC_ALARM))
	{
		m_Actions = SCState::FAIL;
	}
}

/**************************************************************************//**
* 
* \brief   - Ready Exit. To reset Ready signal for external User I/O.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Ready::Exit()
{
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_READY_RESET);
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
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
