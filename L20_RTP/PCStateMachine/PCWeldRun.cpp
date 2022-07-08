/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PCWeldRun.h"
#include "PCStateMachine.h"
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
PCWeldRun::PCWeldRun() 
{
	stateType = PC_WELD_RUN;
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
PCWeldRun::~PCWeldRun() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - AUPS Weld.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCWeldRun::Enter()
{
	// covers WELD_SONIC_ON, WAIT_FOR_TRIGGER (pre-trigger) and WAIT_FOR_READY_POSITION (after-burst)
	// during which the AUPS core should turn sonics on.
	if ((PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) == 0)
	{
		
		// additional control bytes or settings (gain, integral, factors etc.) to the SM here...
		
		// turn on sonics
		vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_HIGH);
	}
}

/**************************************************************************//**
* 
* \brief   - AUPS Weld.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCWeldRun::Loop()
{
    if(PowerSupplyTask::GetCoreState() != NO_ERROR)
	{
    	// Change State to PC_ALARM here..
    	ChangeState(PC_ALARM);
	}
    else
    {
    	if ((PCStateMachine::PC_RX->MasterState == SCState::HOLD) || (PCStateMachine::PC_RX->MasterState == SCState::PRE_READY)
    			|| ((PCStateMachine::PC_RX->MasterState == SCState::WAIT_FOR_READY_POSITION) 
    					&& (PCStateMachine::PC_RX->MasterEvents & BIT_MASK(CTRL_AFTERBURST_ENABLE)) != BIT_MASK(CTRL_AFTERBURST_ENABLE)))
		{
			ChangeState(PC_READY);
		}
		else if ((PCStateMachine::PC_RX->MasterState != SCState::WELD_SONIC_ON)
				&& ((PCStateMachine::PC_RX->MasterState != SCState::WAIT_FOR_TRIGGER) 
						&& (PCStateMachine::PC_TX->PC_StatusEvent & BIT_MASK(STATUS_PREBURST_ENABLE)) == BIT_MASK(STATUS_PREBURST_ENABLE))
					&& ((PCStateMachine::PC_RX->MasterState != SCState::WAIT_FOR_READY_POSITION)
						&& (PCStateMachine::PC_TX->PC_StatusEvent & BIT_MASK(STATUS_AFTERBURST_ENABLE)) == BIT_MASK(STATUS_AFTERBURST_ENABLE))) 		
		{			  	
			// Change State to PC_ALARM here..
			ChangeState(PC_ALARM);
		}
		else
		{
			// do more while welding here...
		}
    }
}

/**************************************************************************//**
* 
* \brief   - AUPS Weld.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void PCWeldRun::Exit()
{
	// check AUPS status
	if (PowerSupplyTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in AUPS
		PCStateMachine::PC_RX->MasterEvents |=  BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	}
	
	// turn off sonics
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	PCStateMachine::PC_RX->TargetAmplitude = 0;
}



