/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCStateMachine.h"
#include "Utils.h"
PCStateMachine::TxPDO_PC* PCStateMachine::PC_TX = nullptr;
PCStateMachine::RxPDO_PC* PCStateMachine::PC_RX = nullptr;
PCState* PCStateMachine::CurrentStateObj = nullptr;
PCStateMachine* PCStateMachine::StateMachineObj = nullptr;
/**************************************************************************//**
* \brief   - Constructor - 
* 			 PC state machine will be instantiated the ReadyObj, WeldRunObj, EstopObj,
* 			 AlarmObj and SeekRunObj following the system type, such as L20 refresh and P1.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PCStateMachine::PCStateMachine() 
{
	m_ReadyObj 		= new PCReady();
	m_WeldRunObj 	= new PCWeldRun();
	m_EstopObj 		= new PCEstop();
	m_AlarmObj 		= new PCAlarm();
	m_SeekRunObj 	= new PCSeekRun();
	
	CurrentStateObj = m_ReadyObj;
	CurrentStateObj->Enter();
}

/**************************************************************************//**
* 
* \brief   - Destructor. Release ReadyObj, WeldRunObj, EstopObj, AlarmObj and SeekRunObj
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
PCStateMachine::~PCStateMachine() 
{
	if(m_ReadyObj != nullptr)
		delete m_ReadyObj;
	if(m_WeldRunObj != nullptr)
		delete m_WeldRunObj;
	if(m_EstopObj != nullptr)
		delete m_EstopObj;
	if(m_AlarmObj != nullptr)
		delete m_AlarmObj;
	if(m_SeekRunObj != nullptr)
		delete m_SeekRunObj;
}

/**************************************************************************//**
* 
* \brief   - Run PC State Machine.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCStateMachine::RunStateMachine()
{
	PCStateMachine::GetInstance();
	if(((PC_RX->MasterEvents & BIT_MASK(PCState::CTRL_ESTOP)) == BIT_MASK(PCState::CTRL_ESTOP)) && (CurrentStateObj->stateType != PCState::PC_ESTOP))
	{		
		// take actions for Power supply module here...
		ChangeState(PCState::PC_ESTOP);
	}
	
	CurrentStateObj->Loop();
}

/**************************************************************************//**
* 
* \brief   - Get PC Current State.
*
* \param   - None.
*
* \return  - State enum.
*
******************************************************************************/
PCState::PCSTATES PCStateMachine::GetStateType()
{
	return CurrentStateObj->GetStateType();
}

/**************************************************************************//**
* 
* \brief   - Change Current State.
*
* \param   - State enum.
*
* \return  - None.
*
******************************************************************************/
void PCStateMachine::ChangeState(PCState::PCSTATES nextState)
{
	bool bValidState = true;
	PCState* previousStateObj = CurrentStateObj; 
	PCStateMachine* _obj = GetInstance();
	switch(nextState)
	{
	case PCState::PC_READY:
		LOG("\n%sPS_T: _PC_READY%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_ReadyObj;
		CurrentStateObj->stateType = PCState::PC_READY;
		break;
	case PCState::PC_WELD_RUN:
		LOG("\n%sPS_T: _PC_WELD_RUN%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_WeldRunObj;
		CurrentStateObj->stateType = PCState::PC_WELD_RUN;
		break;
	case PCState::PC_ESTOP:
		LOG("\n%sPS_T: _PC_ESTOP%s\n", KRED, KNRM);
		
		CurrentStateObj = _obj->m_EstopObj;
		CurrentStateObj->stateType = PCState::PC_ESTOP;
		break;
	case PCState::PC_ALARM:
		LOG("\n%sPS_T: _PC_ALARM%s\n", KRED, KNRM);
		
		CurrentStateObj = _obj->m_AlarmObj;
		CurrentStateObj->stateType = PCState::PC_ALARM;
		break;
	case PCState::PC_SEEK_RUN:
		LOG("\n%sPS_T: _PC_SEEK_RUN%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_SeekRunObj;
		CurrentStateObj->stateType = PCState::PC_SEEK_RUN;
		break;
	default:
		bValidState = false;
		break;
	}
	
	if(bValidState)
	{
		previousStateObj->Exit();
		CurrentStateObj->Enter();
		PC_TX->PCState = nextState;
	}
}

PCStateMachine*	PCStateMachine::GetInstance()
{
	return (StateMachineObj != nullptr) ? StateMachineObj : (StateMachineObj = new PCStateMachine());
}
