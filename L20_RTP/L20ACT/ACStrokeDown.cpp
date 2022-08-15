/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACStrokeDown.h"
#include "../ACStateMachine.h"
#include "../GPIO.h"
#include "../ActuatorTask.h"
#include "../SCStateMachine/SCState.h"
#include "../HeightEncoder.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACStrokeDown::ACStrokeDown() {
	stateType = ACState::AC_DOWN_STROKE;

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
ACStrokeDown::~ACStrokeDown() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Down.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeDown::Enter()
{
	//Horn Down
	if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_AC_MOVE_DISABLE)) == 0)
	{
		//TODO the hardware code will be replaced with the override function for the different system type.
		m_TriggerHeight = HeightEncoder::GetPositionCount() - MOVE_NOISE;
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_HIGH);
		m_Timeout = 0;
		ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_AC_MOVE_DISABLE);
	}
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_PART_CONTACT_FOUND);
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Down.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeDown::Loop()
{
	if((ACStateMachine::AC_RX->MasterState != SCState::WAIT_FOR_TRIGGER) && 
			(ACStateMachine::AC_RX->MasterState != SCState::DOWN_STROKE))
		return;

	if(m_Timeout > HORN_MOVE_TIMEOUT)
	{
		unsigned int coreStatus = ActuatorTask::GetCoreState();
		ActuatorTask::SetCoreState(coreStatus | ERR_HEIGHT_SYSTEM);
		return;
	}
	
	if(HeightEncoder::GetPositionCount() < m_TriggerHeight)
	{
		if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_PART_CONTACT_ENABLE)) == BIT_MASK(CTRL_PART_CONTACT_ENABLE))
		{
			if(ActuatorTask::IsMoving() == false)
			{
				ChangeState(AC_HOLD);
				ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_PART_CONTACT_FOUND);
			}
		}
		else
		{
			ChangeState(AC_HOLD);
		}
	}
	m_Timeout++;
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Down.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeDown::Exit()
{
	// check ACT status
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in ACT
		ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(CTRL_AC_CLEAR_ALARMS);
	}
}


