/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file WeldResultsUI.h is to define the enum and struct related to the Results.

 **********************************************************************************************************/
#ifndef WELDRESULTSUI_H
#define WELDRESULTSUI_H
#define USER_NAME_SIZE		16
#define WELD_TIME_SIZE		20
struct WeldResultsUI{
	int  CycleCounter;
	char RecipeName[USER_NAME_SIZE];
	char DateTime[WELD_TIME_SIZE];
	int AlarmID;
	int WeldMode;
	int WeldResultID;
	int Energy;
	int EnergySetting;
	
	int Amplitude;
	int AmplitudeSetting;
	
	int TPressure;
	int TPressureSetting;
	
	int WPressure;
	int WPressureSetting;
	
	int MaxWeldTime;
	int WeldTime;
	int MinWeldTime;
	
	int MaxPower;
	int Power;
	int MinPower;
	
	int MaxPreHeight;
	int	PreHeight;
	int MinPreHeight;
	
	int MaxPostHeight;
	int	PostHeight;
	int MinPostHeight;
	
	int GraphID;
};

#endif
