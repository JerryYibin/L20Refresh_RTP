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
	m_PBState = WAIT_PRESSED;
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
	m_PBState = WAIT_PRESSED;
	m_Timeout = 0;
	ACStateMachine::AC_TX->AC_StatusEvent &= ~BIT_MASK(STATUS_START_SWITCH_PRESSED);
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
	if(ACStateMachine::AC_RX->MasterState == ACState::AC_READY)
	{
		ChangeState(ACState::AC_READY);
		return;
	}
	switch(m_PBState)
	{
	case WAIT_PRESSED:
		//TODO still need to add input signals checking
		if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_LOW && vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_LOW) // food pedal should be pressed status.
		{
			m_PBState = HOLD_PRESSED_DEBOUNCE;
			m_Timeout = 0;
		}
		break;
	case HOLD_PRESSED_DEBOUNCE:
		if(m_Timeout < DELAY50MSEC)
			m_Timeout++;
		else
		{
			m_Timeout = 0;
			m_PBState = STILL_PRESSED;
		}
		break;
	case STILL_PRESSED:
		if (m_Timeout < DELAY250MSEC)
		{
			//TODO still need to add input signals checking
			if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_LOW && vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_LOW)
			{
				ACStateMachine::AC_TX->AC_StatusEvent |= BIT_MASK(STATUS_START_SWITCH_PRESSED);
				ChangeState(AC_READY);
			}
			else
				m_Timeout++;
		}
		else
		{
			ActuatorTask::SetCoreState(ERR_STARTSWITCH_LOST);
			ChangeState(AC_ALARM);
		}
		break;
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
