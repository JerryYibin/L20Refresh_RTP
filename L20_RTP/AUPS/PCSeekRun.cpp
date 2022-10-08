/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCSeekRun.h"
#include "../PCStateMachine.h"
#include "../SCStateMachine/SCState.h"
#include "../PowerSupplyTask.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PCSeekRun::PCSeekRun() 
{
	stateType = PC_SEEK_RUN;
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
PCSeekRun::~PCSeekRun() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - AUPS Seek.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCSeekRun::Enter()
{	
	if ((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) == 0)
	{		
		// additional control bytes or settings to the SM here...
		
		// turn on sonics
		vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_HIGH);
	}
	m_Timeout = 0;
}

/**************************************************************************//**
* 
* \brief   - AUPS Seek.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCSeekRun::Loop()
{
	if(PowerSupplyTask::GetCoreState() != NO_ERROR)
	{
		// Change State to PC_ALARM here..
		ChangeState(PC_ALARM);
	}
	else
	{	
		if (PCStateMachine::PC_RX->MasterState != SCState::SEEK_SONIC_ON)
		{
			ChangeState(PC_READY);
		}
		else
		{
			// do more while seeking here...
		}
	}
	m_Timeout++;
}

/**************************************************************************//**
* 
* \brief   - AUPS Seek.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCSeekRun::Exit()
{
	// check AUPS status
	if (PowerSupplyTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in AUPS
		PCStateMachine::PC_RX->MasterEvents |=  BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	}
	
	// turn off sonics
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_LOW);
	PowerSupplyTask::GetInstance()->CalculateStartFrequency();
	LOG("\n PowerSupply_T: _PC_SEEK_TIMEOUT = %d\n", m_Timeout);
}

