/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 SC State
***************************************************************************/

#include "SCState.h"
#include "../AlarmEvent.h"
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
	CTL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	DGTOUT_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T]);
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_CTRL = CP->getMsgQId(Data_Task + "/Control");
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

/**************************************************************************//**
* \brief   - With the weld cycle running, there are some output digital need to set and reset for external.
*
* \param   - ScDgtOutputTask::MESSAGE_IDENTIFY
*
* \return  - None.
*
******************************************************************************/
void SCState::ChangeExtDgtOutput(const ScDgtOutputTask::MESSAGE_IDENTIFY msgID)
{
	MESSAGE message;
	message.msgID = msgID;
	SendToMsgQ(message, DGTOUT_MSG_Q_ID);
}

/**************************************************************************//**
*
* \brief   - Send Message to Control Task MessageQueue
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SCState::SendMsgToCtrlMsgQ(const ControlTask::MESSAGE_IDENTIFY msgID, int alarmType)
{
	MESSAGE message;
	message.msgID = msgID;
	if(alarmType != 0)
	{
		memcpy(message.Buffer, &alarmType, sizeof(int));
	}
	SendToMsgQ(message, CTL_MSG_Q_ID);
}

/**************************************************************************//**
*
* \brief   - Send Message to Data Task MessageQueue
*
* \param   - message ID, alarmEvent object.
*
* \return  - None.
*
******************************************************************************/
void SCState::SendMsgToDataMsgQ(const DataTask::MESSAGE_IDENTIFY msgID, const char* _event)
{
	//Record the alarm into alarm table of database.
	MESSAGE message;
	message.msgID = msgID;
	memcpy(message.Buffer, _event, sizeof(AlarmEvent));
	SendToMsgQ(message, DATA_MSG_Q_ID_CTRL);
}
