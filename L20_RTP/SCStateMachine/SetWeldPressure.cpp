/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------  
 
 To set Weld Pressure before the sonics on. 
 As we know there is the special case when the trigger pressure is not equal to weld pressure.
 The trigger pressure can be set at the pre-ready state. 
 There is the short response time on air regulator about 200ms, 
 we can set trigger pressure in the pre-ready state and take the horn down state to cover this response time.
 For weld pressure setting there is not any way can cover this response time before sonics on 
 so we have to generate a new state to handle with the response time when weld pressure setting.   
 
***************************************************************************/

#include "SetWeldPressure.h"
#include "../Recipe.h"
#include "../ACStateMachine.h"
/**************************************************************************//**
*
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SetWeldPressure::SetWeldPressure() {
	m_Actions = SCState::INIT;
	m_State = SCState::WELD_PRESSURE_SET;
	m_Timeout = 0;
	m_WPressure = 0;

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
SetWeldPressure::~SetWeldPressure() {
	m_Actions = SCState::INIT;
}

/**************************************************************************//**
*
* \brief   - SetWeldPressure Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SetWeldPressure::Enter()
{
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WP_PRESSURE, &m_WPressure);
	ACStateMachine::AC_RX->MasterState = SCState::WELD_PRESSURE_SET;
	m_Timeout = 0;
}

/**************************************************************************//**
*
* \brief   - SetWeldPressure Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SetWeldPressure::Loop()
{
	if(ACStateMachine::AC_RX->TargetPressure == m_WPressure)
		m_Actions = SCState::JUMP;
	else
	{
		if (m_Timeout < DELAY200MSEC)
			m_Timeout++;
		else
			m_Actions = SCState::JUMP;
	}
}

/**************************************************************************//**
*
* \brief   - SetWeldPressure Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SetWeldPressure::Exit()
{
	ACStateMachine::AC_RX->MasterState = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - SetWeldPressure Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SetWeldPressure::Fail()
{
}

