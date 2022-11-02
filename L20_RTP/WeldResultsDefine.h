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
#ifndef WELDRESULTS
#define WELDRESULTS
#define L20REFRESH 							1
#define USER_NAME_SIZE						16
#define BARCODE_DATA_SIZE					50
#define TABLE_RESULT_MEM                    15 /* there are 15 columns in table WeldResult */
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
//	enum WELD_STATUS
//	{
//		WELDABORT 	= -1,
//		WELDDONE 	= 0
//	};
	
class WeldResults
{
public:
	WeldResults();
	virtual ~WeldResults();

	enum PARALIST {
		ACTUAL_WIDTH = 0,
		TRIGGER_PRESSURE,
		WELD_PRESSURE,
		PRE_HEIGHT,
		POST_HEIGHT,
		ALARM_FLAGS,
	};

	char	UserName[USER_NAME_SIZE];
	char 	PartID[BARCODE_DATA_SIZE];
	UINT32  CycleCounter;
	UINT32  RecipeNumber;
	UINT32  RecipeRevNumber;
	UINT32	WeldStatus;
	UINT32  PeakPower;
	UINT32  WeldTime;
	UINT32  TotalEnergy;
	UINT32	Amplitude;
public:
    virtual int Set(int, void*) = 0;
    virtual int Get(int, void*) = 0;
    virtual void Init() 		= 0;
    static std::shared_ptr<WeldResults> GetWeldResults();
};

class L20_WeldResults : public WeldResults
{
public:
	UINT32	ActualWidth;
	UINT32	TriggerPressure;
	UINT32	WeldPressure;
	UINT32	PreHeight;
	UINT32	PostHeight;
	UINT32  AlarmFlags;
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
