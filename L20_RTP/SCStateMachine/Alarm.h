/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef ALARM_H_
#define ALARM_H_
#include "SCState.h"
#define ALARMBEEPDELAY 1000
class Alarm: public SCState {
private:
	unsigned int m_iHoldTime;
private:
	int getAlarmType(const int origin);
public:
	Alarm();
	virtual ~Alarm();
public:
	virtual void Enter	()	override;
	virtual void Loop	()	override;
	virtual void Exit	()	override;
	virtual void Fail	()	override;
};

#endif /* ALARM_H_ */
