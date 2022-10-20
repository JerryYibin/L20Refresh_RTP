/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     For the implementation of functions in the system configuration
 
**********************************************************************************************************/

#include <iostream>
#include <string.h>
#include "SystemConfigDefine.h"
#include "SystemType.h"
#include "commons.h"

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SYSTEMCONFIG::SYSTEMCONFIG()
{}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SYSTEMCONFIG::~SYSTEMCONFIG()
{}

/**************************************************************************//**
* 
* \brief   - Select different structures for different devices.
*
* \param   - None.
*
* \return  - Structure pointer.
*
******************************************************************************/
std::shared_ptr<SYSTEMCONFIG> SYSTEMCONFIG::GetSystemConfig()
{
	std::shared_ptr<SYSTEMCONFIG> ptrSystemConfig = nullptr;
	
	switch(GetSystemType())
	{
		case GSXP1:
		{
			ptrSystemConfig = std::make_shared<P1_SYSTEMCONFIG>();
		}
		break;
		
		/*case GSXE1:
		{
			ptrSystemConfig = std::make_shared<E1_SYSTEMCONFIG>();
		}
		break;*/
		
		case L20:
		{
			ptrSystemConfig = std::make_shared<L20_SYSTEMCONFIG>();
		}
		break;
		
		default:
		{
			//LOGERR((char *)"MODEL_T : --------NO System Detected-----------", 0, 0, 0);
			std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
		}
		break;
	}
	return ptrSystemConfig;
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of L20 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
L20_SYSTEMCONFIG::L20_SYSTEMCONFIG()
{
	Init();
	strcpy(DateTime,"0");
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
L20_SYSTEMCONFIG::~L20_SYSTEMCONFIG()
{}

/**************************************************************************//**
* 
* \brief   - Get data from L20 System Configuration.
*
* \param   - Index: data index; _data: output data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_SYSTEMCONFIG::Get(const int index, void* _data)
{
	int iResult = OK;
	if(_data == nullptr)
		return ERROR;
	switch(index)
	{
	case LANGUAGE_OPT:
		*(SYSTEMCONFIG::LANGUAGE*)(_data) = Language;
		break;
	case POWER_OPT:
		*(SYSTEMCONFIG::POWER*)(_data) = PowerSupply;
		break;
	case FREQUENCY_OPT:
		*(SYSTEMCONFIG::FREQUENCY*)(_data) = Frequency;
		break;
	case HGT_ENCODER:
		*(bool*)(_data) = bHeightEncoder;
		break;
	case FOOT_PEDAL_ABORT:
		*(bool*)(_data) = bFootPedalAbort;
		break;
	case LOCK_ON_ALARM:
		*(bool*)(_data) = bLockOnAlarm;
		break;
	case COOLING_OPTION:
		*(L20_SYSTEMCONFIG::COOLING*)(_data) = Cooling;
		break;
	case COOLING_DURATION:
		*(unsigned int*)(_data) = CoolingDuration;
		break;
	case COLLING_DELAY:
		*(unsigned int*)(_data) = CoolingDelay;
		break;
	case AMP_UNIT_OPT:
		*(L20_SYSTEMCONFIG::AMPLITUDE_UNIT*)(_data) = Amplitude_Unit;
		break;
	case PRESSURE_UNIT_OPT:
		*(L20_SYSTEMCONFIG::PRESSURE_UNIT*)(_data) = Pressure_Unit;
		break;
	case HGT_UNIT_OPT:
		*(L20_SYSTEMCONFIG::HEIGHT_UNIT*)(_data) = Height_Unit;
		break;
	case MAX_AMPLITUDE:
		*(unsigned int*)(_data) = MaxAmplitude;
		break;
	case TEACH_QTY:
		*(unsigned int*)(_data) = TeachMode.Quantity;
		break;
	case TEACH_TYPE:
		*(L20_SYSTEMCONFIG::TEACHMODE_TYPE*)(_data) = TeachMode.Teach_mode;
		break;
	case TEACH_TIME_PL:
		*(unsigned int*)(_data) = TeachMode.Time_Upper;
		break;
	case TEACH_TIME_MS:
		*(unsigned int*)(_data) = TeachMode.Time_Lower;
		break;
	case TEACH_POWER_PL:
		*(unsigned int*)(_data) = TeachMode.Power_Upper;
		break;
	case TEACH_POWER_MS:
		*(unsigned int*)(_data) = TeachMode.Power_Lower;
		break;
	case TEACH_PREHGT_PL:
		*(unsigned int*)(_data) = TeachMode.PreHeight_Upper;
		break;
	case TEACH_PREHGT_MS:
		*(unsigned int*)(_data) = TeachMode.PreHeight_Lower;
		break;
	case TEACH_HGT_PL:
		*(unsigned int*)(_data) = TeachMode.PostHeight_Upper;
		break;
	case TEACH_HGT_MS:
		*(unsigned int*)(_data) = TeachMode.PostHeight_Lower;
		break;
	case DATE_TIME:
		memcpy(_data, DateTime, TIME_SIZE);
		break;
	default:
		iResult = ERROR;
		break;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Set data to L20 System Configuration.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_SYSTEMCONFIG::Set(const int index, const void* _data)
{
	int iResult = OK;
	if(_data == nullptr)
		return ERROR;
	switch(index)
	{
	case LANGUAGE_OPT:
		Language = *(SYSTEMCONFIG::LANGUAGE*)(_data);
		break;
	case POWER_OPT:
		PowerSupply = *(SYSTEMCONFIG::POWER*)(_data);
		break;
	case FREQUENCY_OPT:
		Frequency = *(SYSTEMCONFIG::FREQUENCY*)(_data);
		break;
	case HGT_ENCODER:
		bHeightEncoder = *(bool*)(_data);
		break;
	case FOOT_PEDAL_ABORT:
		bFootPedalAbort = *(bool*)(_data);
		break;
	case LOCK_ON_ALARM:
		bLockOnAlarm = *(bool*)(_data);
		break;
	case COOLING_OPTION:
		Cooling = *(L20_SYSTEMCONFIG::COOLING*)(_data);
		break;
	case COOLING_DURATION:
		CoolingDuration = *(unsigned int*)(_data);
		break;
	case COLLING_DELAY:
		CoolingDelay = *(unsigned int*)(_data);
		break;
	case AMP_UNIT_OPT:
		Amplitude_Unit = *(L20_SYSTEMCONFIG::AMPLITUDE_UNIT*)(_data);
		break;
	case PRESSURE_UNIT_OPT:
		Pressure_Unit = *(L20_SYSTEMCONFIG::PRESSURE_UNIT*)(_data);
		break;
	case HGT_UNIT_OPT:
		Height_Unit = *(L20_SYSTEMCONFIG::HEIGHT_UNIT*)(_data);
		break;
	case MAX_AMPLITUDE:
		MaxAmplitude = *(unsigned int*)(_data);
		break;
	case TEACH_QTY:
		TeachMode.Quantity = *(unsigned int*)(_data);
		break;
	case TEACH_TYPE:
		TeachMode.Teach_mode = *(L20_SYSTEMCONFIG::TEACHMODE_TYPE*)(_data);
		break;
	case TEACH_TIME_PL:
		TeachMode.Time_Upper = *(unsigned int*)(_data);
		break;
	case TEACH_TIME_MS:
		TeachMode.Time_Lower = *(unsigned int*)(_data);
		break;
	case TEACH_POWER_PL:
		TeachMode.Power_Upper = *(unsigned int*)(_data);
		break;
	case TEACH_POWER_MS:
		TeachMode.Power_Lower = *(unsigned int*)(_data);
		break;
	case TEACH_PREHGT_PL:
		TeachMode.PreHeight_Upper = *(unsigned int*)(_data);
		break;
	case TEACH_PREHGT_MS:
		TeachMode.PreHeight_Lower = *(unsigned int*)(_data);
		break;
	case TEACH_HGT_PL:
		TeachMode.PostHeight_Upper = *(unsigned int*)(_data);
		break;
	case TEACH_HGT_MS:
		TeachMode.PostHeight_Lower = *(unsigned int*)(_data);
		break;
	case DATE_TIME:
		memcpy(DateTime, (char*)_data, TIME_SIZE);//sizeof(buf)
		break;
	default:
		iResult = ERROR;
		break;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_SYSTEMCONFIG::Init()
{
	Language 					= English;
	PowerSupply 				= POWER_3300W;
	Frequency 					= FREQ_20KHZ;
	bHeightEncoder				= false;
	bFootPedalAbort				= false;
	bLockOnAlarm				= false;
	Cooling						= DISABLE;
	CoolingDuration				= 0;
	CoolingDelay				= 0;

	Amplitude_Unit				= MICRON;
	Pressure_Unit				= PSI;
	Height_Unit					= MILLIMETER;

	TeachMode.Quantity			= 0;
	TeachMode.Teach_mode		= STANDARD;
	TeachMode.Time_Upper		= 0;
	TeachMode.Time_Lower		= 0;
	TeachMode.Power_Upper		= 0;
	TeachMode.Power_Lower		= 0;
	TeachMode.PreHeight_Upper	= 0;
	TeachMode.PreHeight_Lower	= 0;
	TeachMode.PostHeight_Upper	= 0;
	TeachMode.PostHeight_Lower	= 0;
}

/**************************************************************************//**
* 
* \brief   - Size of L20 structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int L20_SYSTEMCONFIG::Size()
{
	return sizeof(L20_SYSTEMCONFIG) - V_PTR_SIZE;
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of L20 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
P1_SYSTEMCONFIG::P1_SYSTEMCONFIG()
{}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
P1_SYSTEMCONFIG::~P1_SYSTEMCONFIG()
{}

/**************************************************************************//**
* 
* \brief   - Size of P1 structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int P1_SYSTEMCONFIG::Size()
{
	return sizeof(P1_SYSTEMCONFIG) - V_PTR_SIZE;
}

int P1_SYSTEMCONFIG::Get(const int index, void* _data)
{
	return OK;
}

int P1_SYSTEMCONFIG::Set(const int index, const void* _data)
{
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_SYSTEMCONFIG::Init()
{}
