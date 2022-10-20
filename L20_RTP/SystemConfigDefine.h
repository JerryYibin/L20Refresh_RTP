/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define the system
configuration.

***************************************************************************/

#ifndef SYSTEM_CONFIG_DEFINE_H_
#define SYSTEM_CONFIG_DEFINE_H_
#include <memory>
#define TIME_SIZE							32
class SYSTEMCONFIG
{
public:
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

	enum OPTION_INDEX
	{
		LANGUAGE_OPT = 0,
		POWER_OPT,
		FREQUENCY_OPT,
		HGT_ENCODER,
		FOOT_PEDAL_ABORT,
		LOCK_ON_ALARM,
		COOLING_OPTION,
		COOLING_DURATION,
		COLLING_DELAY,
		AMP_UNIT_OPT,
		PRESSURE_UNIT_OPT,
		HGT_UNIT_OPT,
		MAX_AMPLITUDE,
		TEACH_QTY,
		TEACH_TYPE,
		TEACH_TIME_PL,
		TEACH_TIME_MS,
		TEACH_POWER_PL,
		TEACH_POWER_MS,
		TEACH_PREHGT_PL,
		TEACH_PREHGT_MS,
		TEACH_HGT_PL,
		TEACH_HGT_MS,
		DATE_TIME
	};
public:
	SYSTEMCONFIG();
	virtual ~SYSTEMCONFIG();
	
	virtual int		Size	() 										= 0;
	virtual int		Get		(const int index, void* _data)			= 0;
	virtual int		Set		(const int index, const void* _data)	= 0;
	virtual void	Init	()										= 0;
	static  void	CurrentTime(char*);
	static std::shared_ptr<SYSTEMCONFIG> GetSystemConfig();
protected:
	LANGUAGE		Language;
	POWER	 		PowerSupply;
	FREQUENCY 		Frequency;
};

class L20_SYSTEMCONFIG : public SYSTEMCONFIG
{
public:
	enum COOLING
	{
		DISABLE = 0,
		SECOND_PER_100JOULE
	};
	
	enum AMPLITUDE_UNIT
	{
		MICRON = 0,
		PERCENTAGE
	};
	
	enum PRESSURE_UNIT
	{
		PSI = 0,
		BAR,
		KPA
	};
	
	enum HEIGHT_UNIT
	{
		MILLIMETER = 0,
		INCH
	};
	
	enum TEACHMODE_TYPE{
		STANDARD = 0,
		AUTO,
		SIGMA
	};
	
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
public:
	L20_SYSTEMCONFIG();
	~L20_SYSTEMCONFIG();
	virtual int  	Size	() 										override;
	virtual int		Get		(const int index, void* _data)			override;
	virtual int 	Set		(const int index, const void* _data)	override;
	virtual void 	Init	()										override;
private:
	bool bHeightEncoder;
	bool bFootPedalAbort;
	bool bLockOnAlarm;
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

class P1_SYSTEMCONFIG : public SYSTEMCONFIG
{
public:
	P1_SYSTEMCONFIG();
	~P1_SYSTEMCONFIG();
	
	virtual int  	Size	() 										override;
	virtual int		Get		(const int index, void* _data)			override;
	virtual int 	Set		(const int index, const void* _data)	override;
	virtual void 	Init	()										override;
};

#endif
