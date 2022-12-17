/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "ReadyForTrigger.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
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
	
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		m_Actions = SCState::FAIL;
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
	//The task switch speed is faster than state machine running
	_ReadyForTrigger->m_Actions = SCState::JUMP;

	return OK;
}
