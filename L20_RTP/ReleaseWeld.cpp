/*
 * ReleaseWeld.cpp
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#include "ReleaseWeld.h"

ReleaseWeld::ReleaseWeld() {
	m_Actions = SCState::INIT;
	m_State = SCState::RELEASE_PART;
	m_Timeout = 0;
}

ReleaseWeld::~ReleaseWeld() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

void ReleaseWeld::Init()
{
//	if (SysConfig.m_SystemInfo.LockOnAlarm == false)
//	{
//		m_Actions = SCState::LOOP;
//		cout << m_StepIndex << ": " << "Release Part Initialization!" << endl;
//	}
//	else
//	{
		if (ProcessAlarmHandler() == true)
		{
			m_Actions = SCState::LOOP;
		}
//	}
}

void ReleaseWeld::Loop()
{
	//TODO still need to add output handler
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	m_Actions = SCState::JUMP;
}

void ReleaseWeld::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Release Part Alarm happened!\n");
}
