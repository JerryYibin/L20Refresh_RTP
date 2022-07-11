/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "PreReady.h"
#include "../HeightEncoder.h"
PreReady::PreReady() {
	m_Actions = SCState::INIT;
	m_State = SCState::PRE_READY;
}

PreReady::~PreReady() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

void PreReady::Init()
{
	m_Timeout = 0;
	m_Actions = SCState::LOOP;
	m_Pressure.DAC_Pressure = Utility::Pressure2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure); 
	DAC_TLV5604::SetAmplitude(Utility::Amplitude2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude));
	if(_objDCan->Sending(&m_Pressure) == ERROR)
		m_Actions = SCState::FAIL;
//	LOG("GetInitCount = %d\n", HeightEncoder::GetInitCount());
//	LOG("GetMaxCount = %d\n", HeightEncoder::GetMaxCount());
	LOG("GetPositionCount = %d\n", HeightEncoder::GetPositionCount());
//	LOG("GetDirection = %d\n", HeightEncoder::GetDirection());
}

void PreReady::Loop()
{
	if (m_Timeout < TOP_CHECK_TIMEOUT)
	{
		if ((m_Timeout % 10) == 0)
		{
			if (/*SysConfig.m_SystemInfo.HomePosition*/ MAX_HEIGHT > HeightEncoder::GetPositionCount())
			{
				if ((/*SysConfig.m_SystemInfo.HomePosition*/MAX_HEIGHT - HeightEncoder::GetPositionCount()) < (TWICE * RDCHECK))
				{
					LOG("#########################################Waiting horn m_Timeout = %d\n", m_Timeout);
					m_Actions = SCState::JUMP;
				}
			}
			else
			{
				if ((HeightEncoder::GetPositionCount() - MAX_HEIGHT/*SysConfig.m_SystemInfo.HomePosition*/) < (TWICE * RDCHECK))
				{
					LOG("###################################Waiting horn m_Timeout = %d\n", m_Timeout);
					m_Actions = SCState::JUMP;
				}

			}
		}
		m_Timeout++;
	}
	else
	{
		CommonProperty::WeldResult.ALARMS.AlarmFlags.HeightSystemFailure = 1;
		vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
		vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
		//TODO Record Database alarm table
		m_Timeout = 0;
		m_Actions = SCState::FAIL;
	}
}

void PreReady::Fail()
{
	if (ProcessAlarmHandler() == true)
		m_Actions = SCState::ABORT;
	LOG("Pre-Ready Alarm process!\n");
}
