/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCEstop.h"
#include "../PCStateMachine.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PCEstop::PCEstop() {
	stateType = PC_ESTOP;
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
PCEstop::~PCEstop() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - AUPS Estop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCEstop::Enter()
{
	// turn off sonics
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_TEST_PSI, GPIO_VALUE_LOW);
	PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(CTRL_PC_SONIC_DISABLE);
	PCStateMachine::PC_RX->TargetAmplitude = 0;
}

/**************************************************************************//**
* 
* \brief   - AUPS Estop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCEstop::Loop()
{
	if((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_ESTOP)) != BIT_MASK(CTRL_ESTOP))
	{
		// take actions here...
		ChangeState(PC_READY);
	}
	else
	{
		// do nothing while in E-stop
	}
}

/**************************************************************************//**
* 
* \brief   - AUPS Estop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCEstop::Exit()
{
	// take actions here...
}

