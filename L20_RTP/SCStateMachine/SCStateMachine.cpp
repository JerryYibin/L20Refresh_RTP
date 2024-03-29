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
#include "ReadyForTrigger.h"
#include "SetWeldPressure.h"
#include "SqueezeTime.h"
#include "HoldTime.h"
#include "TestSonicOn.h"
#include "Alarm.h"
#include "Estop.h"
#include "../PCStateMachine.h"
#include "../ACStateMachine.h"
#include "../Logger.h"
#include "../Recipe.h"
extern "C"
{
	#include "customSystemCall.h"	
}
/* StateMachine is implemented for the operation that is only foucs on the preset level not relevent to sequence or teachmode */

SCStateMachine* SCStateMachine::m_StateMachineObj = nullptr;
SCState* SCStateMachine::m_objState = nullptr;
SEM_ID SCStateMachine::m_semaphoreMutex = SEM_ID_NULL;
unsigned int SCStateMachine::CoreState = 0;
unsigned int SCStateMachine::AlarmStateIndex = 0;
unsigned int SCStateMachine::EStopStateIndex = 0;
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
	CoreState = 0;
	/*create semaphore*/
	m_semaphoreMutex = semMCreate(SEM_Q_FIFO);
	initStateMap();
	_objActionMap = new map<SCState::STATE, SCState*>();
	SelectAlarmEStopSequence();
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
	if(m_semaphoreMutex != SEM_ID_NULL)
	{
		semDelete(m_semaphoreMutex);
		m_semaphoreMutex = SEM_ID_NULL;
	}
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
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::CALIBRATE,					FALSE));	
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::READY_FOR_TRIGGER,			TRUE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::TEST_SONIC_ON,	 			TRUE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::ALARM,			 			FALSE));
	_objStateMap->insert(pair<SCState::STATE, int>(SCState::ESTOP,						FALSE));
	
	
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
void SCStateMachine::addActionToMap(SCState::STATE stateIdx, SCState* _action)
{
	if(_objActionMap != nullptr)
	{
		_objActionMap->insert(pair<SCState::STATE, SCState*>(stateIdx, _action));
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
		m_StateIndex = 0;
		return;
	}
	
	if (m_IsLoading == TRUE)
	{
		m_IsRunning = false;
		m_StateIndex = 0;
		return;
	}
	
	//Once the ESTOP is detected by AC, the SC will go to ESTOP state directly for E-Stop handling.
	if(ACStateMachine::AC_TX->ACState == ACState::AC_ESTOP)
	{
		if(m_objState->m_State != SCState::ESTOP)
		{
			m_objState->Exit();
			m_objState->m_Actions = SCState::INIT;
			m_StateIndex = EStopStateIndex;
		}
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
		if(m_objState->m_Actions == SCState::INIT)
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
//		LOG("Jump Next State\n");
		break;
	case SCState::ALJUMPNORM:
		m_objState->Exit();
		m_objState->m_Actions = SCState::INIT;
		m_StateIndex++;
		LOG("Alarm Jump Loop\n");
		break;
	case SCState::ABORT:
		m_objState->Exit();
		m_objState->m_Actions = SCState::INIT;
		m_StateIndex = AlarmStateIndex; //Directly go to alarm state.
		LOG("Abort\n");
		break;
	case SCState::PUSH:
		m_objState->Exit();
		m_objState->m_Actions = SCState::INIT;
		LOG("Push\n");
		break;
	default:
		m_StateIndex = 0;
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
	//If the state list is not empty, it means there is the specific sequence is running. 
	if(_objStateList->size() > 0)
	{
		auto iter = _objStateMap->find(m_objState->m_State);
		if(iter != _objStateMap->end())
		{
			//If the flag is TRUE, all the state of current sequence may be removed from state list when operation mode change.
			if(iter->second == TRUE)
			{
				if(m_IsRunning == true)
					return iResult;
				else
					deleteAll();
			}
			else
			{
				//If the current state is critical and running, it can't be stopped and removed, such as sonics on, calibration, alarm and estop.
				if(m_objState->m_Actions != SCState::LOOP)
				{
					if(m_IsRunning == true)
						return iResult;
					else
						deleteAll();
				}
			}
		}
		else
		{
			LOGERR((char *) "SC State Machine: State Missing, the current State: %d is not in the State Map.",m_objState->m_State, 0, 0);
		}
	}
	//TODO the following code should be a separate function. Once the states of list is removed, 
	//the list should be loaded the new sequence directly without waiting for the next loop.
	if(_objStateList->size() == 0)
	{
		switch(operation)
		{
		case WELD:
			SelectWeldSequence();
			break;
		case HEIGHT_CALIBRATE_READY:
			SelectHeightCalibrateSequence();
			break;
		case BATCH_WELD:
			SelectWeldSequence();
			break;
		case TEST:
			SelectSonicsTestSequence();
			break;
		default:
			break;
		}
		SelectAlarmEStopSequence();
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
	
	m_objState = new SeekSonicOn();
	_objStateList->push_back(m_objState);

	m_objState = new WaitForTrigger();
	_objStateList->push_back(m_objState);
	
	m_objState = new SqueezeTime();
	_objStateList->push_back(m_objState);
	
	m_objState = new SetWeldPressure();
	_objStateList->push_back(m_objState);

	m_objState = new WeldSonicOn();
	_objStateList->push_back(m_objState);
	
	m_objState = new HoldTime();
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
	m_objState = nullptr;
	m_objState = new ReadyForTrigger();
	_objStateList->push_back(m_objState);
	addActionToMap(m_objState->m_State, m_objState);
	
	m_objState = new HeightCalibrate();
	_objStateList->push_back(m_objState);
}

/**************************************************************************//**
* 
* \brief   - State Machine Loop for Sonics test.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCStateMachine::SelectSonicsTestSequence(void)
{
	m_objState = nullptr;
	m_objState = new ReadyForTrigger();
	_objStateList->push_back(m_objState);
	addActionToMap(m_objState->m_State, m_objState);
	
	m_objState = new SeekSonicOn(SeekSonicOn::TEST_SEEK_ON);
	_objStateList->push_back(m_objState);
	
	m_objState = new TestSonicOn();
	_objStateList->push_back(m_objState);
	addActionToMap(m_objState->m_State, m_objState);
}

void SCStateMachine::SelectAlarmEStopSequence(void)
{
	m_objState = nullptr;
	AlarmStateIndex = _objStateList->size();
	m_objState = new Alarm();
	_objStateList->push_back(m_objState);
	EStopStateIndex = _objStateList->size();
	m_objState = new Estop();
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
	int iResult = ERROR;
	SCState* _action = nullptr;
	semTake(m_semaphoreMutex, WAIT_FOREVER); //lock
	auto iter = _objActionMap->find(stateIdx);
	if (iter != _objActionMap->end())
	{
		_action = iter->second;
		switch(stateIdx)
		{
		case SCState::READY_FOR_TRIGGER:
			iResult = ReadyForTrigger::Execute(_action);
			break;
		case SCState::TEST_SONIC_ON:
			iResult = TestSonicOn::Execute(_action);
		default:
			break;
		}
	}
	semGive(m_semaphoreMutex); //release semaphoreMutex lock
	return iResult;
}

/**************************************************************************//**
* \brief  	- Specific SC core status
*
* \param	- None
*
* \return 	- uint32_t
*
******************************************************************************/
unsigned int SCStateMachine::GetCoreState()
{
	return CoreState;;
}

/**************************************************************************//**
* \brief  	- Set SC core status
*
* \param	- coreState
*
* \return 	- None
*
******************************************************************************/
void SCStateMachine::SetCoreState(unsigned int coreState)
{
	CoreState |= coreState;
}

/**************************************************************************//**
* \brief  	- Clear SC core status
*
* \param	- coreState
*
* \return 	- None
*
******************************************************************************/
void SCStateMachine::ClearCoreState(unsigned int coreState)
{
	CoreState &= ~coreState;
}
