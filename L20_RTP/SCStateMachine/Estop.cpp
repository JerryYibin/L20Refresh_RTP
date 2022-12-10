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

#include "Estop.h"
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
Estop::Estop() {
	m_Actions = SCState::INIT;
	m_State = SCState::ESTOP;

}

/**************************************************************************//**
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Estop::~Estop() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}

/**************************************************************************//**
*
* \brief   - E-Stop State Enter. If there is E-Stop detected, 
* 			The function needs to set PC state machine into E-Stop state¡£
* 			Register the E-Stop event into Alarm Manager and Save E-Stop event into Database using message queue. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Estop::Enter()
{
	if((ACStateMachine::AC_TX->ACState != ACState::AC_ESTOP) && (PCStateMachine::PC_TX->PCState != PCState::PC_ESTOP))
	{
		return;
	}
	else if(ACStateMachine::AC_TX->ACState == ACState::AC_ESTOP)
	{
		PCStateMachine::PC_RX->MasterEvents |= BIT_MASK(PCState::CTRL_ESTOP);
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
	
		event.m_Type = ALARM_ESTOP_NCA;
		//Register the alarm into alarm list of alarm manager. 
		AlarmManager::GetInstance()->EnterAlarmEvent(&event);
		SendMsgToCtrlMsgQ(ControlTask::TO_CTRL_ALARM_EVENT, (const char*)&event);
		m_Timeout = 0;
	}
}

/**************************************************************************//**
*
* \brief   - EStop Loop. Handle with Beeper delay.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Estop::Loop()
{
	if (m_Timeout < ESTOPBEEPDELAY)
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
	if((ACStateMachine::AC_TX->ACState == ACState::AC_READY) && (PCStateMachine::PC_TX->PCState == PCState::PC_ESTOP)) 
	{
		PCStateMachine::PC_RX->MasterEvents &= ~BIT_MASK(PCState::CTRL_ESTOP);
	}
	else if((ACStateMachine::AC_TX->ACState == ACState::AC_READY) && (PCStateMachine::PC_TX->PCState == PCState::PC_READY)) 
	{
		m_Actions = SCState::JUMP;
	}
}

/**************************************************************************//**
*
* \brief   - EStop Exit close Beeper and Clear EStop alarm 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Estop::Exit()
{
	vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
	AlarmManager::GetInstance()->ClearEStopEvent();
	m_Timeout = 0;
}

/**************************************************************************//**
*
* \brief   - E-Stop Fail
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Estop::Fail()
{

}
