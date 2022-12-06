/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     The class is weld result handling for each weld finished.
 
**********************************************************************************************************/

#include "WeldWorkFlow.h"
#include "CommonProperty.h"
#include "Recipe.h"
#include "SystemConfiguration.h"
/**************************************************************************//**
* \brief   - Constructor - Set default operation as BATCH
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
WeldWorkFlow::WeldWorkFlow() {
	m_OperationMode = BATCH;

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
WeldWorkFlow::~WeldWorkFlow() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Initialize Weld process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldWorkFlow::InitProcess(void)
{
	//TODO There is something needs to be added for weld process initialization. 
}

/**************************************************************************//**
* 
* \brief   - Trigger weld process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldWorkFlow::TriggerProcess(void)
{
}

/**************************************************************************//**
* 
* \brief   - Run Weld Process. If the weld process run under the BATCH, 
* the cycle number need to be incremental after each weld finished. 
*
* \param   - None.
*
* \return  - current work flow state.
*
******************************************************************************/
int WeldWorkFlow::RunProcess(void)
{
	STATE state = INVALID;
	unsigned int batchCount;
	if(m_OperationMode == SETUP)
		state =  FINISH;
	else
	{
		WeldResults::_WeldResults->CycleCounter += 1;
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::BATCH_SIZE, &batchCount);
		if(WeldResults::_WeldResults->CycleCounter >= batchCount)
			state = FINISH;
		else
			state = ONGOING;
	}
	return state;
}

/**************************************************************************//**
* 
* \brief   - Update Weld result. Handle with SC alarm process and record maintenance counter. 
*
* \param   - None.
*
* \return  - 0
*
******************************************************************************/
int WeldWorkFlow::UpdateResult(void)
{
	bool bHeightEncoder = true;
	CommonProperty::SystemInfo.psLifeCounter += 1;
	SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::HGT_ENCODER, &bHeightEncoder);
	if(bHeightEncoder == true)
	{
//		if((CommonProperty::m_WeldResults.Get(WeldResults::POST_HEIGHT) < CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMin) && 
//				(CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMin != 0))
//		{
//			CommonProperty::m_WeldResults.SetAlarm(WeldResults::ALARM_HEIGHT_MIN);
//			CommonProperty::m_WeldResults.ResetAlarm(WeldResults::ALARM_HEIGHT_MAX);
//			ErrorOn = true;
//		}
//		else if((CommonProperty::m_WeldResults.Get(WeldResults::POST_HEIGHT) > CommonProperty::m_RecpeSC.m_ActiveRecipe.m_QualityWindowSetting.m_HeightMax))
//		{
//			CommonProperty::m_WeldResults.ResetAlarm(WeldResults::ALARM_HEIGHT_MIN);
//			CommonProperty::m_WeldResults.SetAlarm(WeldResults::ALARM_HEIGHT_MAX);
//			ErrorOn = true;
//		}
	}
	
//	if(CommonProperty::WeldResult.WeldTime < CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMin)
//	{
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMin = 1;
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMax = 0;
//		ErrorOn = true;
//	}
//	else if(CommonProperty::WeldResult.WeldTime > CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMax)
//	{
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMin = 0;
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.TimeMax = 1;
//		ErrorOn = true;
//	}
//
//	if(CommonProperty::WeldResult.PeakPower < CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMin)
//	{
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMin = 1;
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMax = 0;
//		ErrorOn = true;
//	}
//	else if(CommonProperty::WeldResult.PeakPower > CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMax)
//	{
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMin = 0;
//		CommonProperty::WeldResult.ALARMS.AlarmFlags.PowerMax = 1;
//		ErrorOn = true;
//	}
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
	return 0;
}

void WeldWorkFlow::SetOperationMode(unsigned int operation)
{
	m_OperationMode = operation;
}

int WeldWorkFlow::GetOperationMode()
{
	return m_OperationMode;
}

