/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCReady.h"
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
PCReady::PCReady() 
{
	stateType = PC_READY;
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
PCReady::~PCReady() 
{
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - AUPS Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCReady::Enter()
{
	// turn off sonics
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_TEST_PSI, GPIO_VALUE_LOW);
	PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(CTRL_PC_SONIC_DISABLE);
}

/**************************************************************************//**
* 
* \brief   - AUPS Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCReady::Loop()
{
	// run loop code here...
	if((PCStateMachine::PC_RX->MasterState == SCState::PRE_READY) && (PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_POWER_UP_RECIPE)) == BIT_MASK(CTRL_POWER_UP_RECIPE)) 
	{
		PCStateMachine::PC_TX->PC_StatusEvent |= BIT_MASK(STATUS_POWER_UP_RECIPE);
	}
	else
	{
		PCStateMachine::PC_TX->PC_StatusEvent &= ~BIT_MASK(STATUS_POWER_UP_RECIPE);
	}

	if (((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_ESTOP)) != BIT_MASK(CTRL_ESTOP)))
	{
		// check AUPS status
		if (PowerSupplyTask::GetCoreState() != NO_ERROR)
		{
	    	// Change State to PC_ALARM here..
	    	ChangeState(PC_ALARM);
		}
		else if (PCStateMachine::PC_RX->MasterState == SCState::WELD_SONIC_ON)
		{
			ChangeState(PC_WELD_RUN);
		}
		else if (PCStateMachine::PC_RX->MasterState == SCState::SEEK_SONIC_ON)
		{
			// change state to SEEK_RUN here...
			ChangeState(PC_SEEK_RUN);
		}
		// Horn down with pre-burst
		else if(PCStateMachine::PC_RX->MasterState == SCState::WAIT_FOR_TRIGGER) 
		{
			if((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_PREBURST_ENABLE)) == BIT_MASK(CTRL_PREBURST_ENABLE))
			{
				PCStateMachine::PC_TX->PC_StatusEvent |= BIT_MASK(STATUS_PREBURST_ENABLE);
				ChangeState(PC_WELD_RUN);
			} 
			else
			{
				PCStateMachine::PC_TX->PC_StatusEvent &= ~BIT_MASK(STATUS_PREBURST_ENABLE);	
			}
		}
		// Horn up with after burst
		else if(PCStateMachine::PC_RX->MasterState == SCState::WAIT_FOR_READY_POSITION) 
		{
			// in case after-burst is implemented
			if((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_AFTERBURST_ENABLE)) == BIT_MASK(CTRL_AFTERBURST_ENABLE))
			{
				PCStateMachine::PC_TX->PC_StatusEvent |= BIT_MASK(STATUS_AFTERBURST_ENABLE);
				ChangeState(PC_WELD_RUN);
			} 
			else
			{
				PCStateMachine::PC_TX->PC_StatusEvent &= ~BIT_MASK(STATUS_AFTERBURST_ENABLE);	
			}
		}
	}
}

/**************************************************************************//**
* 
* \brief   - AUPS Ready.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCReady::Exit()
{
	// check AUPS status
	if (PowerSupplyTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms from AUPS
		PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	}
}

