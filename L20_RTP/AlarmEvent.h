/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The AlarmEvent is the reused code both for E1, P1 and L20.
 
***************************************************************************/

#ifndef ALARMEVENT_H_
#define ALARMEVENT_H_

#include <iostream>
#include <string.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define ALARMTYPE_MASK  	0x0F00
#define ALARMSUBTYPE_MASK  	0x00F0
// TODO The definitions should have a one place code what will be implemented in item 9589 
#define RECORD_LEN			128
#define TIME_STAMP_LEN		20
using namespace std;
class AlarmEvent 
{
public:
	// source generating the alarm
	enum ALARM_FROM
	{
		NO_SOURCE = 0,
		ALARM_SC,
		ALARM_PC,
		ALARM_AC,
		MAX_SOURCE
	};

	// alarm configurable actions
	struct ALARM_ACTIONS
	{
		bool RESET_REQUIRED;
		bool LOG_ALARM;
		bool GENERAL_ALARM;
		bool CUSTOM_ALARM_OUTPUT;
		bool CYCLE_COUNTER;
		bool PERSISTENT;
		bool RESERVED;
	};

	// types (groups) of alarms
	enum ALARM_TYPE
	{
		TYPE_OVERLOAD = 0,
		TYPE_CYCLE_MODIFIED,
		TYPE_WARNING,
		TYPE_PROCESS_LIMIT,
		TYPE_HARDWARE_FAILURE,
		TYPE_NOCYCLE,
		TYPE_NOCYCLE_OVERLOAD,
		TYPE_EN_FAULT
	};

	// sub types within the group of alarms
	enum OVERLOAD_SUB_TYPE
	{
		OVERLOAD_WELD = 0,
		OVERLOAD_ENERGY_BRAKE,
		OVERLOAD_AFTER_BURST,
		OVERLOAD_POST_WELD,

		OVERLOAD_TEST,
		OVERLOAD_PRE_TRIGGER,
		OVERLOAD_SEEK,
		OVERLOAD_THERMAL,
		OVERLOAD_PRE_WELD_SEEK
	};

	enum ALARM_STATE
	{
		STATE_INACTIVE = -1,
		STATE_ACTIVE,
		STATE_PENDING,
		STATE_CLEARED
	};

	enum ALARM_PRIORITY
	{
		PRIORITY_HIGHEST = 0,
		PRIORITY_HIGH,
		PRIORITY_MEDIUM,
		PRIORITY_LOW,
		PRIORITY_LOWEST
	};

public:
	int				m_Id;
	int				m_Type;
	int				m_Source;
	int				m_Priority;
	int				m_SCState;
	int				m_State;
	int				m_Flag;

	char			m_strDescription[RECORD_LEN]; //The alarm event instance need to send to data task for database using message queue;
	char			m_strAlarmLog[RECORD_LEN];
	char			m_strTimeStamp[TIME_STAMP_LEN];
	int				m_WeldCount;
	
	string			m_RecipeName;
	int				m_WeldRecipeID;
	int				m_WeldResultID;
	ALARM_ACTIONS	m_AlarmActions;
	ALARM_ACTIONS* 	m_pAlarmActions;
public:
	AlarmEvent();
	virtual ~AlarmEvent();
};

#endif /* ALARMEVENT_H_ */
