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


enum COOLING
{
	DISABLE = -1,
	SECOND_PER_100JOULE,
	STANDARD
};

enum Language{
	Chinese_Simplified = 0,
	Chinese_Traditional,
	English,
	Francis,
	Deutsch,
	Italiano,
	Japanese,
	Korean,
	Espanol
};

enum TMtype{
	Standard = 0,
	Auto,
	Sigma
};

enum AUNIT{
	um = 0,
	percentage,
};

enum PUNIT{
	pSI = 0,
	Bar,
	kPa,
};

enum HUNIT{
	mm = 0,
	inch,
};

enum POWERS{
	POWERS_3300W = 0,
	POWERS_4000W,
	POWERS_5500W,
};
enum FREQUENCY{
	FREQ_20KHZ = 0,
	FREQ_30KHZ,
	FREQ_40KHZ,
};

#define SYSINFO_SIZE						16
#define MAC_ADDR_SIZE      					18
class SYSTEM_CONFIG
{
public:
	Language	LanguageIs;
	POWERS 		PowerSupply;
	FREQUENCY 	Frequency;
};

class L20SystemConfig : public SYSTEM_CONFIG
{
public:
	bool bIsHeightEncoder;
	bool bIsFootPedalAbort;
	bool bIsLockOnAlarm;
	COOLING Cooling;
	unsigned int CoolingDuration;
	unsigned int CoolingDelay;

	AUNIT Amplitude_Unit;
	PUNIT Pressure_Unit;
	HUNIT Height_Unit;

	unsigned int Quantity;
	TMtype Teach_mode;
	unsigned int Time_Upper;
	unsigned int Time_Lower;
	unsigned int Power_Upper;
	unsigned int Power_Lower;
	unsigned int PreHeight_Upper;
	unsigned int PreHeight_Lower;
	unsigned int PostHeight_Upper;
	unsigned int PostHeight_Lower;

	char SetTime[2 * SYSINFO_SIZE];	
};

class P1SystemConfig : public SYSTEM_CONFIG
{
public:
	
};

#endif
