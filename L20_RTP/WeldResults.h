/************************************************************************** 

 Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
 This program is the property of Branson Ultrasonics Corporation
 Copying of this software is expressly forbidden, without the prior
 written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
 
 ***************************************************************************/

#ifndef WELDRESULTS_H_
#define WELDRESULTS_H_
#include "Common.h"
#include "HeartBeatUI.h"
#include "WeldResultsDefine.h"
#include <vector>
///////////////////////////////////

#define L20REFRESH 							1
#define USER_NAME_SIZE						16
#define BARCODE_DATA_SIZE					50
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

struct WeldResult{
	static std::vector<shared_ptr<WeldResults>> WeldResultVector;
    static std::vector<WeldResultsUI> TransformResultsVector();
};

#endif

#endif
