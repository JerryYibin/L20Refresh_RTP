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

enum POWER{
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

typedef struct TEACHMODE
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
}TEACHMODE;

typedef struct SYSTEMCONFIG
{
public:
	SYSTEMCONFIG();
	virtual ~SYSTEMCONFIG();
	
	LANGUAGE	Language;
	POWER 		PowerSupply;
	FREQUENCY 	Frequency;
	
	virtual int		Size() 			= 0;
	virtual void	InitialValue()	= 0;
	static  void	CurrentTime(char*);
	static std::shared_ptr<SYSTEMCONFIG> GetSystemConfig();
}SYSTEMCONFIG;

typedef struct L20_SYSTEMCONFIG : public SYSTEMCONFIG
{
public:
	L20_SYSTEMCONFIG();
	~L20_SYSTEMCONFIG();
	bool bHeightEncoder;
	bool bFootPedalAbort;
	bool bLockOnAlarm;
	COOLING Cooling;
	unsigned int CoolingDuration;
	unsigned int CoolingDelay;

	AMPLITUDE_UNIT Amplitude_Unit;
	PRESSURE_UNIT Pressure_Unit;
	HEIGHT_UNIT Height_Unit;

	TEACHMODE TeachMode;

	char DateTime[TIME_SIZE];
	
	int  Size() 			override;
	void InitialValue()		override;
}L20_SYSTEMCONFIG;

typedef struct P1_SYSTEMCONFIG : public SYSTEMCONFIG
{
public:
	P1_SYSTEMCONFIG();
	~P1_SYSTEMCONFIG();
	
	int  Size() 			override;
	void InitialValue()		override;
}P1_SYSTEMCONFIG;

#endif
