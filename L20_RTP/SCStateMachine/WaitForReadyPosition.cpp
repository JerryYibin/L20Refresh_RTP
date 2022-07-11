/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "WaitForReadyPosition.h"

WaitForReadyPosition::WaitForReadyPosition() {
	m_Actions = SCState::INIT;
	m_State = SCState::WAIT_FOR_READY_POSITION;
	m_Timeout = 0;
}

WaitForReadyPosition::~WaitForReadyPosition() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

void WaitForReadyPosition::Init()
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

void WaitForReadyPosition::Loop()
{
	//TODO still need to add output handler
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	m_Actions = SCState::JUMP;
}

void WaitForReadyPosition::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Release Part Alarm happened!\n");
}
