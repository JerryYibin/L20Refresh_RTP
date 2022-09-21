/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#include "ACPowerUp.h"
#include "../ACStateMachine.h"
#include "../ActuatorTask.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACPowerUp::ACPowerUp() {
	stateType = ACState::AC_POWERUP;

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
ACPowerUp::~ACPowerUp() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - ACT Power up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACPowerUp::Enter()
{
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(CTRL_AC_MOVE_DISABLE);
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_AC_MOVE_DISABLE);
	m_Timeout = 0;
}

/**************************************************************************//**
* 
* \brief   - ACT Power up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACPowerUp::Loop()
{
	//Check if the Horn is at the home position using sensor.
	unsigned int coreStatus;
	if(m_Timeout > HORN_MOVE_TIMEOUT)
	{
		coreStatus = ActuatorTask::GetCoreState();
		ActuatorTask::SetCoreState(coreStatus | ERR_HOME_POSITION);
		return;
	}

//	if(ActuatorTask::GetInstance()->IsMoving() == false)
//	{
		//TODO The signal will be available later
//		if (vxbGpioGetValue(GPIO::I_OPSIG) != GPIO_VALUE_HIGH) 
			ChangeState(AC_READY);
			ActuatorTask::GetInstance()->InitHeightSystem();
//	}
	m_Timeout++;
}

/**************************************************************************//**
* 
* \brief   - ACT Power up.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACPowerUp::Exit()
{
	//TODO Initialize Maximum value to height encoder.
}

