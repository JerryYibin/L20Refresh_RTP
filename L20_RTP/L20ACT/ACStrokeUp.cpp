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
#include "../HeightEncoder.h"
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
* \brief   - ACT Stroke Up. Initialize the moved count to check if the horn has been moving.
* 			 Initialize m_Timeout, Reset bit STATUS_HOME_POSITION_FOUND in AC StatusEvent. 
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
		m_MovedCount = HeightEncoder::GetInstance()->GetPositionCount() + MOVE_NOISE;
		m_Timeout = 0;
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	}
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_HOME_POSITION_FOUND);
}

/**************************************************************************//**
* 
* \brief   - ACT Stroke Up. The process to check if the horn has been reached out Home Position using IsMoving and switch prox.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStrokeUp::Loop()
{
	//If timeout happens, the ERR_HEIGHT_SYSTEM error will be set.
	if(m_Timeout > HORN_MOVE_TIMEOUT)
	{
		ActuatorTask::SetCoreState(ERR_HOME_POSITION);
		return;
	}
	
	//If the current count is more than Moved Count, it means the horn has been moving.
	if(HeightEncoder::GetInstance()->GetPositionCount() > m_MovedCount)
	{
		//Reset bit Status_AC_MOVE_DISABLE in AC_StatusEvent, because the horn is moving.
		ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_AC_MOVE_DISABLE);

		if((ACStateMachine::AC_RX->MasterEvents & BIT_MASK(CTRL_ULS_ENABLE)) == BIT_MASK(CTRL_ULS_ENABLE))
		{
			if(ActuatorTask::GetInstance()->IsMoving() == false)
			{
//				if (vxbGpioGetValue(GPIO::I_OPSIG) == GPIO_VALUE_HIGH)
//				{
					ChangeState(AC_READY);
					ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_HOME_POSITION_FOUND);
//				}
			}
		}
		else
		{
			ChangeState(AC_READY);
			LOG("CTRL_HOME_POSITION_DISABLE\n");
		}
	}
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
		ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(CTRL_AC_CLEAR_ALARMS);
	}
}

