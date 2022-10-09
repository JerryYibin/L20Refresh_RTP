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
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStartSwitch::Enter()
{
	m_Timeout = 0;
	if((ACStateMachine::AC_TX->ACInputs & SS1MASK) == SS1MASK)
		m_PBIndex = SS1MASK;
	else if((ACStateMachine::AC_TX->ACInputs & SS2MASK) == SS2MASK)
		m_PBIndex = SS2MASK;
	else
		m_PBIndex = 0;
	
}

/**************************************************************************//**
* 
* \brief   - ACT Start Switch Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACStartSwitch::Loop()
{
	unsigned int coreStatus = NO_ERROR;
	if(ACStateMachine::AC_RX->MasterState != ACState::AC_READY)
	{
		ChangeState(AC_READY);
	}
	else if(m_PBIndex == 0)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
	else if(m_Timeout > DELAY250MSEC)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
	else if((ACStateMachine::AC_TX->ACInputs & BOTHSTARTSWITCHMASK) != m_PBIndex)
	{
		ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
	}
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
