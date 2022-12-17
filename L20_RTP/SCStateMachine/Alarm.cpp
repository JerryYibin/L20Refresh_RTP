/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 Alarm state is the a independent state for other states. 
 It will be push back the state list when the sequence generating. 
 Why did push it into the end of state list? There is a good example for weld cycle.
 If there is any alarm happened during the welding, the horn shall be released that is not same with E-STOP. 
***************************************************************************/

#include "Alarm.h"
#include "SCStateMachine.h"
#include "../Recipe.h"
#include "../AlarmEvent.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
#include "../ActuatorTask.h"
#include "../PowerSupplyTask.h"
#include "../AlarmManager.h"
extern "C"
{
	#include "hwif/drv/resource/vxbRtcLib.h"
}
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Alarm::Alarm() {
	m_Actions = SCState::INIT;
	m_State = SCState::ALARM;

}

/**************************************************************************//**
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Alarm::~Alarm() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - Alarm State Enter. If there is any alarm detected, 
* 			the function needs to parse these alarms from AC, PC and SC using the unify alarm code.
* 			Register the alarm event into Alarm Manager and Save alarm event into Database using message queue. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Enter()
{
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		if((SCStateMachine::getInstance()->GetCoreState() & ERR_OVERLOAD) != ERR_OVERLOAD)
		{
			getPCAlarmEvents(true);
		}
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		getACAlarmEvents(true);
	}
	else if(SCStateMachine::getInstance()->GetCoreState() != 0)
	{
		getSCAlarmEvents(true);
	}
	else
	{
		m_Actions = SCState::JUMP;
		return;
	}

	m_Timeout = 0;
}

/**************************************************************************//**
*
* \brief   - Alarm Loop. Handle with Beeper delay.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Loop()
{
	if((PCStateMachine::PC_TX->PCState != PCState::PC_ALARM) && (ACStateMachine::AC_TX->ACState != ACState::AC_ALARM) && 
			(SCStateMachine::getInstance()->GetCoreState() == 0))
	{
		m_Actions = SCState::JUMP;
	}
	else
	{
		if (m_Timeout < ALARMBEEPDELAY)
		{
			m_Timeout++;
		}
		else
		{
			if (vxbGpioGetValue(GPIO::O_BUZZ) == GPIO_VALUE_HIGH)
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
			else
			{
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_HIGH);
				if(SCStateMachine::getInstance()->GetCoreState() != 0)
				{
					getSCAlarmEvents();
				}
				else if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
				{
					getPCAlarmEvents();
				}
				else if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
				{
					getACAlarmEvents();
				}
			}
			m_Timeout = 0;
		}
	}
}

/**************************************************************************//**
*
* \brief   - Alarm Exit close Beeper 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Exit()
{
	vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
	m_Timeout = 0;
}

/**************************************************************************//**
*
* \brief   - Alarm Fail
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Fail()
{

}

/**************************************************************************//**
*
* \brief   - Get AC Alarm Event
*
* \param   - isLogAlarm = false is the default setting.
*
* \return  - 0.
*
******************************************************************************/
int Alarm::getACAlarmEvents(bool isLogAlarm)
{
	unsigned int alarmCode = 0;
    char timeBuf[20];
    struct tm timeStamp;
	AlarmEvent event;
	memcpy(event.m_RecipeName, "N/A", RECIPE_LEN);
	event.m_WeldCount = -1;
	event.m_WeldRecipeID = -1;
	event.m_WeldResultID = -1;
	vxbRtcGet(&timeStamp);
	strftime(event.m_strTimeStamp, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
//	event.m_SCState = state;
	event.m_Source = AlarmEvent::ALARM_AC;
	memset(&event.m_AlarmActions, false, sizeof(AlarmEvent::ALARM_ACTIONS));
	event.m_AlarmActions.LOG_ALARM = true;
	event.m_AlarmActions.RESET_REQUIRED = true;
	
	alarmCode = ActuatorTask::GetCoreState();
	if ((alarmCode & ERR_PRESSURE_SET) == ERR_PRESSURE_SET)
	{
		event.m_Type = ALARM_PRESSURE_COMM_CAN_EFA;
		//Register the alarm into alarm list of alarm manager. 
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_HEIGHT_SYSTEM) == ERR_HEIGHT_SYSTEM)
	{
		event.m_Type = ALARM_HEIGHT_ENCODER_EFA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_HOME_POSITION) == ERR_HOME_POSITION)
	{
		event.m_Type = ALARM_HOME_POSITION_TIMEOUT_NCA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_STARTSWITCH_LOST) == ERR_STARTSWITCH_LOST)
	{
		event.m_Type = ALARM_START_SWITCH_LOST_EFA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	
	return 0;
}

/**************************************************************************//**
*
* \brief   - Get PC Alarm Event
*
* \param   - isLogAlarm = false is the default setting.
*
* \return  - 0.
*
******************************************************************************/
int Alarm::getPCAlarmEvents(bool isLogAlarm)
{
	unsigned int alarmCode = 0;
    char timeBuf[20];
    struct tm timeStamp;
	AlarmEvent event;
	memcpy(event.m_RecipeName, "N/A", RECIPE_LEN);
	event.m_WeldCount = -1;
	event.m_WeldRecipeID = -1;
	event.m_WeldResultID = -1;
	vxbRtcGet(&timeStamp);
	strftime(event.m_strTimeStamp, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
//	event.m_SCState = state;
	event.m_Source = AlarmEvent::ALARM_PC;
	memset(&event.m_AlarmActions, false, sizeof(AlarmEvent::ALARM_ACTIONS));
	event.m_AlarmActions.LOG_ALARM = true;
	event.m_AlarmActions.RESET_REQUIRED = true;
	
	alarmCode = PowerSupplyTask::GetCoreState();
	if ((alarmCode & ERR_POWER_OVERLOAD) == ERR_POWER_OVERLOAD)
	{
		event.m_Type = ALARM_POWER_OVERLOAD_OVA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	return 0;
}

/**************************************************************************//**
*
* \brief   - Get SC Alarm Event
*
* \param   - isLogAlarm = false is the default setting.
*
* \return  - 0
*
******************************************************************************/
int Alarm::getSCAlarmEvents(bool isLogAlarm)
{
	unsigned int alarmCode = 0;
    char timeBuf[20];
    struct tm timeStamp;
	AlarmEvent event;
	memcpy(event.m_RecipeName, Recipe::ActiveRecipeSC->m_RecipeName, RECIPE_LEN);
	event.m_WeldCount = WeldResults::_WeldResults->CycleCounter;
	event.m_WeldRecipeID = Recipe::ActiveRecipeSC->m_RecipeID;
	event.m_WeldResultID = -1;
	vxbRtcGet(&timeStamp);
	strftime(event.m_strTimeStamp, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
//	event.m_SCState = state;
	event.m_Source = AlarmEvent::ALARM_SC;
	
	memset(&event.m_AlarmActions, false, sizeof(AlarmEvent::ALARM_ACTIONS));
	event.m_AlarmActions.LOG_ALARM = true;
	event.m_AlarmActions.RESET_REQUIRED = true;
	event.m_AlarmActions.CYCLE_COUNTER = true;
	
	alarmCode = SCStateMachine::getInstance()->GetCoreState();
	if ((alarmCode & ERR_BATCH_SIZE) == ERR_BATCH_SIZE)
	{
		event.m_Type = ALARM_BATCH_COUNT_NCA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_OVERLOAD) == ERR_OVERLOAD)
	{
		event.m_Type = ALARM_POWER_OVERLOAD_OVA;
		//Register the alarm into alarm list of alarm manager. 
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_TIME_MS) == ERR_TIME_MS)
	{
		event.m_Type = ALARM_TIME_LIMIT_MLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_TIME_PL) == ERR_TIME_PL)
	{
		event.m_Type = ALARM_TIME_LIMIT_PLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_POWER_MS) == ERR_POWER_MS)
	{
		event.m_Type = ALARM_PEAKPOWER_LIMIT_MLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_POWER_PL) == ERR_POWER_PL)
	{
		event.m_Type = ALARM_PEAKPOWER_LIMIT_PLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_PRE_HEIGHT_MS) == ERR_PRE_HEIGHT_MS)
	{
		event.m_Type = ALARM_PREHEIGHT_LIMIT_MLR_PRA;
		event.m_AlarmActions.CYCLE_COUNTER = false;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_PRE_HEIGHT_PL) == ERR_PRE_HEIGHT_PL)
	{
		event.m_Type = ALARM_PREHEIGHT_LIMIT_PLR_PRA;
		event.m_AlarmActions.CYCLE_COUNTER = false;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_POST_HEIGHT_MS) == ERR_POST_HEIGHT_MS)
	{
		event.m_Type = ALARM_POSTHEIGHT_LIMIT_MLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	if ((alarmCode & ERR_POST_HEIGHT_PL) == ERR_POST_HEIGHT_PL)
	{
		event.m_Type = ALARM_POSTHEIGHT_LIMIT_PLR_PRA;
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		if(isLogAlarm == true)
			SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
	}
	return 0;
}
