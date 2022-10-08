/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACStrokeUp.h"
#include "../ACStateMachine.h"
#include "../GPIO.h"
#include "../ActuatorTask.h"
#include "../SCStateMachine/SCState.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACStrokeUp::ACStrokeUp() {
	stateType = ACState::AC_RETURN_STROKE;

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
ACStrokeUp::~ACStrokeUp() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeUp::Enter()
{
	//Horn up
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_AC_MOVE_DISABLE)) == 0)
	{
		//TODO the hardware code will be replaced with the override function for the different system type.
		m_Timeout = 0;
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
		ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_AC_MOVE_DISABLE);
	}
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_READY_SIGNAL_ON);
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_HOME_POSITION_FOUND);
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeUp::Loop()
{
	if((ACStateMachine::AC_RX->MasterState != SCState::WAIT_FOR_READY_POSITION) &&
			(ACStateMachine::AC_RX->MasterState != SCState::RETURN_STROKE))
		return;
	
	if(m_Timeout > HORN_MOVE_TIMEOUT)
	{
		ActuatorTask::SetCoreState(ERR_HOME_POSITION);
		return;
	}
	
	// The signal will be available later
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_READY_CHECK_ENABLE)) == BIT_MASK(CTRL_READY_CHECK_ENABLE))
	{
//		if (vxbGpioGetValue(GPIO::I_OPSIG) == GPIO_VALUE_HIGH)
			ChangeState(AC_READY);
			ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_READY_SIGNAL_ON);
	}
	else if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_HOME_POSITION_ENABLE)) == BIT_MASK(CTRL_HOME_POSITION_ENABLE))
	{
		if(ActuatorTask::GetInstance()->IsMoving() == false)
		{
//			if (vxbGpioGetValue(GPIO::I_OPSIG) == GPIO_VALUE_HIGH)
//			{
				ChangeState(AC_READY);
				ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_HOME_POSITION_FOUND);
//			}
		}
	}
	else
		ChangeState(AC_READY);

	m_Timeout++;
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeUp::Exit()
{
	// check ACT status
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in ACT
		ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(CTRL_AC_CLEAR_ALARMS);
	}
}

