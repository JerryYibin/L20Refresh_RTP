/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define the alarm
code IDs.

***************************************************************************/

#include "AlarmManager.h"
#include "GPIO.h"
#include "ActuatorTask.h"
#include "PowerSupplyTask.h"
#include "SCStateMachine/SCStateMachine.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
map<int, AlarmEvent*>* AlarmManager::_AlarmListSC = nullptr;
vector<UI_ALARM_LOG>* AlarmManager::_AlarmListUI = nullptr;
SEM_ID AlarmManager::SemaphoreMutex = SEM_ID_NULL;
AlarmManager* AlarmManager::_AlarmManager = nullptr;

AlarmManager* AlarmManager::GetInstance()
{
	return _AlarmManager ? _AlarmManager : (_AlarmManager = new AlarmManager());
}

/**************************************************************************//**
* 
* \brief   - Constructor Initialize Alarm Manager
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
AlarmManager::AlarmManager() {
	_AlarmListSC = new map<int, AlarmEvent*>();
	_AlarmListUI = new vector<UI_ALARM_LOG>();
	/*create semaphore*/
	SemaphoreMutex = semMCreate(SEM_Q_FIFO);
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
AlarmManager::~AlarmManager() {
	auto iter = _AlarmListSC->begin();
	while (iter != _AlarmListSC->end())
	{
		AlarmEvent* _alarmEvent = iter->second;
		if (_alarmEvent != nullptr)
			delete _alarmEvent;
	}
	_AlarmListSC->clear();
	_AlarmListUI->clear();
	if(SemaphoreMutex != SEM_ID_NULL)
	{
		semDelete(SemaphoreMutex);
		SemaphoreMutex = SEM_ID_NULL;
	}
}

/**************************************************************************//**
* 
* \brief   - The function is for the function external recall in case of the alarm happened. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AlarmManager::EnterAlarmEvent(const AlarmEvent* _event)
{
	semTake(SemaphoreMutex, WAIT_FOREVER); //lock map for map essential operation
	if(_AlarmListSC->find(_event->m_Type) == _AlarmListSC->end())
	{
		AlarmEvent* alarm = new AlarmEvent();
		alarm->m_WeldCount = _event->m_WeldCount;
		alarm->m_Flag = _event->m_Flag;
		alarm->m_Id = _event->m_Id;
		alarm->m_Priority = _event->m_Priority;
		alarm->m_SCState = _event->m_SCState;
		alarm->m_Source = _event->m_Source;
		alarm->m_State = _event->m_State;
		alarm->m_Type = _event->m_Type;
		alarm->m_WeldRecipeID = _event->m_WeldRecipeID;
		alarm->m_WeldResultID = _event->m_WeldResultID;
		memcpy(alarm->m_strAlarmLog, _event->m_strAlarmLog, RECORD_LEN);
		memcpy(alarm->m_strTimeStamp, _event->m_strTimeStamp, 20);
		memcpy(alarm->m_strDescription, _event->m_strDescription, RECORD_LEN);
		_AlarmListSC->insert(pair<int, AlarmEvent*>(_event->m_Type, alarm));
		//TODO still need to add output signal handler.
		vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
	}
	semGive(SemaphoreMutex); //release map
}

/**************************************************************************//**
* 
* \brief   - For UI to get more details getting for current alarm list.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AlarmManager::SyncUIAlarmList()
{
	semTake(SemaphoreMutex, WAIT_FOREVER); //lock map for map essential operation
	_AlarmListUI->clear();
	UI_ALARM_LOG alarmUI;
	for(auto iter = _AlarmListSC->begin(); iter != _AlarmListSC->end(); iter++)
	{
		alarmUI.AlarmType = iter->second->m_Type;
		alarmUI.WeldCount = iter->second->m_WeldCount;
		memset(alarmUI.DateTime, 0, TIME_STAMP_LEN);
		memcpy(alarmUI.DateTime, iter->second->m_strTimeStamp, TIME_STAMP_LEN);

		memset(alarmUI.RecipeName, 0, RECIPE_LEN);
		memcpy(alarmUI.RecipeName, iter->second->m_RecipeName, RECIPE_LEN);
		_AlarmListUI->push_back(alarmUI);
	}
	semGive(SemaphoreMutex); //release map
}

/**************************************************************************//**
* \brief  - Clear Alarms and Set AC, PC, SC state
*
* \param - UINT16 alarmID
*
* \return  - bool success
*
******************************************************************************/
void AlarmManager::ClearAlarmList()
{
	semTake(SemaphoreMutex, WAIT_FOREVER); //lock map for map essential operation
	for(auto iter = _AlarmListSC->begin(); iter != _AlarmListSC->end(); iter++)
	{
		switch(iter->second->m_Source)
		{
		case AlarmEvent::ALARM_SC:
			clearSupervisoryAlarm();
			break;
		case AlarmEvent::ALARM_PC:
			clearPowerSupplyAlarm();
			break;
		case AlarmEvent::ALARM_AC:
			clearActuatorAlarm();
			break;
		default:
			break;
		}
		delete iter->second;
	}
	_AlarmListSC->clear();
	//TODO still need to add output signal handler.
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
	semGive(SemaphoreMutex); //release map
}

/**************************************************************************//**
* \brief  - The function is for heartbeat alarm type only.
*
* \param - none
*
* \return  - none
*
******************************************************************************/
int AlarmManager::GetAlarmType()
{
	int alarmType = 0;
	//ESTOP has the highest priority against other alarms
	auto iter = _AlarmListSC->find(ALARM_ESTOP_NCA);
	if(iter != _AlarmListSC->end())
	{
		LOG("222222222222222222222222222222222222222222: %d\n", alarmType);
		alarmType = iter->first;
	}
	else
	{
		LOG("1111111111111111111111111111111111111111111: %d\n", _AlarmListSC->size());
		if(_AlarmListSC->size() != 0)
		{
			iter = _AlarmListSC->begin();
			alarmType = iter->first;
		}
	}
	return alarmType;
}

/**************************************************************************//**
* \brief  - Remove E-Stop event from Alarm list.
*
* \param - none
*
* \return  - none
*
******************************************************************************/
void AlarmManager::ClearEStopEvent()
{
	semTake(SemaphoreMutex, WAIT_FOREVER); //lock map for map essential operation
	//ESTOP has the highest priority against other alarms
	auto iter = _AlarmListSC->find(ALARM_ESTOP_NCA);
	if(iter != _AlarmListSC->end())
	{
		delete iter->second;
		_AlarmListSC->erase(iter);
	}
	semGive(SemaphoreMutex); //release map
}

/**************************************************************************//**
* \brief  - Clear up AC Alarm flag
*
* \param - none
*
* \return  - none
*
******************************************************************************/
void AlarmManager::clearActuatorAlarm()
{
	unsigned int coreState = ActuatorTask::GetCoreState();
	ActuatorTask::ClearCoreState(coreState);
}

/**************************************************************************//**
* \brief  - Clear up PC Alarm flag
*
* \param - none
*
* \return  - none
*
******************************************************************************/
void AlarmManager::clearPowerSupplyAlarm()
{
	unsigned int coreState = PowerSupplyTask::GetCoreState();
	PowerSupplyTask::ClearCoreState(coreState);
}

/**************************************************************************//**
* \brief  - Clear up SC Alarm flag
*
* \param - none
*
* \return  - none
*
******************************************************************************/
void AlarmManager::clearSupervisoryAlarm()
{
	unsigned int coreState = SCStateMachine::getInstance()->GetCoreState();
	SCStateMachine::getInstance()->ClearCoreState(coreState);
}
