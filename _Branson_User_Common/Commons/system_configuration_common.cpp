/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     For the implementation of functions in the system configuration
 
**********************************************************************************************************/

#include <iostream>
#include "system_configuration_common.h"
#include "SystemType.h"

extern "C"
{
	#include "time.h"
	#include "customSystemCall.h"
	#include "hwif/drv/resource/vxbRtcLib.h"
}

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
			LOGERR((char *)"MODEL_T : --------NO System Detected-----------", 0, 0, 0);
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
	InitialValue();
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

void L20_SYSTEMCONFIG::Get(char* buf)
{
	//memcpy(buf,this->DateTime,sizeof(buf));
}

void L20_SYSTEMCONFIG::Set(char *buf)
{
	memcpy(this->DateTime,buf,20);//sizeof(buf)
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_SYSTEMCONFIG::InitialValue()
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

void P1_SYSTEMCONFIG::Get(char* buf)
{}

void P1_SYSTEMCONFIG::Set(char *buf)
{}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 system configuration.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_SYSTEMCONFIG::InitialValue()
{}
