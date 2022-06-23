/*
 * SCStateMachine.cpp
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */
#include "SCStateMachine.h"
#include "PreReady.h"
#include "Ready.h"
#include "StartSwitch.h"
#include "Seek.h"
#include "WeldSonicOn.h"
#include "WaitForTrigger.h"
#include "ReleaseWeld.h"
#include "Logger.h"
extern "C"
{
	#include "customSystemCall.h"	
}
/* StateMachine is implemented for the operation that is only foucs on the preset level not relevent to sequence or teachmode */
SCStateMachine* SCStateMachine::m_StateMachineObj = nullptr;
SCState* SCStateMachine::m_objState = nullptr;
SCStateMachine::SCStateMachine() {
	_objStateList = new vector<SCState*>();
	m_StateIndex = 0;
	m_IsRunning = false;
	m_IsLoading = false;
	initStateMap();
}

SCStateMachine * SCStateMachine::getInstance()
{
	return (m_StateMachineObj != NULL) ? m_StateMachineObj : (m_StateMachineObj = new SCStateMachine());
}

SCStateMachine::~SCStateMachine() {
	deleteAll();
	_objStateMap->clear();
	delete _objStateMap;
}

void SCStateMachine::deleteAll()
{
	vector<SCState*>::iterator iter = _objStateList->begin();
	while (iter != _objStateList->end())
	{
		SCState* _objState = *iter;
		delete _objState;
		iter++;
	}
	_objStateList->clear();
}


void SCStateMachine::initStateMap()
{
	_objStateMap  = new map<int, int>();
	_objStateMap->insert(pair<int, int>(SCState::PRE_READY, TRUE));
	_objStateMap->insert(pair<int, int>(SCState::READY, TRUE));
	_objStateMap->insert(pair<int, int>(SCState::START_SWITCH, TRUE));
	_objStateMap->insert(pair<int, int>(SCState::WAIT_TRIGGER, TRUE));
	_objStateMap->insert(pair<int, int>(SCState::SONICS_ON, FALSE));
	_objStateMap->insert(pair<int, int>(SCState::RELEASE_PART, TRUE));
}

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
		m_objState->Init();
		break;
	case SCState::LOOP:
		m_objState->Loop();
		break;
	case SCState::FAIL:
		m_objState->Fail();
		break;
	case SCState::JUMP:
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

bool SCStateMachine::GetStateMachineState() const
{
	return m_IsRunning;
}

int	SCStateMachine::GetCurrentStateIndex() const
{
	return m_objState->m_State;
}

int SCStateMachine::LoadStatesHandler(int operation)
{
	m_IsLoading = TRUE;
	int iResult = ERROR;
	if(_objStateList->size() > 0)
	{
		map<int, int>::iterator iter = _objStateMap->find(m_objState->m_State);
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
		default:
			break;
		}
		iResult = OK;
	}
	m_IsLoading = FALSE;
	return iResult;
}

void SCStateMachine::SelectWeldSequence(void)
{
	m_objState = NULL;
	m_objState = new PreReady();
	_objStateList->push_back(m_objState);

	m_objState = new Ready();
	_objStateList->push_back(m_objState);

	m_objState = new StartSwitch();
	_objStateList->push_back(m_objState);
	
	m_objState = new Seek();
	_objStateList->push_back(m_objState);

	m_objState = new WaitForTrigger();
	_objStateList->push_back(m_objState);

	m_objState = new WeldSonicOn();
	_objStateList->push_back(m_objState);

	m_objState = new ReleaseWeld();
	_objStateList->push_back(m_objState);

}

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

void SCStateMachine::SetCoolingTimer(unsigned int delay, unsigned int duration)
{
	m_CoolDelayTimer = delay;
	m_CoolDurationTimer = duration;
}

