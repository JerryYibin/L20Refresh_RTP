/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACStateMachine.h"
#include "ActuatorTask.h"
ACStateMachine::TxPDO_AC* ACStateMachine::AC_TX = nullptr;
ACStateMachine::RxPDO_AC* ACStateMachine::AC_RX = nullptr;
ACState* ACStateMachine::CurrentStateObj = nullptr;
ACStateMachine* ACStateMachine::StateMachineObj = nullptr;

ACStateMachine * ACStateMachine::GetInstance()
{
	return (StateMachineObj != nullptr) ? StateMachineObj : (StateMachineObj = new ACStateMachine());
}

/**************************************************************************//**
* \brief   - Constructor - 
* 			 AC state machine will be instantiated the AlarmyObj, EstopObj, PowerUpObj,
* 			 ReadyObj, StrokeDownObj, StrokeUpObj and WeldHoldObj.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACStateMachine::ACStateMachine() 
{
	m_AlarmObj 			= new ACAlarm();
	m_EstopObj			= new ACEstop();
	m_PowerUpObj		= new ACPowerUp();
	m_ReadyObj			= new ACReady();
	m_StrokeDownObj 	= new ACStrokeDown();
	m_StrokeUpObj		= new ACStrokeUp();
	m_ToHoldObj			= new ACWeldHold();
	m_StartSwitchObj	= new ACStartSwitch();
	CurrentStateObj = m_PowerUpObj;
	CurrentStateObj->Enter();
	LOG("\n%s ACT_T: _AC_POWERUP%s\n", KGRN, KNRM);
}

/**************************************************************************//**
* 
* \brief   - Destructor. Release states objects
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ACStateMachine::~ACStateMachine() {
	if(m_AlarmObj != nullptr)
		delete m_AlarmObj;
	if(m_EstopObj != nullptr)
		delete m_EstopObj;
	if(m_PowerUpObj != nullptr)
		delete m_PowerUpObj;
	if(m_ReadyObj != nullptr)
		delete m_ReadyObj;
	if(m_StrokeDownObj != nullptr)
		delete m_StrokeDownObj;
	if(m_StrokeUpObj != nullptr)
		delete m_StrokeUpObj;
	if(m_ToHoldObj != nullptr)
		delete m_ToHoldObj;
	if(m_StartSwitchObj != nullptr)
		delete m_StartSwitchObj;
}

/**************************************************************************//**
* 
* \brief   - Run AC State Machine.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStateMachine::RunStateMachine()
{
	ACStateMachine::GetInstance();
	if(((AC_RX->MasterEvents & BIT_MASK(ACState::CTRL_ESTOP)) == BIT_MASK(ACState::CTRL_ESTOP)) && (CurrentStateObj->stateType != ACState::AC_ESTOP))
	{		
		ChangeState(ACState::AC_ESTOP);
	}
	else if ((ActuatorTask::GetCoreState() != NO_ERROR) && (CurrentStateObj->stateType != ACState::AC_ALARM))
	{
		ChangeState(ACState::AC_ALARM);
	}
	
	CurrentStateObj->Loop();
}

/**************************************************************************//**
* 
* \brief   - Get AC Current State.
*
* \param   - None.
*
* \return  - State enum.
*
******************************************************************************/
ACState::ACSTATES ACStateMachine::GetStateType()
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
void ACStateMachine::ChangeState(ACState::ACSTATES nextState)
{
	bool bValidState = true;
	ACState* previousStateObj = CurrentStateObj;
	ACStateMachine* _obj = GetInstance();
	switch(nextState)
	{
	case ACState::AC_POWERUP:
		LOG("\n%sACT_T: _AC_POWERUP%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_PowerUpObj;
		CurrentStateObj->stateType = ACState::AC_POWERUP;
		break;
	case ACState::AC_READY:
		LOG("\n%sACT_T: _AC_READY%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_ReadyObj;
		CurrentStateObj->stateType = ACState::AC_READY;
		break;
	case ACState::AC_DOWN_STROKE:
		LOG("\n%sACT_T: _AC_DOWN_STROKE%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_StrokeDownObj;
		CurrentStateObj->stateType = ACState::AC_DOWN_STROKE;
		break;
	case ACState::AC_RETURN_STROKE:
		LOG("\n%sACT_T: _AC_RETURN_STROKE%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_StrokeUpObj;
		CurrentStateObj->stateType = ACState::AC_RETURN_STROKE;
		break;
	case ACState::AC_HOLD:
		LOG("\n%sACT_T: _AC_WELD_HOLD%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_ToHoldObj;
		CurrentStateObj->stateType = ACState::AC_HOLD;
		break;
	case ACState::AC_STARTSWICH:
		LOG("\n%sACT_T: _AC_Start_Switch%s\n", KGRN, KNRM);
		
		CurrentStateObj = _obj->m_StartSwitchObj;
		CurrentStateObj->stateType = ACState::AC_STARTSWICH;
		break;
	case ACState::AC_ESTOP:
		LOG("\n%sACT_T: _AC_ESTOP%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_EstopObj;
		CurrentStateObj->stateType = ACState::AC_ESTOP;
		break;
	case ACState::AC_ALARM:
		LOG("\n%sACT_T: _AC_ALARM%s\n", KGRN, KNRM);

		CurrentStateObj = _obj->m_AlarmObj;
		CurrentStateObj->stateType = ACState::AC_ALARM;
		break;
	default:
		bValidState = false;
		break;
	}
	
	if(bValidState == true)
	{
		previousStateObj->Exit();
		CurrentStateObj->Enter();
		AC_TX->ACState = nextState;
	}
}

