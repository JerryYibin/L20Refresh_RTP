/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define the system
configuration.

***************************************************************************/

#ifndef SYSTEM_CONFIGURATION_COMMON_H_
#define SYSTEM_CONFIGURATION_COMMON_H_

#include <memory>

enum COOLING
{
	DISABLE = 0,
	SECOND_PER_100JOULE
};

enum LANGUAGE{
	English = 0,
	Chinese_Simplified,
	Chinese_Traditional,
	Francis,
	Deutsch,
	Italiano,
	Japanese,
	Korean,
	Espanol
};

enum TEACHMODE_TYPE{
	STANDARD = 0,
	AUTO,
	SIGMA
};

enum AMPLITUDE_UNIT{
	MICRON = 0,
	PERCENTAGE
};

enum PRESSURE_UNIT{
	PSI = 0,
	BAR,
	KPA
};

enum HEIGHT_UNIT{
	MILLIMETER = 0,
	INCH
};

enum POWER_TYPE{
	POWER_3300W = 0,
	POWER_4000W,
	POWER_5500W,
	POWER_8000W
};
enum FREQUENCY{
	FREQ_20KHZ = 0,
	FREQ_30KHZ,
	FREQ_40KHZ
};

#define TIME_SIZE							32

struct TEACHMODE
{
	unsigned int Quantity;
	TEACHMODE_TYPE Teach_mode;
	unsigned int Time_Upper;
	unsigned int Time_Lower;
	unsigned int Power_Upper;
	unsigned int Power_Lower;
	unsigned int PreHeight_Upper;
	unsigned int PreHeight_Lower;
	unsigned int PostHeight_Upper;
	unsigned int PostHeight_Lower;
};

struct SYSTEMCONFIGFORUI
{
	LANGUAGE		Language;
	POWER_TYPE 		PowerSupply;
	FREQUENCY 		Frequency;
	bool bHeightEncoder;
	bool bFootPedalAbort;
	bool bLockOnAlarm;
	bool bFirstScreen;
	COOLING Cooling;
	unsigned int CoolingDuration;
	unsigned int CoolingDelay;

	AMPLITUDE_UNIT Amplitude_Unit;
	PRESSURE_UNIT Pressure_Unit;
	HEIGHT_UNIT Height_Unit;

	unsigned int MaxAmplitude;
	
	TEACHMODE TeachMode;

	char DateTime[TIME_SIZE];
	
};

struct POWER_SUPPLY_TYPE
{
	unsigned int Frequency;
	unsigned int Power;
};

#endif
