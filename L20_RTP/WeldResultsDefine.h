/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

 The file WeldResultDefine.h is to define the enum and struct related to the Result.
 
***************************************************************************/
#include <stdio.h>
#include <cstring>
#include <memory>
#include <vector>
#include "Common.h"
#include "WeldResultsUI.h"
#ifndef WELDRESULTS
#define WELDRESULTS
#define L20REFRESH 							1
#define USER_NAME_SIZE						16
#define WELD_TIME_SIZE						20
#define BARCODE_DATA_SIZE					50
typedef uint32_t UINT32;
typedef int32_t  INT32;
using namespace std;
#if GSX
	struct WELD_RESULT
	{
		UINT32	MaxWeldForce;
		UINT32  EndHoldForce;
		UINT32	WeldAbsolute;
		UINT32	TotalAbsolute;
		UINT32  CycleCounter;
		UINT32  RecipeNumber;
		UINT32  RecipeRevNumber;
		UINT32  WeldTime;
		UINT32  CycleTime;  
		UINT32  TotalEnergy;
		UINT32  PeakPower;
		UINT32  StartFrequency;
		INT32   FrequencyChange;
		UINT32  Velocity;
		UINT32  TriggerDistance;
		INT32   WeldCollapseDistance;
		INT32   HoldCollapseDistance;
		INT32   TotalCollapseDistance;
		UINT32	TriggerTime;
		UINT32	WeldSonicTime;
		UINT32	HoldTime;
		UINT32  WeldMode;
		UINT32  StackSerialNo;
		UINT8	WeldStatus;
		UINT8 	recipeStatus;
		bool 	bIsAlarmflag;	
		char	UserName[USER_NAME_SIZE];
		char 	PartID[BARCODE_DATA_SIZE];
	};
#elif L20REFRESH
	
class WeldResults
{
public:
	WeldResults();
	virtual ~WeldResults();
	static shared_ptr<WeldResults> _WeldResults;

	enum PARALIST {
		TRIGGER_PRESSURE = 0,
		WELD_PRESSURE,
		PRE_HEIGHT,
		POST_HEIGHT,
		ALARM_ID,
		ENERGY_SETTING,
		AMPLITUDE_SETTING,
		T_PRESSURE_SETTING,
		W_PRESSURE_SETTING,
		MAX_WELD_TIME,
		MIN_WELD_TIME,
		MAX_POWER,
		MIN_POWER,
		MAX_PRE_HEIGHT,
		MIN_PRE_HEIGHT,
		MAX_POST_HEIGHT,
		MIN_POST_HEIGHT
		
	};

	int CycleCounter;
	int RecipeID;
	char RecipeName[USER_NAME_SIZE];
	char DateTime[WELD_TIME_SIZE];
	char PartID[BARCODE_DATA_SIZE];
	int WeldMode;
	int Energy;
	int Amplitude;
	int WeldTime;
	int PeakPower;
	int GraphID;
public:
    virtual int Set(int, void*) = 0;
    virtual int Get(int, void*) = 0;
    virtual void Init() 		= 0;
    static std::shared_ptr<WeldResults> GetWeldResults();
};

class L20_WeldResults : public WeldResults
{
public:
	int TriggerPressure;
	int WeldPressure;
	int	PreHeight;
	int	PostHeight;
	int AlarmID;
	
	int EnergySetting;
	int AmplitudeSetting;
	int TPressureSetting;
	int WPressureSetting;
	
	int MaxWeldTime;
	int MinWeldTime;
	
	int MaxPower;
	int MinPower;
	
	int MaxPreHeight;
	int MinPreHeight;
	
	int MaxPostHeight;
	int MinPostHeight;

public:
    int Set(int, void*) 	override;
    int Get(int, void*) 	override;
    void Init()				override;
};

class P1_WeldResults : public WeldResults
{
public:
	UINT32	MaxWeldForce;
	UINT32  EndHoldForce;
	UINT32	WeldAbsolute;
	UINT32	TotalAbsolute;
	UINT32  CycleTime;

	UINT32  StartFrequency;
	INT32   FrequencyChange;
	UINT32  Velocity;
	UINT32  TriggerDistance;
	INT32   WeldCollapseDistance;
	INT32   HoldCollapseDistance;
	INT32   TotalCollapseDistance;
	UINT32	TriggerTime;
	UINT32	WeldSonicTime;
	UINT32	HoldTime;
	UINT32  WeldMode;
	UINT32  StackSerialNo;
	UINT32 	recipeStatus;
	bool 	bAlarmflag;
public:
    int Set(int, void*) 	override;
    int Get(int, void*) 	override;
    void Init()				override;
};
#endif
#endif /* WELDRESULTS_H_ */
