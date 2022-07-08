/*
 * Seek.cpp
 *
 *  Created on: May 26, 2022
 *      Author: JerryW.Wang
 */

#include "Seek.h"

Seek::Seek() {
	m_Actions = SCState::INIT;
	m_Timeout = 0;
}

Seek::~Seek() {
	m_Actions = SCState::INIT;
}

void Seek::Init()
{
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_HIGH);
	m_Actions = SCState::LOOP;
	m_Timeout = 0;
}

void Seek::Loop()
{
	if (m_Timeout < DELAY25MSEC)
	{
		if (m_Timeout % 5 == 0)
		{
			if (DefeatWeldAbortHandler() == true)
				m_Actions = SCState::ABORT;
		}
		m_Timeout++;
	}
	else
	{
		//TODO still need to add output signal hander
		vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_LOW);
		m_Actions = SCState::JUMP;
		LOG("m_StepIndex: Seek Loop Running! Timeout: %d\n", m_Timeout);
	}
}

void Seek::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Seek Alarm happened!\n");
}
