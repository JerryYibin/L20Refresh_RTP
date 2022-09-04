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
#define TIMEDELAY	1000
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
* \brief   - ReadyForTrigger Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Enter()
{
	ACStateMachine::AC_RX->MasterState = ACState::AC_READY;
	ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	m_Timeout = 0;
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Loop.
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
		if(m_Timeout > TIMEDELAY)
			m_Actions = SCState::JUMP;
		else
			m_Timeout++;
	}
}

/**************************************************************************//**
* 
* \brief   - ReadyForTrigger Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForTrigger::Exit()
{
	m_IsTrigger = false;
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
* \brief   - ReadyForTrigger Execute.
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
