/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

Alarm Manager for all the alarm handling, such as alarm what is from SC, AC or PC and E-Stop
Alarm Manager shall for E1, P1 and L20 Refresh that shall be the same.

***************************************************************************/

#ifndef ALARMMANAGER_H_
#define ALARMMANAGER_H_
#include "AlarmEvent.h"
#include "AlarmCodeUI.h"
#include <map>
#include <semLib.h>
class AlarmManager {
private:
	static SEM_ID SemaphoreMutex; /* ID of mutex semaphore */
	static AlarmManager* _AlarmManager;
public:
	// various alarm actions
	enum ALARM_ACTION_COUNT
	{
		COUNT_ALL,
		COUNT_RESET_REQ,
		COUNT_LOG_ALARM,
		COUNT_GENERAL_OUTPUT,
		COUNT_CUSTOM_OUTPUT,
		COUNT_CYCLE_COUNT
	};
	
	// counts for various alarm types
	enum ALARM_TYPE_COUNT
	{
		COUNT_OVERLOAD,
		COUNT_CYCLE_MODIFIED,
		COUNT_WARNING,
		COUNT_PROCESS_SUSPECT,
		COUNT_PROCESS_REJECT,
		COUNT_HARDWARE_FAILURE,
		COUNT_NOCYCLE,
		COUNT_NOCYCLE_OVERLOAD
	};
	
	// alarm configuration in configurable actions
	struct ALARM_CONFIGURATION
	{
		AlarmEvent::ALARM_ACTIONS  OverloadAlarmActions;
		AlarmEvent::ALARM_ACTIONS  CycleModifiedAlarmActions;
		AlarmEvent::ALARM_ACTIONS  WarningAlarmActions;
		AlarmEvent::ALARM_ACTIONS  SuspectAlarmActions;
		AlarmEvent::ALARM_ACTIONS  RejectAlarmActions;
		AlarmEvent::ALARM_ACTIONS  NoCycleAlarmActions;
		AlarmEvent::ALARM_ACTIONS  HardwareFailureAlarmActions;
		AlarmEvent::ALARM_ACTIONS  NoCycleOverloadAlarmActions;
	};
	static map<int, AlarmEvent*>* _AlarmListSC;
	static vector<UI_ALARM_LOG>* _AlarmListUI;
public:
	static AlarmManager* GetInstance();
	virtual ~AlarmManager();
public:
	void EnterAlarmEvent(const AlarmEvent* _event);
	void SyncUIAlarmList();
	void ClearAlarmList();
	int GetAlarmType();
private:
	void operateMapOperand(const int type, const AlarmEvent* _event = nullptr);
	void clearActuatorAlarm();
	void clearPowerSupplyAlarm();
	void clearSupervisoryAlarm();
protected:
	AlarmManager();
};

#endif /* ALARMMANAGER_H_ */
