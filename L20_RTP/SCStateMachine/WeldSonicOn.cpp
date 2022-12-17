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
#include "../SystemConfiguration.h"
#include "../ActuatorTask.h"

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
	bool bHeightEncoder = true;
	int currentValue = 0;
	int upperLimit = 0;
	int lowerLimit = 0;
	unsigned int m_EnergySetting;
	
	SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::HGT_ENCODER, &bHeightEncoder);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PRE_HEIGHT_MIN, &lowerLimit);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PRE_HEIGHT_MAX, &upperLimit);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::ENERGY_SETTING,&m_EnergySetting);
	m_EnergyTarget = Utility::Energy2HEX(m_EnergySetting);
	WeldResultSignature::_OrignalSignature->clear();
	ClearWeldData();
	WeldResults::_WeldResults->Set(WeldResults::PRE_HEIGHT, &ACStateMachine::AC_TX->ActualHeight);
	currentValue = ACStateMachine::AC_TX->ActualHeight;
	ActuatorTask::GetInstance()->ResetCoolingTimer();
	if (bHeightEncoder == true)
	{
		if ((currentValue < lowerLimit) && (lowerLimit != 0))
		{
			SCStateMachine::getInstance()->SetCoreState(ERR_PRE_HEIGHT_MS);
			m_Actions = SCState::FAIL;
			return;
		}
		else if (currentValue > upperLimit)
		{
			SCStateMachine::getInstance()->SetCoreState(ERR_PRE_HEIGHT_PL);
			m_Actions = SCState::FAIL;
			return;
		}
	}


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
	unsigned int actualPower = 0;
	unsigned int actualFrequency = 0;
	shared_ptr<WeldResultSignatureDefine> _Signature = WeldResultSignatureDefine::GetWeldSignature();
	WELDMODE m_WeldMode = INVALID_MODE; 
	int m_TimeMin = 0;
	int m_TimeMax = 0;

	m_WeldTime += 1;
	tmpPower = GetFilteredPower();
	actualPower = Utility::HEX2Power(tmpPower);
	_Signature->Set(WeldResultSignatureDefine::POWER, actualPower);
	if (tmpPower > m_PeakPower)
		m_PeakPower = tmpPower;
	m_EnergyAccumulator += tmpPower;
	
	_Signature->Set(WeldResultSignatureDefine::HEIGHT, ACStateMachine::AC_TX->ActualHeight);
	actualFrequency = Utility::HEX2Frequency(PCStateMachine::PC_TX->Frequency);
	_Signature->Set(WeldResultSignatureDefine::FRQUENCY, actualFrequency);
	if (WeldResultSignature::_OrignalSignature->size() < ABSMAXTIME)
	{
		WeldResultSignature::_OrignalSignature->push_back(_Signature);
	}
		

	if (m_WeldTime > ABSMAXTIME)
	{
		SCStateMachine::getInstance()->SetCoreState(ERR_TIME_PL);
		m_Actions = SCState::FAIL;
	}
	else
	{
		if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
		{
			SCStateMachine::getInstance()->SetCoreState(ERR_OVERLOAD);
			m_Actions = SCState::FAIL;
		}
		
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::WELD_MODE, &m_WeldMode);
		switch (m_WeldMode)
		{
		case ENERGY_MODE:
			Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TIME_MAX, &m_TimeMax);
			if((m_EnergyAccumulator > m_EnergyTarget) || (m_WeldTime > m_TimeMax))
				m_Actions = SCState::JUMP;
			break;
		case TIME_MODE:
			Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TIME_MIN, &m_TimeMin);
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
	unsigned int AlarmFlags = 0;
	PCStateMachine::PC_RX->MasterState = SCState::NO_STATE;
	WeldResults::_WeldResults->Energy = Utility::HEX2Energy(m_EnergyAccumulator);
	WeldResults::_WeldResults->WeldTime = m_WeldTime;
	WeldResults::_WeldResults->Set(WeldResults::POST_HEIGHT, &ACStateMachine::AC_TX->ActualHeight);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TP_PRESSURE, &TriggerPressure);
	WeldResults::_WeldResults->Set(WeldResults::TRIGGER_PRESSURE, &TriggerPressure);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::WP_PRESSURE, &WeldPressure);
	WeldResults::_WeldResults->Set(WeldResults::WELD_PRESSURE, &WeldPressure);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::AMPLITUDE, &WeldResults::_WeldResults->Amplitude);
	WeldResults::_WeldResults->PeakPower = Utility::HEX2Power(m_PeakPower);
	CheckWeldAlarm();
	AlarmFlags = SCStateMachine::getInstance()->GetCoreState();
	WeldResults::_WeldResults->Set(WeldResults::ALARM_ID, &AlarmFlags);
	
	ActuatorTask::GetInstance()->SetCoolingTimer();
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
	if(WeldResults::_WeldResults->RecipeID != Recipe::ActiveRecipeSC->m_RecipeID)
		WeldResults::_WeldResults->RecipeID = Recipe::ActiveRecipeSC->m_RecipeID;
	WeldResults::_WeldResults->Energy = 0;

	WeldResults::_WeldResults->WeldTime = 0;

	WeldResults::_WeldResults->PeakPower = 0;

	int PreHeight = 0;
	int PostHeight = 0;
	int WeldPressure = 0;
	int TriggerPressure = 0;
	int AlarmID = 0;
	
	WeldResults::_WeldResults->Set(WeldResults::PRE_HEIGHT, &PreHeight);
	WeldResults::_WeldResults->Set(WeldResults::POST_HEIGHT, &PostHeight);
	WeldResults::_WeldResults->Set(WeldResults::WELD_PRESSURE, &WeldPressure);
	WeldResults::_WeldResults->Set(WeldResults::TRIGGER_PRESSURE, &TriggerPressure);
	WeldResults::_WeldResults->Set(WeldResults::ALARM_ID, &AlarmID);
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

void WeldSonicOn::CheckWeldAlarm()
{
	bool bHeightEncoder = true;
	int currentValue = 0;
	int upperLimit = 0;
	int lowerLimit = 0;
	
	SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::HGT_ENCODER, &bHeightEncoder);
	if(bHeightEncoder == true) 
	{
		WeldResults::_WeldResults->Get(WeldResults::POST_HEIGHT, &currentValue);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::HEIGHT_MIN, &lowerLimit);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::HEIGHT_MAX, &upperLimit);
		
		if((currentValue < lowerLimit) && (lowerLimit != 0))
		{
			SCStateMachine::getInstance()->SetCoreState(ERR_POST_HEIGHT_MS);
		}
		else if(currentValue > upperLimit)
		{
			SCStateMachine::getInstance()->SetCoreState(ERR_POST_HEIGHT_PL);
		}
	}
	
	currentValue = WeldResults::_WeldResults->WeldTime;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TIME_MIN, &lowerLimit);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::TIME_MAX, &upperLimit);
	if(currentValue < lowerLimit)
	{
		SCStateMachine::getInstance()->SetCoreState(ERR_TIME_MS);
	}
	else if(currentValue > upperLimit)
	{
		SCStateMachine::getInstance()->SetCoreState(ERR_TIME_PL);
	}
	
	currentValue = WeldResults::_WeldResults->PeakPower;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PEAK_POWER_MIN, &lowerLimit);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PEAK_POWER_MAX, &upperLimit);
	if(currentValue < lowerLimit)
	{
		SCStateMachine::getInstance()->SetCoreState(ERR_POWER_MS);
	}
	else if(currentValue > upperLimit)
	{
		SCStateMachine::getInstance()->SetCoreState(ERR_POWER_PL);
	}
}
