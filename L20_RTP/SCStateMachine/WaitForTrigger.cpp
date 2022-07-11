/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "WaitForTrigger.h"
#include "Utils.h"

WaitForTrigger::WaitForTrigger() {
	m_Actions = SCState::INIT;
	m_State = SCState::WAIT_FOR_TRIGGER;
	m_Timeout = 0;
}

WaitForTrigger::~WaitForTrigger() {
	m_Actions = SCState::INIT;
}

void WaitForTrigger::Init()
{
	//TODO still need to add output signal handler
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_HIGH);
//	if (SysConfig.m_SystemInfo.HeightEncoder == false)
		m_Actions = SCState::JUMP;
//	else
//	{
//		m_Timeout = 0;
//		m_TriggerHeight = SysConfig.GetHeightRead() - 50;
//		m_HornState = WaitForTrigger::WAIT_MOVE;
//		m_Actions = SCState::LOOP;
//		cout << m_StepIndex << ": " << "Trigger Initialization!" << endl;
//	}
}

void WaitForTrigger::Loop()
{
	unsigned int tmpSpeed, tmpHeight;
	if (m_Timeout % 5 == 0)
	{
		if (DefeatWeldAbortHandler() == true)
		{
			m_Actions = SCState::ABORT;
			return;
		}
	}
	switch(m_HornState)
	{
	case WaitForTrigger::WAIT_MOVE:
		if (m_Timeout < DELAY6SEC)
		{
			if (m_Timeout % 5 == 0)
			{
//				if ((unsigned int)SysConfig.GetHeightRead() < m_TriggerHeight)
//				{
//					m_StartHeight = SysConfig.GetHeightRead();
					m_MaxSpeed = 0;
					m_MinMoveCount = 20;
					m_HornState = WaitForTrigger::WAIT_TOUCH;
//					cout << m_StepIndex << ": " << "Trigger Loop - Wait Move! " << "Timeout: " << m_Timeout << endl;
//				}
			}
		}
		else
			m_Actions = SCState::ABORT;
		break;
	case WaitForTrigger::WAIT_TOUCH:
		if (m_Timeout < DELAY6SEC)
		{
			if (m_Timeout % 5 == 0)
			{
//				tmpHeight = SysConfig.GetHeightRead();
				tmpSpeed = AverageSpeed(tmpHeight);
				if (tmpSpeed > m_MaxSpeed)
					m_MaxSpeed = tmpSpeed;
				if (m_MinMoveCount != 0)
					m_MinMoveCount--;
				else
				{
					if (tmpSpeed <= MINHORNDOWNSPEED)
					{
						m_Actions = SCState::JUMP;
//						cout << m_StepIndex << ": " << "Trigger Loop - Wait Touch! " << "Timeout: " << m_Timeout << endl;
					}
					else
						m_MinMoveCount = 20;
				}
			}

		}
		else
			m_Actions = SCState::ABORT;
		break;
	default:
		m_Actions = SCState::ABORT;
		break;
	}
	m_Timeout++;
}

void WaitForTrigger::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Trigger Alarm happened!\n");
}

unsigned int WaitForTrigger::AverageSpeed(unsigned int EncoderPosition)
{
	static int OldPosition = 0;
	static int DeltaPosition[4] = {0};
	int tmpSpeed = 0;
	DeltaPosition[0] = DeltaPosition[1];
	DeltaPosition[1] = DeltaPosition[2];
	DeltaPosition[2] = DeltaPosition[3];
	DeltaPosition[3] = EncoderPosition - OldPosition;
	OldPosition = EncoderPosition;
	tmpSpeed = (DeltaPosition[3] * 30 + DeltaPosition[2] * 30 + DeltaPosition[1] * 20 + DeltaPosition[0] * 20); //100 total
	tmpSpeed = _ABS(tmpSpeed);
	return tmpSpeed;
}
