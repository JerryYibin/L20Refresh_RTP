/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "ReadyForTrigger.h"
#include "../ACStateMachine.h"
#include "SCStateMachine.h"
#define DEBOUNCE_TIME	1000
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ReadyForTrigger::ReadyForTrigger() 
{
	m_Actions = SCState::INIT;
	m_State = SCState::READY_FOR_TRIGGER;
	m_IsTrigger = false;
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
ReadyForTrigger::~ReadyForTrigger() 
{
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Enter. Set READY_FOR_TRIGGER as the current Master State of AC.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Enter()
{
	ACStateMachine::AC_RX->MasterState = SCState::READY_FOR_TRIGGER;
	ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	m_Timeout = 0;
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Loop. With litter debounce time after the triggered.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		m_Actions = SCState::FAIL;
	}
	if(m_IsTrigger == true)
	{
		if(m_Timeout > DEBOUNCE_TIME)
			m_Actions = SCState::JUMP;
		else
			m_Timeout++;
	}
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Exit. To set current master state of AC as NO_STATE.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Exit()
{
	m_IsTrigger = false;
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Fail()
{
	m_Actions = SCState::ABORT;
	LOGERR((char *)"ReadyForTrigger State Alarm happened!\n", 0, 0, 0);
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Execute. To set m_IsTrigger is true.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int ReadyForTrigger::Execute(void* _obj)
{
	ReadyForTrigger* _ReadyForTrigger = (ReadyForTrigger*)_obj;
	_ReadyForTrigger->m_IsTrigger = true;
	return OK;
}
