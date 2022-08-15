/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "ReadyForRequest.h"
#include "../ACStateMachine.h"
#include "SCStateMachine.h"

/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ReadyForRequest::ReadyForRequest() 
{
	m_Actions = SCState::INIT;
	m_State = SCState::READY_FOR_REQUEST;
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
ReadyForRequest::~ReadyForRequest() 
{
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
* 
* \brief   - ReadyForRequest Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForRequest::Enter()
{
	ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE);
	m_Timeout = 0;
}

/**************************************************************************//**
* 
* \brief   - ReadyForRequest Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForRequest::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		m_Actions = SCState::FAIL;
	}
}

/**************************************************************************//**
* 
* \brief   - ReadyForRequest Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForRequest::Exit()
{
	
}

/**************************************************************************//**
* 
* \brief   - ReadyForRequest Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReadyForRequest::Fail()
{
	m_Actions = SCState::ABORT;
	LOGERR((char *)"ReadyForRequest State Alarm happened!\n", 0, 0, 0);
}

/**************************************************************************//**
* 
* \brief   - ReadyForRequest Execute.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int ReadyForRequest::Execute()
{
	return OK;
}
