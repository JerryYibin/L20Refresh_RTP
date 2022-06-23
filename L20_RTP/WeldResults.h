/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <stdio.h>
#include <cstring>
#ifndef WELDRESULTS_H_
#define WELDRESULTS_H_
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
	enum WELD_STATUS
	{
		WELDABORT 	= -1,
		WELDDONE 	= 0
	};
	
	struct WELD_RESULT
	{
		UINT32	RecipeNum;
		char 	PartID[BARCODE_DATA_SIZE];
		char	UserName[USER_NAME_SIZE];
		UINT32  CycleCounter;
		UINT32  RecipeNumber;
		UINT32  RecipeRevNumber;
		UINT32	WeldStatus;
		UINT32	TotalEnergy;
		UINT32	ActualWidth;
		UINT32	Amplitude;
		UINT32	TriggerPressure;
		UINT32	WeldPressure;
		UINT32	WeldTime;
		UINT32	PeakPower;
		UINT32	PreHeight;
		UINT32	PostHeight;
		union
		{
			UINT32	ALARMflags;
			/* There is a little endian for structure bit field alignment for alarm */
			struct
			{
				UINT32	Overload			: 1; //bit 0
				UINT32	HeightSystemFailure	: 1; //bit 1
				UINT32	WeldAborted			: 1; //bit 2
				
				UINT32	TimeMin				: 1; //bit 3
				UINT32	PowerMin			: 1; //bit 4
				UINT32	PreHeightMin		: 1; //bit 5
				UINT32	PostHeightMin		: 1; //bit 6
				
				UINT32	TimeMax				: 1; //bit 7
				UINT32	PowerMax			: 1; //bit 8
				UINT32	PreHeightMax		: 1; //bit 9
				UINT32	PostHeightMax		: 1; //bit 10
				
				UINT32	HeightEncoderBad	: 1; //bit 11
				UINT32	FootPedalAbort		: 1; //bit 12
				UINT32	SafetySwitch		: 1; //bit 13
				UINT32	WidthError			: 1; //bit 14
				
				UINT32	CutterSwitch		: 1; //bit 15
				
				
			} AlarmFlags;
		} ALARMS;
	};
#endif


#endif /* WELDRESULTS_H_ */
