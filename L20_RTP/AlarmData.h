/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The AlarmData is for the AlarmData process only.
 
***************************************************************************/

#ifndef ALARMDATA_H_
#define ALARMDATA_H_
#include "AlarmCodeUI.h"
#include <iostream>
#include <vector>

#define ALARM_LOG_MAX 50

struct ALARM_DATA
{
	unsigned int 	AlarmType;
	unsigned int	AlarmSubType;
	unsigned int	WeldResultID;
	unsigned int	WeldRecipeID;
	unsigned int	UserID;
};
using namespace std;
class AlarmDataSC 
{
public:
	static ALARM_DATA AlarmData;
	static vector<UI_ALARM_LOG> AlarmLog;
public:
	AlarmDataSC();
	virtual ~AlarmDataSC();
};

#endif /* ALARMDATA_H_ */
