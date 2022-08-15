/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "SCStateMachine.h"
#include "PreReady.h"
#include "Ready.h"
#include "StartSwitch.h"
#include "SeekSonicOn.h"
#include "WeldSonicOn.h"
#include "WaitForTrigger.h"
#include "WaitForReadyPosition.h"
#include "HeightCalibrate.h"
#include "HeightCorrect.h"
#include "ReadyForRequest.h"
#include "../PCStateMachine.h"
#include "../ACStateMachine.h"
#include "../Logger.h"
extern "C"
{
	#include "customSystemCall.h"	
}
/* StateMachine is implemented for the operation that is only foucs on the preset level not relevent to sequence or teachmode */

SCStateMachine* SCStateMachine::m_StateMachineObj = nullptr;
SCState* SCStateMachine::m_objState = nullptr;

/**************************************************************************//**
* \brief   - Constructor -
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
SCStateMachine::SCStateMachine() {
	_objStateList = new vector<SCState*>();
	m_StateIndex = 0;
	m_IsRunning = false;
	m_IsLoading = false;
	initStateMap();
	_objActionMap = new map<SCState::STATE, SCAction*>();
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  -    SCStateMachine* Object
*
******************************************************************************/
SCStateMachine * SCStateMachine::getInstance()
{
	return (m_StateMachineObj != NULL) ? m_StateMachineObj : (m_StateMachineObj = new SCStateMachine());
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
SCStateMachine::~SCStateMachine() {
	deleteAll();
	_objStateMap->clear();
	delete _objStateMap;
	_objActionMap->clear();
	delete _objActionMap;
}

/**************************************************************************//**
*
* \brief   - Delete all objects in SC state List
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::deleteAll()
{
	auto iter = _objStateList->begin();
	while (iter != _objStateList->end())
	{
		SCState* _objState = *iter;
		delete _objState;
		iter++;
	}
	_objStateList->clear();
	_objActionMap->clear();
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_WELD_CYCLE_ENABLE);
}

/**************************************************************************//**
*
* \brief   - Init SC State Map
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::initStateMap()
{
	_objStateMap  = new map<SCState::STATE, int>();
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::PRE_READY, 					TRUE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::READY, 						TRUE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::START_SWITCH, 				TRUE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::WAIT_FOR_TRIGGER, 			FALSE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::WELD_SONIC_ON, 				FALSE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::WAIT_FOR_READY_POSITION, 	TRUE));
	
	
}

/**************************************************************************//**
*
* \brief   - Add the specific action of the state to the Map
*
* \param   - stateidx is the State Enum, action is the object.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::addActionToMap(SCState::STATE stateIdx, SCAction* _action)
{
	if(_objActionMap != nullptr)
	{
		_objActionMap->insert(pair<SCState::STATE, SCAction*>(stateIdx, _action));
	}
}

/**************************************************************************//**
*
* \brief   - Run SC State Machine.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::RunStateMachine()
{
	if (_objStateList->size() == 0)
	{
		m_IsRunning = false;
		return;
	}
	
	if (m_IsLoading == TRUE)
	{
		m_IsRunning = false;
		return;
	}
	
	m_IsRunning = true;
	if(m_StateIndex >= _objStateList->size())
		m_StateIndex = 0;
	m_objState = _objStateList->at(m_StateIndex);
	switch (m_objState->m_Actions)
	{
	case SCState::INIT:
		m_objState->m_StepIndex = m_StateIndex;
		m_objState->Enter();
		m_objState->m_Actions = SCState::LOOP;    // Only Enter INIT State Once
		break;
	case SCState::LOOP:
		m_objState->Loop();
		break;
	case SCState::FAIL:
		m_objState->Fail();
		break;
	case SCState::JUMP:
		m_objState->Exit();
		m_objState->m_Actions = SCState::INIT;
		m_StateIndex++;
		LOG("Jump Next State\n");
		break;
	case SCState::ALJUMPNORM:
		m_objState->m_Actions = SCState::INIT;
		m_StateIndex++;
		LOG("Alarm Jump Loop\n");
		break;
	case SCState::ABORT:
		m_objState->m_Actions = SCState::INIT;
		m_StateIndex = 0;
		LOG("Abort\n");
		break;
	case SCState::PUSH:
		m_objState->m_Actions = SCState::INIT;
		LOG("Push\n");
		break;
	default:
		LOG("Default\n");
		break;
	}
}

/**************************************************************************//**
*
* \brief   - Get SC State Machine status.
*
* \param   - None.
*
* \return  - Bool running or not.
*
******************************************************************************/
bool SCStateMachine::GetStateMachineState() const
{
	return m_IsRunning;
}

/**************************************************************************//**
*
* \brief   - Get SC Current State.
*
* \param   - None.
*
* \return  - State enum.
*
******************************************************************************/
int	SCStateMachine::GetCurrentStateIndex() const
{
	return m_objState->m_State;
}

/**************************************************************************//**
*
* \brief   - Load state objects in State List based on operation type
*
* \param   - SC Operation.
*
* \return  - If the Load States to list is successful, it returns OK,
* 			otherwise it will return ERROR; .
******************************************************************************/
int SCStateMachine::LoadStatesHandler(int operation)
{
	m_IsLoading = TRUE;
	int iResult = ERROR;
	if(_objStateList->size() > 0)
	{
		auto iter = _objStateMap->find(m_objState->m_State);
		if(iter != _objStateMap->end())
		{
			if(iter->second == TRUE)
			{
				if(m_objState->m_Actions != SCState::FAIL)
				{
					if(m_IsRunning == true)
						return iResult;
					else
						deleteAll();
				}
			}
		}
	}
	else
	{
		switch(operation)
		{
		case WELD:
			SelectWeldSequence();
			break;
		case HEIGHT_CALIBRATION:
			SelectHeightCalibrateSequence();
			break;
		default:
			break;
		}
		iResult = OK;
	}
	m_IsLoading = FALSE;
	return iResult;
}

/**************************************************************************//**
*
* \brief   - State Machine Loop for Weld Sequence.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::SelectWeldSequence(void)
{
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_WELD_CYCLE_ENABLE);
	m_objState = NULL;
	m_objState = new PreReady();
	_objStateList->push_back(m_objState);

	m_objState = new Ready();
	_objStateList->push_back(m_objState);

	m_objState = new StartSwitch();
	_objStateList->push_back(m_objState);
	
	m_objState = new SeekSonicOn();
	_objStateList->push_back(m_objState);

	m_objState = new WaitForTrigger();
	_objStateList->push_back(m_objState);

	m_objState = new WeldSonicOn();
	_objStateList->push_back(m_objState);

	m_objState = new WaitForReadyPosition();
	_objStateList->push_back(m_objState);

}

/**************************************************************************//**
* 
* \brief   - State Machine Loop for Height Calibration/ Check.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::SelectHeightCalibrateSequence(void)
{
	m_objState = NULL;
	
	m_objState = new ReadyForRequest();
	_objStateList->push_back(m_objState);
	
	m_objState = new HeightCalibrate();
	_objStateList->push_back(m_objState);
	
	m_objState = new HeightCorrect();
	_objStateList->push_back(m_objState);
	
}

/**************************************************************************//**
* 
* \brief   - Execute the action of the specific state
*
* \param   - stateIdx is the STATE enum.
*
* \return  - None.
*
******************************************************************************/
int	SCStateMachine::ExecuteStateAction(SCState::STATE stateIdx)
{
	auto iter = _objActionMap->find(stateIdx);
	int iResult = ERROR;
	SCAction* _action = nullptr;
	if (iter != _objActionMap->end())
	{
		_action = iter->second;
		iResult = _action->Execute();
	}
	return iResult;
}

/**************************************************************************//**
*
* \brief   - Trigger Cooling
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
//TODO Need to move to L20 actuator task, because it should be specific for L20 only.
void SCStateMachine::XTickTock()
{
	if (m_CoolDelayTimer == 0)
	{
		if (m_CoolDurationTimer != 0)
		{
			m_CoolDelayTimer--;
			if (m_CoolDelayTimer == 0)
			{
				//Turn OFF cooling air
				vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
			}
		}
	}
	else
	{
		m_CoolDelayTimer--;
		if(m_CoolDelayTimer == 0)
			vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_HIGH);

	}
}

/**************************************************************************//**
*
* \brief   - Set Cooling Timer
*
* \param   - Cooling delay time, Cooling duration
*
* \return  - None.
*
******************************************************************************/
//TODO Need to move to L20 actuator task, because it should be specific for L20 only.
void SCStateMachine::SetCoolingTimer(unsigned int delay, unsigned int duration)
{
	m_CoolDelayTimer = delay;
	m_CoolDurationTimer = duration;
}

