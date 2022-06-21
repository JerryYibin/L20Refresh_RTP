/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
 ---------------------------- MODULE DESCRIPTION ----------------------------

 
***************************************************************************/
#include "Common.h"
#include "WeldResults.h"

/**************************************************************************//**
* \brief  - Sets parameters at start up (needed for static initialization)
*
* \param  - All results parameters to initialize
*
* \return  - None
*
******************************************************************************/
WeldResults::WeldResults() 
{
	char name[] = "new";
	memcpy(m_WeldResult.RecipeName, name, sizeof(name));
	char partid[] = "0";
	memcpy(m_WeldResult.PartID, partid, sizeof(partid));
	m_WeldResult.CycleCounter = 0;
	m_WeldResult.RecipeNumber = 1;
	m_WeldResult.RecipeRevNumber = 0;
	m_WeldResult.WeldStatus = WeldResults::WELDDONE;
	m_WeldResult.TotalEnergy = 1000;
	m_WeldResult.ActualWidth = 0;
	m_WeldResult.Amplitude = 25;
	m_WeldResult.TriggerPressure = 80;
	m_WeldResult.WeldPressure = 80;
	m_WeldResult.WeldTime = 100;
	m_WeldResult.PeakPower = 2500;
	m_WeldResult.PreHeight = 1500;
	m_WeldResult.PostHeight = 1500;
	m_WeldResult.ALARMS.ALARMflags = 0;
}

/**************************************************************************//**
* 
* \brief   - Memory release of object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResults::~WeldResults() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief  - Gets any parameter from the weld results.
*
* \param  - idx - index of specific result to get
*
* \return  - Specific result's value
*
******************************************************************************/
UINT32 WeldResults::Get(const WELD_RESULT_IDX idx)
{
	UINT32 result = 0;
	switch(idx)
	{
	case WeldResults::CYCLE_COUNTER:
		result = m_WeldResult.CycleCounter;
		break;
	case WeldResults::RECIPE_NUM:
		result = m_WeldResult.RecipeNumber;
		break;
	case WeldResults::RECIPE_REV_NUM:
		result = m_WeldResult.RecipeRevNumber;
		break;
	case WeldResults::WELD_STATUS:
		result = m_WeldResult.WeldStatus;
		break;
	case WeldResults::TOTAL_ENERGY:
		result = m_WeldResult.TotalEnergy;
		break;
	case WeldResults::ACTUAL_WIDTH:
		result = m_WeldResult.ActualWidth;
		break;
	case WeldResults::ACTUAL_AMPLITUDE:
		result = m_WeldResult.Amplitude;
		break;
	case WeldResults::TRIGGER_PRESSURE:
		result = m_WeldResult.TriggerPressure;
		break;
	case WeldResults::WELD_PRESSURE:
		result = m_WeldResult.WeldPressure;
		break;
	case WeldResults::WELD_TIME:
		result = m_WeldResult.WeldTime;
		break;
	case WeldResults::PEAK_POWER:
		result = m_WeldResult.PeakPower;
		break;
	case WeldResults::PREHEIGHT:
		result = m_WeldResult.PreHeight;
		break;
	case WeldResults::POST_HEIGHT:
		result = m_WeldResult.PostHeight;
		break;
	case WeldResults::WELD_ALARM:
		result = m_WeldResult.ALARMS.ALARMflags;
		break;
	default:
		LOGERR((char*)"Get WeldResult : Invalid parameter index passed : %d",idx,0,0);
		break;
	}
	return result;
}

/**************************************************************************//**
* \brief  - Saves a specific result from the weld results.
*
* \param  - idx - 	index of specific result to save
* \param  - data - 	value to set specific result to
*
* \return  - None
*
******************************************************************************/
void WeldResults::Set(const WELD_RESULT_IDX idx, const UINT32 data)
{
	switch(idx)
	{
	case WeldResults::CYCLE_COUNTER:
		m_WeldResult.CycleCounter = data;
		break;
	case WeldResults::RECIPE_NUM:
		m_WeldResult.RecipeNumber = data;
		break;
	case WeldResults::RECIPE_REV_NUM:
		m_WeldResult.RecipeRevNumber = data;
		break;
	case WeldResults::WELD_STATUS:
		m_WeldResult.WeldStatus = data;
		break;
	case WeldResults::TOTAL_ENERGY:
		m_WeldResult.TotalEnergy = data;
		break;
	case WeldResults::ACTUAL_WIDTH:
		m_WeldResult.ActualWidth = data;
		break;
	case WeldResults::ACTUAL_AMPLITUDE:
		m_WeldResult.Amplitude = data;
		break;
	case WeldResults::TRIGGER_PRESSURE:
		m_WeldResult.TriggerPressure = data;
		break;
	case WeldResults::WELD_PRESSURE:
		m_WeldResult.WeldPressure = data;
		break;
	case WeldResults::WELD_TIME:
		m_WeldResult.WeldTime = data;
		break;
	case WeldResults::PEAK_POWER:
		m_WeldResult.PeakPower = data;
		break;
	case WeldResults::PREHEIGHT:
		m_WeldResult.PreHeight = data;
		break;
	case WeldResults::POST_HEIGHT:
		m_WeldResult.PostHeight = data;
		break;
	case WeldResults::WELD_ALARM:
		m_WeldResult.ALARMS.ALARMflags = data;
		break;
	default:
		LOGERR((char*)"Set WeldResult : Invalid parameter index passed : %d",idx,0,0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Returns current active recipe name
*
* \param   - None
*
* \return  - string recipe name.
*
******************************************************************************/
string WeldResults::GetRecipeName()
{
	return (const char*)m_WeldResult.RecipeName;
}

/**************************************************************************//**
* 
* \brief   - Set current active recipe name. 
*
* \param   - char* name
*
* \return  - None.
*
******************************************************************************/
void WeldResults::SetRecipeName(const char* name)
{
	strncpy(m_WeldResult.RecipeName, name, sizeof(m_WeldResult.RecipeName));
}

/**************************************************************************//**
* 
* \brief   - Returns current user name
*
* \param   - None
*
* \return  - string user name.
*
******************************************************************************/
string WeldResults::GetUserName()
{
	return "";
}

/**************************************************************************//**
* 
* \brief   - Set current user name. 
*
* \param   - char* name
*
* \return  - None.
*
******************************************************************************/
void WeldResults::SetUserName(const char* name)
{
	
}

/**************************************************************************//**
* \brief   - Get Part ID
*
* \param   - None
*
* \return  - char *PartId - Bar code read Part ID to be updated
*
******************************************************************************/
string WeldResults::GetPartID()
{
	return (const char*)m_WeldResult.PartID;
}

/**************************************************************************//**
* \brief   - Set Part ID 
*
* \param   - char *partid - Part ID read from the bar code
*
* \return  - None
*
******************************************************************************/
void WeldResults::SetPartID(const char* partid)
{
	strncpy(m_WeldResult.PartID, partid, sizeof(m_WeldResult.PartID));
}

/**************************************************************************//**
* 
* \brief   - Get the member data of WeldResults. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResults::WELD_RESULT WeldResults::GetWeldData()
{
	return m_WeldResult;
}
