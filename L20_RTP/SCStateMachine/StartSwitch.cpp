/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "StartSwitch.h"

StartSwitch::StartSwitch() {
	m_Actions = SCState::INIT;
	m_State = SCState::START_SWITCH;
	m_Timeout = 0;
}

StartSwitch::~StartSwitch() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

void StartSwitch::Init()
{
	//TODO  still need to add input signals checking
	if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH && vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH) // food pedal should be release status.
	{
		m_PBState = StartSwitch::WAIT_PRESSED;
		m_Actions = SCState::LOOP;
	}
}

void StartSwitch::Loop()
{
	switch (m_PBState)
	{
	case StartSwitch::WAIT_PRESSED:
		//TODO still need to add input signals checking
		if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_LOW && vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_LOW) // food pedal should be pressed status.
		{
			m_PBState = StartSwitch::HOLD_PRESSED_DEBOUNCE;
			m_Timeout = 0;
		}
		break;
	case StartSwitch::HOLD_PRESSED_DEBOUNCE:
		if (m_Timeout < DELAY50MSEC)
			m_Timeout++;
		else
		{
			m_Timeout = 0;
			m_PBState = StartSwitch::STILL_PRESSED;
		}
		break;
	case StartSwitch::STILL_PRESSED:
		//TODO still need to add input signals checking
		if (m_Timeout < DELAY250MSEC)
		{
			if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_LOW && vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_LOW)
			{
				m_Actions = SCState::JUMP;
			}
			else if (vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH || vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
			{
				m_Actions = SCState::FAIL;
			}
			else
				m_Timeout++;
		}
		else
			m_Actions = SCState::FAIL;
		break;
	default:
		m_Actions = SCState::FAIL;
		break;
	}
}

void StartSwitch::Fail()
{
	m_Actions = SCState::PUSH;
	LOG("Start Switch Alarm happened!\n");
}
