/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC PreReady state
***************************************************************************/

#include "PreReady.h"
#include "../Recipe.h"
#include "../HeightEncoder.h"
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
PreReady::PreReady() {
	m_Actions = SCState::INIT;
	m_State = SCState::PRE_READY;
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
PreReady::~PreReady() {
	m_Actions = SCState::INIT;
	m_State = SCState::PRE_READY;
}

/**************************************************************************//**
* 
* \brief   - PreReady Enter. To set Trigger pressure and start amplitude.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Enter()
{
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TP_PRESSURE, &ACStateMachine::AC_RX->TargetPressure);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AMPLITUDE, &PCStateMachine::PC_RX->TargetAmplitude);
	//LOG("GetInitCount = %d\n", HeightEncoder::GetInitCount());
	//LOG("GetMaxCount = %d\n", HeightEncoder::GetMaxCount());
	LOG("GetPositionCount = %d\n", HeightEncoder::GetInstance()->GetPositionCount());
	//LOG("GetDirection = %d\n", HeightEncoder::GetDirection());
	ACStateMachine::AC_RX->MasterState = SCState::PRE_READY;
}

/**************************************************************************//**
* 
* \brief   - PreReady Loop. And waitting for the both start switches are released. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_READY)
	{
		if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_START_SWITCH_PRESSED)) == 0)
			m_Actions = SCState::JUMP;
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		m_Actions = SCState::FAIL;
	}
}

/**************************************************************************//**
* 
* \brief   - PreReady Exit to set external digital I/O.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Exit()
{
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_READY_SET);
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
* 
* \brief   - PreReady Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Pre-Ready Alarm process!\n");
}
