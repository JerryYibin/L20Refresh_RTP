/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#ifndef ALARM_H_
#define ALARM_H_
#include "AlarmDefine.h"
#include "AlarmCodeUI.h"
#include "CommonProperty.h"

#define TABLE_ALARM_MEM                    7 /* there are 7 columns in table AlarmLog */

class AlarmLog{
public:
	static shared_ptr<AlarmLogDefine> _ptrAlarm;
	static std::vector<shared_ptr<AlarmLogDefine>> AlarmVector;
public:
	AlarmLog();
	~AlarmLog();
	static std::vector<UI_ALARM_LOG> TransformAlarmLogVector();
};

#endif
