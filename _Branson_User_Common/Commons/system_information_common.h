/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define the system
information.

***************************************************************************/

#ifndef SYSTEM_INFORMATION_COMMON_H_
#define SYSTEM_INFORMATION_COMMON_H_


//system information structure for the HMI response parsing
#define SYSINFO_SIZE						16
#define MAC_ADDR_SIZE      					18
struct SYSTEM_INFO
{
	unsigned int	psLifeCounter;							//L20 refresh display only
	unsigned int	actuatorlifecounter;
	unsigned int	generalAlarmCounter;
	unsigned int	overloadAlarmCounter;
	unsigned int	actuatorOverloads;
	unsigned int	actuatorStrokeLength;
	unsigned int	psFrequency;							//L20 refresh display only
	unsigned int	psWatt;									//L20 refresh display only
	unsigned int	calibrationStatus;
	unsigned int	psType;
	unsigned int	actuatorType;
	unsigned int	crc_SC;
	unsigned int	crc_AC;
	char    		modelName[SYSINFO_SIZE];
	char    		version_SC[SYSINFO_SIZE];				//L20 refresh display only
	char    		version_PC[SYSINFO_SIZE];
	char    		version_AC[SYSINFO_SIZE];
	char			version_DB[SYSINFO_SIZE];				//L20 refresh display only
	char			version_HMI[SYSINFO_SIZE];				//L20 refresh display only
	char    		psAssemblyNumber[SYSINFO_SIZE];
	char    		actuatorAssembly[SYSINFO_SIZE];
	char    		stackAssembly[SYSINFO_SIZE];
	char    		psMacID[MAC_ADDR_SIZE];					//L20 refresh display only
	char    		psIP[SYSINFO_SIZE];						//L20 refresh display only
	char    		dateTime[2 * SYSINFO_SIZE];				//L20 refresh display only
};


#endif
