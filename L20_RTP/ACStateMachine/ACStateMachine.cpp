/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACStateMachine.h"
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
* 			 PC state machine will be instantiated the ReadyObj, WeldRunObj, EstopObj,
* 			 AlarmObj and SeekRunObj following the system type, such as L20 refresh and P1.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

ACStateMachine::ACStateMachine() {
	// TODO Auto-generated constructor stub

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
ACStateMachine::~ACStateMachine() {
	// TODO Auto-generated destructor stub
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
void ACStateMachine::RunStateMachine()
{
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
	return ACState::ACSTATES();
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
void ACStateMachine::ChangeState(ACState::ACSTATES)
{
}

