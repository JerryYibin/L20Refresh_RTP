/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used to define all the External Ethernet TCP related Data

***************************************************************************/
#include "ExternalDataTCP.h"
#include "Recipe.h"
#include "SystemConfiguration.h"
#include "WeldResultsDefine.h"
#include "WeldResultSignature.h"
extern "C"
{
	#include "customSystemCall.h"	
	#include "hwif/drv/resource/vxbRtcLib.h"
}

ExternalDataTCP* ExternalDataTCP::_objTCPData = nullptr;
/**************************************************************************//**
* \brief   - Constructor 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ExternalDataTCP::ExternalDataTCP() 
{
	// TODO Auto-generated constructor stub
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ExternalDataTCP::~ExternalDataTCP() 
{
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief   - Get the Ethernet Message Data according to the input type.
*
* \param   - int type, ETHMESSAGE* _data
*
* \return  - ok or not
*
******************************************************************************/

int ExternalDataTCP::BuildProtocolData(int type, char* _data)
{
	int len = NONE;
	if(_data == nullptr)
		return ERROR;
	switch(type)
	{
	case WELDDATA:
		WeldData welddata;
		welddata.CycleCount = WeldResults::_WeldResults->CycleCounter; 
		welddata.ActuatorType = L20REFRESHACTUATOR;
		SystemConfiguration::_SystemConfig->Get(SYSTEMCONFIG::POWER_OPT, &welddata.PowerSupply);
		WeldResults::_WeldResults->Get(WeldResults::PARALIST::ALARM_ID, &welddata.Alarm);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::AMPLITUDE, &welddata.AmplitudeSetting);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_SETTING, &welddata.EnergySetting);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &welddata.TimeSetting);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &welddata.TriggerPressureSetting);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WP_PRESSURE, &welddata.WeldPressureSetting);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WELD_MODE, &welddata.WeldModeSetting);
		if(welddata.WeldModeSetting != 0)
		{
			welddata.EnergySetting = NONE;
		}
		else
		{
			welddata.TimeSetting = NONE;
		}
		welddata.ActualWeldTime = WeldResults::_WeldResults->WeldTime;
		welddata.ActualWeldEnergy = WeldResults::_WeldResults->Energy;
		welddata.ActualPeakPower = WeldResults::_WeldResults->PeakPower;
		WeldResults::_WeldResults->Get(WeldResults::PARALIST::PRE_HEIGHT, &welddata.ActualPreHeight);
		WeldResults::_WeldResults->Get(WeldResults::PARALIST::POST_HEIGHT,&welddata.ActualPostHeight);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MAX, &welddata.QualityTimeUpper);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &welddata.QualityTimeLower);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MAX, &welddata.QualityPowerUpper);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MIN, &welddata.QualityPowerLower);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX, &welddata.QualityPreHeightUpper);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN, &welddata.QualityPreHeightLower);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MAX, &welddata.QualityPostHeightUpper);
		Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MIN, &welddata.QualityPostHeightLower);		
		memcpy(_data,&welddata, sizeof(WeldData));	
		len = sizeof(WeldData);
		break;
	case POWERCURVE:	
		WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_POWER);
		len = WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
		memcpy(_data, &WeldResultSignature::GetInstance()->L20PowerGraphCurvePointVector[0],len);
		break;
	case HEIGHTCURVE:	
		WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_HEIGHT);
		len = WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
		memcpy(_data, &WeldResultSignature::GetInstance()->L20HeightGraphCurvePointVector[0],len);
		break;
	case FREQUENCYCURVE:	
		WeldResultSignature::GetInstance()->SelectGraphCurvePointsHandler(GRAPH_FRQUENCY);
		len = WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector.size() * sizeof(GraphCurvePoint);
		memcpy(_data, &WeldResultSignature::GetInstance()->L20FrquencyGraphCurvePointVector[0],len);
		break;
	default:
		break;
	}

	return len;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
ExternalDataTCP* ExternalDataTCP::GetInstance()
{
	return (_objTCPData != nullptr) ? _objTCPData : (_objTCPData = new ExternalDataTCP());
}
