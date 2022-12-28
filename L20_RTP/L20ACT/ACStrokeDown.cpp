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
* \brief   - ACT Stroke Down. Initialize the moved count to check if the horn has been moving.
* 			 Initialize m_Timeout, Reset bit STATUS_PART_CONTACT_FOUND in AC StatusEvent. 
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
		m_MovedCount = HeightEncoder::GetInstance()->GetPositionCount() - MOVE_NOISE;
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_HIGH);
		m_Timeout = 0;
	}
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_PART_CONTACT_FOUND);
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Down.  The process to check if the horn has been reached out part using IsMoving
* 			 when bit CTRL_PART_CONTACT_ENABLE is set in MasterEvents.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeDown::Loop()
{
	//If timeout happens, the ERR_HEIGHT_SYSTEM error will be set.
	if(m_Timeout > HORN_MOVE_TIMEOUT)
	{
		ActuatorTask::SetCoreState(ERR_HEIGHT_SYSTEM);
		return;
	}
	
	if(ACStateMachine::AC_RX->MasterState == SCState::PRE_READY)
		ChangeState(AC_READY);
	
	//If the current count is less than Moved Count, it means the horn has been moving.
	if(HeightEncoder::GetInstance()->GetPositionCount() < m_MovedCount)
	{
		//Reset bit Status_AC_MOVE_DISABLE in AC_StatusEvent, because the horn is moving.
		ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_AC_MOVE_DISABLE);
		if(((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_PART_CONTACT_ENABLE)) == BIT_MASK(CTRL_PART_CONTACT_ENABLE)) ||
				((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_PREBURST_ENABLE)) == BIT_MASK(CTRL_PREBURST_ENABLE)))
		{
			if(ActuatorTask::GetInstance()->IsMoving() == false)
			{
				ChangeState(AC_HOLD);
				ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_PART_CONTACT_FOUND);
			}
		}
		else
		{
			ChangeState(AC_HOLD);
			LOG("CTRL_PART_CONTACT_DISABLE\n");
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
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in ACT
		ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(CTRL_AC_CLEAR_ALARMS);
	}
}


