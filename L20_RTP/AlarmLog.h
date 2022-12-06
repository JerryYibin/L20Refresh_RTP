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

class AlarmLog{
public:
	static AlarmLogDefine* _ptrAlarm;
public:
	AlarmLog();
	~AlarmLog();
	static int Set(const UI_ALARM_LOG*);
	static int Get(UI_ALARM_LOG*);
};

#endif
