/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC PreReady state
***************************************************************************/

#include "PreReady.h"
#include "../HeightEncoder.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"

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
* \brief   - PreReady Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Enter()
{
	ACStateMachine::AC_RX->TargetPressure = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure;
	PCStateMachine::PC_RX->TargetAmplitude = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude;
//	LOG("GetInitCount = %d\n", HeightEncoder::GetInitCount());
//	LOG("GetMaxCount = %d\n", HeightEncoder::GetMaxCount());
	LOG("GetPositionCount = %d\n", HeightEncoder::GetInstance()->GetPositionCount());
//	LOG("GetDirection = %d\n", HeightEncoder::GetDirection());
}

/**************************************************************************//**
* 
* \brief   - PreReady Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Loop()
{
	if(ACStateMachine::AC_TX->ACState == ACState::AC_READY)
		m_Actions = SCState::JUMP;
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		//TODO Record Database alarm table
		CommonProperty::WeldResult.ALARMS.AlarmFlags.HeightSystemFailure = 1;
		m_Actions = SCState::FAIL;
	}
}

/**************************************************************************//**
* 
* \brief   - PreReady Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PreReady::Exit()
{
	
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
	if (ProcessAlarmHandler() == true)
		m_Actions = SCState::ABORT;
	LOG("Pre-Ready Alarm process!\n");
}
