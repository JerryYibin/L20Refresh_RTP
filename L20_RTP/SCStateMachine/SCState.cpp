/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC State
***************************************************************************/

#include "SCState.h"

/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
SCState::SCState() {
	// TODO Auto-generated constructor stub
	CP = CommonProperty::getInstance();
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	CTL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
}
/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SCState::~SCState() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Abort Weld.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
//TODO Consider to not directly call hardware function here
void SCState::abortWeld(void)
{
	///* Clear outputs and panel data */
	vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
	///* Clear SONICSRUN, SONICSRESET and SONICSTEST */
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OL_RST_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_TEST_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_GATHER, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_CLAMP, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SEEK, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SAFETY, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OUT0, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OUT1, GPIO_VALUE_LOW);
	
	//SetPressure(ActivePreset.m_WeldParam.TPressure);
}

/**************************************************************************//**
* 
* \brief   - Abort Weld.
*
* \param   - None.
*
* \return  - bool.
*
******************************************************************************/
bool SCState::DefeatWeldAbortHandler()
{
	bool isAbort = false;
//	if (SysConfig.m_SystemInfo.FootPedalAbort == true)
//	{
		if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH || vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
		{
			abortWeld();
			CommonProperty::WeldResult.ALARMS.AlarmFlags.WeldAborted = 1;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.FootPedalAbort = 1;
			isAbort = true;
		}
//	}
	return isAbort;
}
/**************************************************************************//**
* 
* \brief   - Abort Weld.
*
* \param   - Process Alarm
*
* \return  - bool.
*
******************************************************************************/
bool SCState::ProcessAlarmHandler(void)
{
	bool isReset = false;
	if(CommonProperty::WeldResult.ALARMS.AlarmFlags.WeldAborted != 0)
	{
		if(m_Timeout < ALARMBEEPDELAY)
		{
			m_Timeout++;
		}
		else
		{
			if(vxbGpioGetValue(GPIO::O_BUZZ) == GPIO_VALUE_HIGH)
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
			else
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_HIGH);
			m_Timeout = 0;
		}
	}
	else
		isReset = true;
	return isReset;
}

/**************************************************************************//**
* \brief   - Post message into message queue based on Q-ID.
*
* \param   - MSG_Q_ID msgQID, Message& msg.
*
* \return  - None.
*
******************************************************************************/
STATUS SCState::SendToMsgQ(MESSAGE& msgBuffer, const MSG_Q_ID& msgQID, _Vx_ticks_t waitType)
{
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	STATUS status = ERROR;
	
	if(msgQSend(msgQID, reinterpret_cast<char*>(&msgBuffer), sizeof(msgBuffer), waitType, MSG_PRI_NORMAL) == OK)
	{
//		printf("\nSCTask::SendToMsg: %d OK\n",msgQID);
//		printf("\nCP->GetMsgQId(Data_Task Control): %d OK\n",CP->GetMsgQId(Data_Task + "/Control"));
//		printf("\nCP->GetMsgQId(Data_Task Data): %d OK\n",CP->GetMsgQId(Data_Task + "/Data"));
//		printf("\nCP->GetMsgQId(Data_Task Request): %d OK\n",CP->GetMsgQId(Data_Task + "/Request"));
		
		if (msgQID == CP->getMsgQId(Data_Task + "/Control") || msgQID == CP->getMsgQId(Data_Task + "/Data") || msgQID == CP->getMsgQId(Data_Task + "/Request"))
		{		
			if(eventSend (CP->getTaskId(CommonProperty::cTaskName[CommonProperty::DATA_T]), DATA_TASK_EVENT) == OK)
				status = OK;
			else
				LOGERR((char *) "SCTask : SendToMsgQ: eventSend Error\n",0,0,0);
		}
		else
			status = OK;
	}
	else
		LOGERR((char *) "SCTask : SendToMsgQ: msgQSend Error\n",0,0,0);

	return status;
}
