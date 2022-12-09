/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used to define all the External Ethernet DIG related Data

***************************************************************************/
#include "ExternalDataDIG.h"

ExternalDataDIG* ExternalDataDIG::_objDIGData = nullptr;
/**************************************************************************//**
* \brief   - Constructor 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ExternalDataDIG::ExternalDataDIG() 
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
ExternalDataDIG::~ExternalDataDIG() 
{
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief   - Pop the specific Ethernet DIG Data from the list in DIG Protocol Library
*
* \param   - int type, ETHMESSAGE* _data
*
* \return  - ok or not
*
******************************************************************************/

int ExternalDataDIG::BuildProtocolData(int type, char* _data)
{
	int iResult = OK;
	
	if(_data == nullptr)
		return ERROR;
	switch(type)
	{
	case WELDDATA:
		//TODO:To be implemented
		break;
	case PRESET:
		//TODO:To be implemented
		break;
	case SYSCONFIG:
		//TODO:To be implemented
		break;
	case PWRCURVESEC1:
		//TODO:To be implemented
		break;
	case PWRCURVESEC2:
		//TODO:To be implemented
		break;
	case PWRCURVESEC3:
		//TODO:To be implemented
		break;
	case PWRCURVESEC4:
		//TODO:To be implemented
		break;	
	case HEARTBEAT:
		//TODO:To be implemented
		break;		
	default:
		iResult = ERROR;
		break;
	}
	return iResult;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
ExternalDataDIG* ExternalDataDIG::GetInstance()
{
	return (_objDIGData != nullptr) ? _objDIGData : (_objDIGData = new ExternalDataDIG());
}
