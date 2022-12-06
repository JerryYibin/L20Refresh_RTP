/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 SC Welding State
 
***************************************************************************/

#include "WeldSonicOn.h"
#include "SCStateMachine.h"
#include "../Recipe.h"
#include "../PCStateMachine.h"
#include "../ACStateMachine.h"
#include "../UserInterface.h"
#include "../WeldResultSignature.h"
#include "../ControlTask.h"
#include "../Utility.h"

unsigned int WeldSonicOn::PwrBuffer[PWR_SIZE] = {0};
unsigned int WeldSonicOn::PwrIndex = 0;

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of WeldSonicOn.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
WeldSonicOn::WeldSonicOn() 
{
	m_Actions = SCState::INIT;
	m_State = SCState::WELD_SONIC_ON;
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
WeldSonicOn::~WeldSonicOn() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Weld Sonic On Enter. Initialize variables
* 			 Reset bit CTRL_PC_SONIC_DISABLE of MasterEvents. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Enter()
{
	PCStateMachine::PC_RX->MasterEvents &= ~ BIT_MASK(PCState::CTRL_PC_SONIC_DISABLE);
	memset(PwrBuffer, 0, sizeof(PwrBuffer));
	PwrIndex = 0;
	m_WeldTime = 0;
	m_PeakPower = 0;
	m_EnergyAccumulator = 0;
	m_Timeout = 0;
	unsigned int m_EnergySetting;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_SETTING,&m_EnergySetting);
	m_EnergyTarget = Utility::Energy2HEX(m_EnergySetting);
	WeldResultSignature::_OrignalSignature->clear();
	ClearWeldData();
	//CommonProperty::WeldResult.PreHeight = ACStateMachine::AC_TX->ActualHeight;
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, &ACStateMachine::AC_TX->ActualHeight);
	//TODO Need to consider to move cooling control to the specific actuator task.
	CoolAirControl(0, 0);
//	if (SysConfig.m_SystemInfo.HeightEncoder == true)
//	{
//		if ((WeldData.m_WeldResult.PreHeight < ActivePreset.m_WeldParam.Height.min) && (ActivePreset.m_WeldParam.Height.min != 0))
//		{
//			WeldData.m_WeldResult.ALARMS.AlarmFlags.PreHeightMin = 1;
//			WeldData.m_WeldResult.ALARMS.AlarmFlags.PreHeightMax = 0;
//			m_Actions = SCState::FAIL;
//		}
//		else if (WeldData.m_WeldResult.PreHeight > ActivePreset.m_WeldParam.Height.max)
//		{
//			WeldData.m_WeldResult.ALARMS.AlarmFlags.PreHeightMin = 0;
//			WeldData.m_WeldResult.ALARMS.AlarmFlags.PreHeightMax = 1;
//			m_Actions = SCState::FAIL;
//		}
//	}


	PCStateMachine::PC_RX->MasterState = SCState::WELD_SONIC_ON;
	
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_SONICS_SET);
}

/**************************************************************************//**
*
* \brief   - Weld Sonic On Loop for weld signature data sampling and weld process control 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Loop()
{
	bool ErrorOn = false;
	unsigned int tmpHeight = 0;
	unsigned int tmpPower = 0;
	shared_ptr<WeldResultSignatureDefine> _Signature = WeldResultSignatureDefine::GetWeldSignature();
	WELDMODE m_WeldMode = INVALID_MODE; 
	int m_TimeMin = 0;

	m_WeldTime += 1;
	tmpPower = GetFilteredPower();
	_Signature->Set(WeldResultSignatureDefine::POWER, tmpPower);
	if (tmpPower > m_PeakPower)
		m_PeakPower = tmpPower;
	m_EnergyAccumulator += tmpPower;
		
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		//CommonProperty::WeldResult.ALARMS.AlarmFlags.Overload = 1;
		int AlarmFlags = 1;
		WeldResults::_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, &AlarmFlags);
		m_Actions = SCState::FAIL;
	}

	_Signature->Set(WeldResultSignatureDefine::HEIGHT, ACStateMachine::AC_TX->ActualHeight);
	_Signature->Set(WeldResultSignatureDefine::FRQUENCY, PCStateMachine::PC_TX->Frequency);
	if (WeldResultSignature::_OrignalSignature->size() < ABSMAXTIME)
	{
		WeldResultSignature::_OrignalSignature->push_back(_Signature);
	}
		
	if (m_WeldTime > ABSMAXTIME)
	{
		m_Actions = SCState::JUMP;
	}
	else
	{
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WELD_MODE, &m_WeldMode);
		switch (m_WeldMode)
		{
		case ENERGY_MODE:
			if((m_EnergyAccumulator > m_EnergyTarget) || (m_WeldTime > ABSMAXTIME))
				m_Actions = SCState::JUMP;
			break;
		case TIME_MODE:
			Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &m_TimeMin);
			if (m_WeldTime > m_TimeMin)
			{
				m_Actions = SCState::JUMP;
			}
			break;
		case HEIGHT_MODE:

			break;
		case ENERGY_HEIGHT_MODE:
			break;
		default:
			if(m_WeldTime > ABSMAXTIME)
				m_Actions = SCState::JUMP;
			break;
		}
	}
}

/**************************************************************************//**
*
* \brief   - Weld Sonic On Exit. Update weld result.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Exit()
{
	int TriggerPressure = 0;
	int WeldPressure = 0;
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
	WeldResults::_WeldResults->Energy = Utility::HEX2Energy(m_EnergyAccumulator);
	WeldResults::_WeldResults->WeldTime = m_WeldTime;
	//CommonProperty::WeldResult.PostHeight = ACStateMachine::AC_TX->ActualHeight;
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::POST_HEIGHT, &ACStateMachine::AC_TX->ActualHeight);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &TriggerPressure);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::TRIGGER_PRESSURE, &TriggerPressure);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WP_PRESSURE, &WeldPressure);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::WELD_PRESSURE, &WeldPressure);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::AMPLITUDE, &WeldResults::_WeldResults->Amplitude/*CommonProperty::WeldResult.Amplitude*/);
	//CommonProperty::WeldResult.PeakPower = Utility::HEX2Power(m_PeakPower);
	WeldResults::_WeldResults->PeakPower = Utility::HEX2Power(m_PeakPower);
	
//		if (SysConfig.m_SystemInfo.CoolingType == WelderSystem::SECOND_PER_100JOULE)
//		{
//			if (WeldData.m_WeldResult.Energy < JOULE500)
//			{
//				CoolAirControl(SysConfig.m_SystemInfo.CoolingDelay, DELAY5SEC);
//			}
//			else
//			{
//				CoolAirControl(SysConfig.m_SystemInfo.CoolingDelay, WeldData.m_WeldResult.Energy * 10);
//			}
//		}
//		else if(SysConfig.m_SystemInfo.CoolingType == WelderSystem::STANDARD)
//		{
//			CoolAirControl(SysConfig.m_SystemInfo.CoolingDelay, SysConfig.m_SystemInfo.CoolingDuration);
//		}
//		else
//		{
		CoolAirControl(0, 0);
//		}
	LOG("m_StepIndex: Weld Sonics Loop - Result Update! Weld Time = %d Timeout = %d\n", m_WeldTime, m_Timeout);
	SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_SC_RESPONSE);
	ChangeExtDgtOutput(ScDgtOutputTask::TO_DGT_OUTPUT_TASK_SONICS_RESET);

}

/**************************************************************************//**
*
* \brief   - Weld Sonic On Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Fail()
{
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
	//Record Database alarm table
	m_Actions = SCState::ALJUMPNORM;
}

/**************************************************************************//**
*
* \brief   - Clear Weld Data. Initialize weld result.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::ClearWeldData()
{
	if(WeldResults::_WeldResults->RecipeID != Recipe::ActiveRecipeSC->m_RecipeNumber)
		WeldResults::_WeldResults->RecipeID = 0;
	WeldResults::_WeldResults->Energy = 0;

	WeldResults::_WeldResults->WeldTime = 0;

	WeldResults::_WeldResults->PeakPower = 0;

	int PreHeight = 0;
	int PostHeight = 0;
	int WeldPressure = 0;
	int TriggerPressure = 0;
	int AlarmID = 0;
	
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::PRE_HEIGHT, &PreHeight);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::POST_HEIGHT, &PostHeight);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::WELD_PRESSURE, &WeldPressure);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::TRIGGER_PRESSURE, &TriggerPressure);
	WeldResults::_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, &AlarmID);
}

/**************************************************************************//**
* 
* \brief   - Get power average value.
*
* \param   - None.
*
* \return  - power average value.
*
******************************************************************************/
unsigned int WeldSonicOn::GetFilteredPower()
{
	unsigned int result;
	/* Keep a running average */
	PwrBuffer[PwrIndex] = PCStateMachine::PC_TX->Power;
	PwrIndex++;
	if (PwrIndex >= PWR_SIZE)
		PwrIndex = 0;

	result = Utility::Average((const int*)PwrBuffer, PWR_SIZE);
	
	return result;
}

/**************************************************************************//**
*
* \brief   - Cooling Control.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
//Todo Consider to use ACCoolAir State
void WeldSonicOn::CoolAirControl(unsigned int delay, unsigned duration)
{
	/* Delay and Duration in 1/100 second units*/
	if (duration == 0)
	{
		SCStateMachine::getInstance()->SetCoolingTimer(0, 0);
		//GPIOE_SetFieldValue(GPIOE, COOLINGON, OFF);
		vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
		return;
	}

	SCStateMachine::getInstance()->SetCoolingTimer(delay, duration);
	/* Turn it on if there is no delay */
	if (delay == 0)
	{
		vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_HIGH);
	}
	else /* Turn it off to insure delay */
	{
		vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
	}
}

