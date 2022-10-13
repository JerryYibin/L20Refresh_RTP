#include "ACStartSwitch.h"
#include "../ACStateMachine.h"
#include "../SCStateMachine/SCState.h"
#include "../ActuatorTask.h"
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ACStartSwitch::ACStartSwitch() {
	stateType = ACState::AC_STARTSWICH;
	m_Timeout = 0;
	m_PB1 = 0;
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
ACStartSwitch::~ACStartSwitch() {
	// TODO Auto-generated destructor stub
}



/**************************************************************************//**
* 
* \brief   - ACT Start Switch Enter. 
* 			 Need to record current pressed PB1 status using m_PBIndex.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStartSwitch::Enter()
{
	m_Timeout = 0;
	ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(CTRL_AC_MOVE_DISABLE);
	if((ACStateMachine::AC_TX->ACInputs & SS1MASK) == SS1MASK)
		m_PB1 = SS1MASK;
	else if((ACStateMachine::AC_TX->ACInputs & SS2MASK) == SS2MASK)
		m_PB1 = SS2MASK;
	else
		m_PB1 = 0;
	
}

/**************************************************************************//**
* 
* \brief   - ACT Start Switch Loop. If the code can entry to the Start Switch state, 
* 			 it means the PB1 has been already pressed, 
* 			 so the Start Switch is only responsible for the PB2 checking within 250ms. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStartSwitch::Loop()
{
	unsigned int coreStatus = NO_ERROR;
	// If there is not any start switch pressed at the first checking, the start switch lost alarm will be set.
	if(m_PB1 == 0)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
	// If it is timeout, the start switch lost alarm will be set.
	else if(m_Timeout > DELAY250MSEC)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
	// If the current start switches are not equal to PB1, it means the current is the noise. 
	else if((ACStateMachine::AC_TX->ACInputs & m_PB1) != m_PB1)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
	// If the PB2 is pressed with PB1, the state will entry to ready state.
	else if((ACStateMachine::AC_TX->ACInputs & BOTHSTARTSWITCHMASK) == BOTHSTARTSWITCHMASK)
	{
		ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_START_SWITCH_PRESSED);
		ChangeState(AC_READY);
	}
	else
	{
		m_Timeout++;
	}
}

/**************************************************************************//**
* 
* \brief   - ACT Start Switch Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStartSwitch::Exit()
{
	// check ACT status
	if (ActuatorTask::GetCoreState() == NO_ERROR)
	{
		// clear any alarms in ACT
		ACStateMachine::AC_RX->MasterEvents |=  BIT_MASK(CTRL_AC_CLEAR_ALARMS);
	}
}
