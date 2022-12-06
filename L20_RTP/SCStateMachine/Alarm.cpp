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
* \brief   - Alarm State Enter. If there is any alarm happened.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Enter()
{
	int origin;
    char timeBuf[20];
    struct tm timeStamp;
	AlarmEvent event;
	if(PCStateMachine::PC_TX->PCState == PCState::PC_ALARM)
	{
		origin = AlarmEvent::ALARM_PC;
		event.m_RecipeName = "N/A";
		event.m_WeldCount = -1;
		event.m_WeldRecipeID = -1;
		event.m_WeldResultID = -1;
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
	{
		origin = AlarmEvent::ALARM_AC;
		event.m_RecipeName = "N/A";
		event.m_WeldCount = -1;
		event.m_WeldRecipeID = -1;
		event.m_WeldResultID = -1;
	}
	//TODO missing SC Alarm handling.
	else
	{
		m_Actions = SCState::JUMP;
		return;
	}

//	event.m_SCState = state;
	event.m_Source = origin;
	event.m_Type = getAlarmType(origin);

	vxbRtcGet(&timeStamp);
	strftime(event.m_strTimeStamp, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
	m_Timeout = 0;
	//Register the alarm into alarm list of alarm manager. 
	AlarmManager::GetInstance()->EnterAlarmEvent(&event);
	//Record the alarm into alarm table of database.
	MESSAGE message;
	message.msgID = DataTask::TO_DATA_TASK_ALARM_LOG_INSERT;
	memcpy(message.Buffer, (void*)&event, sizeof(AlarmEvent));
	SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
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
	if (m_Timeout < ALARMBEEPDELAY)
	{
		m_Timeout++;
	}
	else
	{
		if (vxbGpioGetValue(GPIO::O_BUZZ) == GPIO_VALUE_HIGH)
			vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
		else
			vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_HIGH);
		m_Timeout = 0;
	}
	//TODO missing SC alarm handling
	if((PCStateMachine::PC_TX->PCState != PCState::PC_ALARM) && (ACStateMachine::AC_TX->ACState != ACState::AC_ALARM))
	{
		vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
		m_Actions = SCState::JUMP;
	}
}

/**************************************************************************//**
*
* \brief   - Hold time Exit 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Exit()
{
	m_Timeout = 0;
}

void Alarm::Fail()
{

}

/**************************************************************************//**
*
* \brief   - Get alarm type. There is the unify indicators for each alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int	Alarm::getAlarmType(const int origin)
{
	unsigned int alarmCode = 0;
	unsigned int alarmType = 0;
	switch (origin)
	{
	case AlarmEvent::ALARM_AC:
		alarmCode = ActuatorTask::GetCoreState();
		if ((alarmCode & ERR_PRESSURE_SET) == ERR_PRESSURE_SET)
		{
			alarmType = ALARM_PRESSURE_COMM_CAN_EFA;
		}
		else if ((alarmCode & ERR_HEIGHT_SYSTEM) == ERR_HEIGHT_SYSTEM)
		{
			alarmType = ALARM_HEIGHT_ENCODER_EFA;
		}
		else if ((alarmCode & ERR_HOME_POSITION) == ERR_HOME_POSITION)
		{
			alarmType = ALARM_HOME_POSITION_TIMEOUT_NCA;
		}
		else if ((alarmCode & ERR_STARTSWITCH_LOST) == ERR_STARTSWITCH_LOST)
		{
			alarmType = ALARM_START_SWITCH_LOST_EFA;
		}
		else
		{
			alarmType = ALARM_ELC_UNKNOWN;
		}
		break;
	case AlarmEvent::ALARM_PC:
		alarmCode = PowerSupplyTask::GetCoreState();
		if ((alarmCode & BIT_MASK(PowerSupplyTask::POWER_OVERLOAD)) == BIT_MASK(PowerSupplyTask::POWER_OVERLOAD))
			alarmType = ALARM_POWER_OVERLOAD_OVA;
		else
			alarmType = ALARM_ELC_UNKNOWN;
		break;
	default:
		break;
	}
	return alarmType;
}
