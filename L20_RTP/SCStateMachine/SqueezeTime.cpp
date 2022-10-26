/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "SqueezeTime.h"
#include "../Recipe.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
SqueezeTime::SqueezeTime() {
	m_Actions = SCState::INIT;
	m_State = SCState::SQUEEZE_TIME;

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
SqueezeTime::~SqueezeTime() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - SqueezeTime Enter. To get current Sequeeze Time from Active Recipe.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SqueezeTime::Enter()
{
	m_iSqueezeTime = 0;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::SQUEEZE_TIME, &m_iSqueezeTime);
	m_Timeout = 0;

}

/**************************************************************************//**
*
* \brief   - Squeeze Loop. Squeeze time delay.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SqueezeTime::Loop()
{
	if (m_Timeout > m_iSqueezeTime)
		m_Actions = SCState::JUMP;
	else
		m_Timeout++;
}

/**************************************************************************//**
*
* \brief   - Squeeze time Exit 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SqueezeTime::Exit()
{
	m_Timeout = 0;

}

/**************************************************************************//**
*
* \brief   - Squeeze time Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SqueezeTime::Fail()
{
	m_Actions = SCState::ABORT;
}

