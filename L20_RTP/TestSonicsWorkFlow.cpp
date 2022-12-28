/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#include "TestSonicsWorkFlow.h"
#include "SCStateMachine/SCStateMachine.h"
#include "PCStateMachine.h"
#include "commons.h"
SONICS_TEST TestSonicsWorkFlow::TestResult;
/**************************************************************************//**
* \brief   - Constructor - 
* 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
TestSonicsWorkFlow::TestSonicsWorkFlow() {
	m_TriggerCounter = 0;
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
TestSonicsWorkFlow::~TestSonicsWorkFlow() {
	m_TriggerCounter = 0;
}

/**************************************************************************//**
* 
* \brief   - Initialize Test Sonics process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicsWorkFlow::InitProcess(void)
{
	PCStateMachine::PC_RX->TargetAmplitude = m_Amplitude;
	SCStateMachine::getInstance()->ExecuteStateAction(SCState::READY_FOR_TRIGGER);
}

/**************************************************************************//**
* 
* \brief   - Trigger Test sonics process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicsWorkFlow::TriggerProcess(void)
{
	InitProcess();
	m_TriggerCounter++;
	m_State = WorkFlow::ONGOING;
}

/**************************************************************************//**
* 
* \brief   - Update Test sonics result.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int TestSonicsWorkFlow::UpdateResult(void)
{

	return OK;
}

/**************************************************************************//**
* 
* \brief   - Run Test Sonics work flow.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int TestSonicsWorkFlow::RunProcess(void)
{
	//TODO not sure if there is anything need to do for ACE, because the ACE needs to update curve.
	if(m_TriggerCounter > 0)
	{
		m_TriggerCounter--;
		if(m_TriggerCounter == 0)
		{
			SCStateMachine::getInstance()->ExecuteStateAction(SCState::TEST_SONIC_ON);
			m_State = WorkFlow::FINISH;
		}
	}
	else if(m_TriggerCounter == 0)
	{
		SCStateMachine::getInstance()->ExecuteStateAction(SCState::TEST_SONIC_ON);
		m_State = WorkFlow::FINISH;
	}
	return m_State;
}

/**************************************************************************//**
* 
* \brief   - Reset Test sonics process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicsWorkFlow::ResetProcess(void)
{
	m_TriggerCounter = 0;
	SCStateMachine::getInstance()->ExecuteStateAction(SCState::TEST_SONIC_ON);
	m_State = WorkFlow::INVALID;
}

/**************************************************************************//**
* 
* \brief   - Set Amplitude for 100% and current amplitude.
*
* \param   - amplitude
*
* \return  - None.
*
******************************************************************************/
void TestSonicsWorkFlow::SetAmplitude(unsigned int amplitude)
{
	m_Amplitude = amplitude;
}
