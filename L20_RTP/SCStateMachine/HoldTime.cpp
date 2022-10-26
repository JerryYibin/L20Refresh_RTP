/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "HoldTime.h"
#include "../Recipe.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
HoldTime::HoldTime() {
	m_Actions = SCState::INIT;
	m_State = SCState::HOLD;

}

/**************************************************************************//**
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
HoldTime::~HoldTime() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - HoldTime Enter. To get current Hold Time from Active Recipe.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HoldTime::Enter()
{
	m_iHoldTime = 0;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::HOLD_TIME, &m_iHoldTime);
	m_Timeout = 0;
}

/**************************************************************************//**
*
* \brief   - Hold Time Loop. Hold time delay.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HoldTime::Loop()
{
	if (m_Timeout > m_iHoldTime)
		m_Actions = SCState::JUMP;
	else
		m_Timeout++;
}

/**************************************************************************//**
*
* \brief   - Hold time Exit 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HoldTime::Exit()
{
	m_Timeout = 0;
}

void HoldTime::Fail()
{
}

