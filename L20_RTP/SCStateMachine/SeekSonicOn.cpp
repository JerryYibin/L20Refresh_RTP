/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "SeekSonicOn.h"

SeekSonicOn::SeekSonicOn() {
	m_Actions = SCState::INIT;
	m_State = SCState::SEEK_SONIC_ON;
	m_Timeout = 0;
}

SeekSonicOn::~SeekSonicOn() {
	m_Actions = SCState::INIT;
}

void SeekSonicOn::Init()
{
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_EXT_SEEK_PSI, GPIO_VALUE_HIGH);
	m_Actions = SCState::LOOP;
	m_Timeout = 0;
}

void SeekSonicOn::Loop()
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
		LOG("m_StepIndex: SeekSonicOn Loop Running! Timeout: %d\n", m_Timeout);
	}
}

void SeekSonicOn::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("SeekSonicOn Alarm happened!\n");
}
