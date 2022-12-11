/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 SC Sonics Test State. 
 According to feature required, the state needs to be triggered by external, such as the button pressed.
 So there is the counter to record if the button is still pressed. If the   
 
***************************************************************************/

#include "TestSonicOn.h"
#include "SCStateMachine.h"
#include "../Recipe.h"
#include "../PCStateMachine.h"
#include "../ACStateMachine.h"
#include "../WeldResultSignature.h"
#include "../ControlTask.h"
#include "../Utility.h"
#include "../TestSonicsWorkFlow.h"

/**************************************************************************//**
* 
* \brief   - Constructor - initialize Test Sonics state.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
TestSonicOn::TestSonicOn() 
{
	m_Actions = SCState::INIT;
	m_State = SCState::TEST_SONIC_ON;
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
TestSonicOn::~TestSonicOn() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Test Sonic On Enter. Initialize variables
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicOn::Enter()
{
	m_Timeout = 0;
	PCStateMachine::PC_RX->MasterEvents &= ~ BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	PCStateMachine::PC_RX->MasterState = SCState::WELD_SONIC_ON; //Move PC state to Sonics On State.
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_SONICS_SET);
	//TODO Clean up Test Signature Vector
}

/**************************************************************************//**
*
* \brief   - Test Sonic On Loop.
*            Send Sonics test result per each 500ms if there is not any alarm happend.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicOn::Loop()
{
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		//If there is not any State that is equal to requested, there is alarm happened in PC.
		LOG("Current PC State: %d\n", PCStateMachine::PC_TX->PCState);
		m_Actions = SCState::FAIL;
		return;
	}

	m_Timeout++;
	if((m_Timeout % DEDAULT_WELD_TIME) == 0)
	{
		TestSonicsWorkFlow::TestResult.Frequency = Utility::HEX2Frequency(PCStateMachine::PC_TX->Frequency);
		TestSonicsWorkFlow::TestResult.Power = Utility::HEX2Power(PCStateMachine::PC_TX->Power);
		SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_SC_RESPONSE);                 //  Update current Power and Frequency to UI Controller
	}

	//TODO test sonics data signature sampling.
}

/**************************************************************************//**
*
* \brief   - Test Sonics On Exit. Close sonics on.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicOn::Exit()
{
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
	PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_WELDTEST_ENABLE);
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_SONICS_RESET);
	LOG("Test Sonics Weld Time = %d Timeout = %d\n", m_Timeout);
}

/**************************************************************************//**
*
* \brief   - Test Sonics On Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TestSonicOn::Fail()
{
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
	LOG("TestSonicOn Alarm happened!\n");
	//Record Database alarm table
	m_Actions = SCState::ABORT;
}

/**************************************************************************//**
*
* \brief   - Test Sonics On Trigger. 
* 			 To close sonics test and JUMP State
*
* \param   - _obj TestSonicsOn object.
*
* \return  - OK.
*
******************************************************************************/
int	TestSonicOn::Execute(void* _obj)
{
	TestSonicOn* _TestSonic = (TestSonicOn*)_obj;
	_TestSonic->m_Actions = SCState::JUMP;		 //  If the button is released, JUMP state.
	return OK;
}

