/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

      ASC Versions Management
 
**********************************************************************************************************/
#include  <sys/types.h>
#include  <cstdlib>
#include  <cstdio>
#include  <iostream>
#include  <string>
#include  <cstring>
#include  "versions.h"
#include  "version_common.h"

using namespace std;


/*
 *  initialize this with your own version base on your FW_VER_IDX
 *  and default for others.
 *  
 */
UINT16 FWVersion::versions[FW_VERSION_MAX][VERSION_FIELD_MAX] = {
		{SC_FW_VERSION_MAJOR, SC_FW_VERSION_MINOR, SC_FW_VERSION_BUILD},
		{0,0,0},
		{0,0,0},
};

UINT16 FWCrc::crc[FW_CRC_MAX] = {0,0,0};
UINT32 FWInfo::info[FW_INFO_MAX] = {0,0,0,0};
//char FWSerial::serial_ac[SER_VER_NO_SERIAL_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char FWSerial::serial_pc[SER_VER_NO_SERIAL_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char FWSerial::serial_asc[SER_VER_NO_SERIAL_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

FWVersion::FWVersion ()
{	
}

FWSerial::FWSerial ()
{	
}

FWCrc::FWCrc ()
{	
}

FWInfo::FWInfo ()
{	
}


/**************************************************************************//**
* \brief  - gets the version for the requested component's field
*
* \param  - idx - 	index of specific component version to get
* \param  - field - index of specific field to get
*
*
* \return  - value of component version
*
******************************************************************************/
UINT16 FWVersion::Get(FW_VER_IDX idx, VER_FIELD_IDX field)
{
	return versions[idx][field];
}


/**************************************************************************//**
* \brief  - sets the version for the requested component's field
*
* \param  - idx - 	index of specific component's version to set
* \param  - field - index of specific field to set
* \param  - val - 	value of specific component's version to set
*
*
* \return  - nothing
*
******************************************************************************/
void FWVersion::Set(FW_VER_IDX idx, VER_FIELD_IDX field, UINT16 val)
{
	if ((idx < FW_VERSION_MAX) && (field < VERSION_FIELD_MAX))
		versions[idx][field] = val;
}


/**************************************************************************//**
* \brief  - gets the serial number for the requested component
*
* \param  - FW_VER_IDX idx
*
* \return  - char *
*
******************************************************************************/
char * FWSerial::Get(FW_VER_IDX idx)
{
	//if (idx == FW_VERSION_AC)
	//		return serial_ac;
	//else if (idx == FW_VERSION_PC) 
	//		return serial_pc;
	//else if (idx == FW_VERSION_SC)
	//		return serial_asc;

	return 0;
}


/**************************************************************************//**
* \brief  - sets the serial number for the requested component
*
* \param  - FW_VER_IDX idx
* \param  - char * serialNo
*
* \return  - nothing
*
******************************************************************************/
void FWSerial::Set(FW_VER_IDX idx, char * serialNo)
{
	//_CHECK(serialNo)
	
	//for(int i = 0; i < SER_VER_NO_SERIAL_SIZE;i++)
	//{
	//	if (idx == FW_VERSION_AC)
	//			serial_ac[i] = serialNo[i];
	//	else if (idx == FW_VERSION_PC)
	//		serial_pc[i] = serialNo[i];
	//	else if (idx == FW_VERSION_SC)
	//		serial_asc[i] = serialNo[i];
	//}
}

/**************************************************************************//**
* \brief  - sets the firmware's CRC for the requested component
*
* \param  - idx - 	index of specific component
* \param  - val - 	value of specific component's CRC
*
*
* \return  - nothing
*
******************************************************************************/
void FWCrc::Set(FW_CRC_IDX idx, UINT16 val)
{
	if (idx < FW_CRC_MAX)
		crc[idx] = val;
}


/**************************************************************************//**
* \brief  - gets the firmware's CRC for the requested component
*
* \param  - idx - 	index of specific component's CRC to get
*
*
* \return  - value of component's CRC
*
******************************************************************************/
UINT16 FWCrc::Get(FW_CRC_IDX idx)
{
	return crc[idx];
}


/**************************************************************************//**
* \brief  - sets the firmware's info for the requested component
*
* \param  - idx - 	index of specific component
* \param  - val - 	value of specific component's info
*
*
* \return  - nothing
*
******************************************************************************/
void FWInfo::Set(FW_INFO_IDX idx, UINT32 val)
{
	if (idx < FW_INFO_MAX)
		info[idx] = val;
}


/**************************************************************************//**
* \brief  - gets the firmware's info for the requested component
*
* \param  - idx - 	index of specific component's info to get
*
*
* \return  - value of component's info
*
******************************************************************************/
UINT32 FWInfo::Get(FW_INFO_IDX idx)
{
	return info[idx];
}

/**************************************************************************//**
* \brief  - prints the version/serial for the requested component
*
* \param  - FW_VER_IDX idx
*
* \return  - None
*
******************************************************************************/
void FWVersion::PrintVersion(FW_VER_IDX idx)
{
	/*switch (idx)
	{
	case FW_VERSION_AC:

		LOG("\nPrintVersion: AC Version: %d.%d.%d.%d.",FWVersion::Get(idx, VERSION_MAJOR),FWVersion::Get(idx, VERSION_MINOR),
				FWVersion::Get(idx, VERSION_BUILD), AC_FW_VERSION_AUTOS);
		for(int i = 0; i < SER_VER_NO_SERIAL_SIZE;i++)
			LOG("%d", FWSerial::Get(idx)[i]);
		break;
		
	case FW_VERSION_PC:
		
		LOG("\nPrintVersion: DSP Version: %d.%d.%d.%d",FWVersion::Get(idx, VERSION_MAJOR),FWVersion::Get(idx, VERSION_MINOR),
				FWVersion::Get(idx, VERSION_BUILD), PC_FW_VERSION_AUTOS);
		break;
		
	case FW_VERSION_SC:
		
		LOG("\nPrintVersion: ASC version: %d.%d.%d.%d.", ASC_FW_VERSION_MAJOR, ASC_FW_VERSION_MINOR, ASC_FW_VERSION_BUILD, ASC_FW_VERSION_AUTOS);
		for(int i = 0; i < SER_VER_NO_SERIAL_SIZE;i++)
			LOG("%c",FWSerial::Get(idx)[i]);
		break;
	default:
		break;
	}

	LOG("\n");*/
}
