/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#include "WeldSonicOn.h"
#include "SCStateMachine.h"
#include "../SocketReceiverCAN.h"

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

void WeldSonicOn::Init()
{
	ClearWeldData();
	//TODO still need to add hardware signals output handler 
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
	unsigned int tmpHeight = SocketReceiver_CAN::GetHeight();
	CommonProperty::WeldResult.PreHeight = Utility::HEX2Height(tmpHeight);
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
//		else
//			m_Actions = SCState::LOOP;
//	}
//	else
		m_Actions = SCState::LOOP;

	if (m_Actions == SCState::LOOP)
	{
		m_Pressure.DAC_Pressure = Utility::Pressure2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WPpressure); 
		_objDCan->Sending(&m_Pressure);
		if (CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure > CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WPpressure)
		{
			m_Timeout = 0;
			m_WeldStep = WeldSonicOn::SET_WPRESSURE;
		}
		else
		{
			InitPowerBuffer();
			m_WeldTime = 0;
			m_PeakPower = 0;
			m_EnergyAccumulator = 0;
			m_EnergyTarget = Utility::Energy2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergySetting);
			CommonProperty::WeldSignatureVector.clear();

			//TODO still need to add some output handler.
			vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_HIGH);
			vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_HIGH);
			m_WeldStep = WeldSonicOn::RUN_SONICS;
		}
	}
}

void WeldSonicOn::Loop()
{
	bool ErrorOn = false;
	unsigned int tmpHeight = 0;
	CommonProperty::WELD_SIGNATURE tmpWeldSignature;
	switch (m_WeldStep)
	{
	case WeldSonicOn::SET_WPRESSURE:
		if (m_Timeout < DELAY500MSEC)
		{
			if (CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_PreBurst == 0)
			{
				if (m_Timeout % 5 == 0)
				{
					if (DefeatWeldAbortHandler() == true)
						m_Actions = SCState::ABORT;
				}
			}
		}
		else
		{
			InitPowerBuffer();
			m_WeldTime = 0;
			m_PeakPower = 0;
			m_EnergyAccumulator = 0;
			CommonProperty::WeldSignatureVector.clear();

			//TODO still need to add some output handler.
			vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_HIGH);
			vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_HIGH);
			m_WeldStep = WeldSonicOn::RUN_SONICS;
			cout << m_StepIndex << ": " << "Weld Sonics Loop - Set Weld Pressure" << endl;
		}
		break;
	case WeldSonicOn::RUN_SONICS:
		m_WeldTime += 1;
		tmpWeldSignature.Power = GetFilteredPower();
		if (tmpWeldSignature.Power > m_PeakPower)
			m_PeakPower = tmpWeldSignature.Power;
		m_EnergyAccumulator += tmpWeldSignature.Power;
		//still need to add output handler.
		/* Inline check for overload but process after weld release */
		
		if(vxbGpioGetValue(GPIO::I_OL_PSOUI) == GPIO_VALUE_HIGH)
		{
			CommonProperty::WeldResult.ALARMS.AlarmFlags.Overload = 1;
			m_Actions = SCState::FAIL;
		}

		tmpWeldSignature.Height = SocketReceiver_CAN::GetHeight();
		tmpWeldSignature.Frquency = ADC_AD7689::GetFrequency();
		if (CommonProperty::WeldSignatureVector.size() < (GRAPHDATASIZE - 5))
		{
			CommonProperty::WeldSignatureVector.push_back(tmpWeldSignature);
		}
		
		if (m_WeldTime > ABSMAXTIME)
		{
			m_WeldStep = WeldSonicOn::RESULT_UPDATE;
		}
		else
		{
			switch (CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_WeldMode)
			{
			case ENERGY_MODE:
				if((m_EnergyAccumulator > m_EnergyTarget) || (m_WeldTime > ABSMAXTIME))
					m_WeldStep = WeldSonicOn::RESULT_UPDATE;
				break;
			case TIME_MODE:
				if (m_WeldTime > CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMin)
				{
					m_WeldStep = WeldSonicOn::RESULT_UPDATE;
				}
				break;
			case HEIGHT_MODE:
	
				break;
			case ENERGY_HEIGHT_MODE:
				break;
			default:
				if ((m_EnergyAccumulator > CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergySetting) || (m_WeldTime > ABSMAXTIME))
					m_WeldStep = WeldSonicOn::RESULT_UPDATE;
				break;
			}
		}
		break;
	case WeldSonicOn::RESULT_UPDATE:
		//TODO still need to add output signal handle.
		vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
		vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_LOW);
		CommonProperty::WeldResult.TotalEnergy = Utility::HEX2Energy(m_EnergyAccumulator);
		CommonProperty::WeldResult.WeldTime = m_WeldTime;
		tmpHeight = SocketReceiver_CAN::GetHeight();
		CommonProperty::WeldResult.PostHeight = Utility::HEX2Height(tmpHeight);
		m_Pressure.DAC_Pressure = Utility::Pressure2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure); 
		_objDCan->Sending(&m_Pressure);
		CommonProperty::WeldResult.TriggerPressure = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure;
		CommonProperty::WeldResult.WeldPressure = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WPpressure;
		CommonProperty::WeldResult.Amplitude = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude;

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
		LOG("m_StepIndex: Weld Sonics Loop - Result Updata! Timeout = %d\n", m_Timeout);
		m_Timeout = 0;
		m_WeldStep = WeldSonicOn::EXTEND_SAMPLE;
		break;
	case WeldSonicOn::EXTEND_SAMPLE:
		if (m_Timeout < 5)
		{
			tmpWeldSignature.Power = GetFilteredPower();
			if (tmpWeldSignature.Power > m_PeakPower)
				m_PeakPower = tmpWeldSignature.Power;
			tmpWeldSignature.Height = SocketReceiver_CAN::GetHeight();
			tmpWeldSignature.Frquency = ADC_AD7689::GetFrequency();
			if (CommonProperty::WeldSignatureVector.size() < (GRAPHDATASIZE - 5))
			{
				CommonProperty::WeldSignatureVector.push_back(tmpWeldSignature);
			}
		}
		else
		{
			CommonProperty::WeldResult.PeakPower = Utility::HEX2Power(m_PeakPower);
			m_WeldStep = WeldSonicOn::RESULT_ANALYSIS;
		}
		break;
	case WeldSonicOn::RESULT_ANALYSIS:
		//TODO need to move following code to the control task.
		CommonProperty::WeldResult.CycleCounter += 1;
//		if (SysConfig.m_SystemInfo.HeightEncoder == true)
//		{
//			if((CommonProperty::m_WeldResults.Get(WeldResults::POST_HEIGHT) < CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMin) && 
//					(CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMin != 0))
//			{
//				CommonProperty::m_WeldResults.SetAlarm(WeldResults::ALARM_HEIGHT_MIN);
//				CommonProperty::m_WeldResults.ResetAlarm(WeldResults::ALARM_HEIGHT_MAX);
//				ErrorOn = true;
//			}
//			else if((CommonProperty::m_WeldResults.Get(WeldResults::POST_HEIGHT) > CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMax))
//			{
//				CommonProperty::m_WeldResults.ResetAlarm(WeldResults::ALARM_HEIGHT_MIN);
//				CommonProperty::m_WeldResults.SetAlarm(WeldResults::ALARM_HEIGHT_MAX);
//				ErrorOn = true;
//			}
//		}

		if(CommonProperty::WeldResult.WeldTime < CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMin)
		{
			CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMin = 1;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMax = 0;
			ErrorOn = true;
		}
		else if(CommonProperty::WeldResult.WeldTime > CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMax)
		{
			CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMin = 0;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMax = 1;
			ErrorOn = true;
		}

		if(CommonProperty::WeldResult.PeakPower < CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMin)
		{
			CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMin = 1;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMax = 0;
			ErrorOn = true;
		}
		else if(CommonProperty::WeldResult.PeakPower > CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMax)
		{
			CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMin = 0;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMax = 1;
			ErrorOn = true;
		}

		///* Handle the maintenance counters */
		//TODO Handle the maintenance counters and need to consider if the following code can be moved to the control task
		//MaintCount[TIPCOUNT] += WD.WeldData.Energy;
		//MaintCount[ANVILCOUNT] += WD.WeldData.Energy;
		//MaintCount[GATHERCOUNT] += WD.WeldData.Energy;
		//MaintCount[GUIDECOUNT] += WD.WeldData.Energy;
		//MaintCount[HORNCOUNT]++;
		//MaintCount[CONVERTERCOUNT]++;
		//MaintCount[GENERATORCOUNT]++;
		//MaintCount[ACTUATORCOUNT]++;
		if (ErrorOn == false)
		{
			m_Actions = SCState::JUMP;
		}
		else
			m_Actions = SCState::FAIL;
		break;
	default:
		m_Actions = SCState::JUMP;
		break;
	}
	m_Timeout++;
}

void WeldSonicOn::Fail()
{
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
	//Record Database alarm table
	m_Actions = SCState::ALJUMPNORM;
}

void WeldSonicOn::ClearWeldData()
{
	if(CommonProperty::WeldResult.RecipeNum != CommonProperty::ActiveRecipeSC.m_RecipeNumber)
		CommonProperty::WeldResult.CycleCounter = 0;
	CommonProperty::WeldResult.TotalEnergy = 0;

	CommonProperty::WeldResult.ActualWidth = 0;

	CommonProperty::WeldResult.WeldTime = 0;

	CommonProperty::WeldResult.PeakPower = 0;

	CommonProperty::WeldResult.PreHeight = 0;

	CommonProperty::WeldResult.PostHeight = 0;

	CommonProperty::WeldResult.WeldPressure = 0;

	CommonProperty::WeldResult.TriggerPressure = 0;
	
	CommonProperty::WeldResult.ALARMS.AlarmFlags.Overload = 0;
}

/**************************************************************************//**
* 
* \brief   - Initialize power buffer.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::InitPowerBuffer()
{
	/* Clear FIR buffer and return one unfiltered value */
	for (int cnt = 0; cnt < PWR_SIZE; cnt++)
		PwrBuffer[cnt] = 0;
	PwrIndex = 0;
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

	unsigned int PwrSum;
	unsigned int result;

	/* Keep a running average */
	PwrBuffer[PwrIndex] = ADC_AD7689::GetPower();
	PwrIndex++;
	if (PwrIndex >= PWR_SIZE)
		PwrIndex = 0;

	PwrSum = 0;
	for (int cnt = 0; cnt < PWR_SIZE; cnt++)
		PwrSum += PwrBuffer[cnt];

	result = PwrSum / PWR_SIZE;
	return result;
}

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
