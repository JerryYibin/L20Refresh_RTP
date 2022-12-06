/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Task functions for ASC
 
**********************************************************************************************************/
#include "SCTask.h"

/**************************************************************************//**
* 
* \brief   - Retrieve the Control task's message queue ID.  
*
* \param   - None.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
MSG_Q_ID SCTask::GetMsgQID() const
{
	return SELF_MSG_Q_ID;
}

/**************************************************************************//**
* 
* \brief  - Decodes the received buffer from the message queue into a
* 			structured message.
*
* \param   - char *pRecvBuffer, MESSAGE& ReqMessage.
*
* \return  - None
*
******************************************************************************/
void SCTask::Decode(const char *pRecvBuffer, MESSAGE& ReqMessage)
{
	memcpy(&ReqMessage.msgID, pRecvBuffer, sizeof(ReqMessage.msgID));
	memcpy(ReqMessage.Buffer, pRecvBuffer + sizeof(ReqMessage.msgID), MAX_SIZE_OF_MSG_LENGTH - sizeof(ReqMessage.msgID));
}

/**************************************************************************//**
* \brief  - Posts the encoded message to the destination message queue of
* 			a given task  
*
* \param  - Message& msgBuffer
* \param  - MSG_Q_ID &msgQID
* \param  - _Vx_ticks_t waitType	
*
* \return  - STATUS status
*
******************************************************************************/
STATUS SCTask::SendToMsgQ (MESSAGE& msgBuffer, const MSG_Q_ID& msgQID, _Vx_ticks_t waitType)
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
				LOGERR((char *) "Data Task : SendToMsgQ: eventSend Error\n",0,0,0);
		}
		else if(msgQID == CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]))
		{
			if(eventSend (CP->getTaskId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]), ACT_TASK_QEVENT) == OK)
				status = OK;
			else
				LOGERR((char *) "Actuator Task : SendToMsgQ: eventSend Error\n",0,0,0);
		}
		else if(msgQID == CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]))
		{
			if(eventSend (CP->getTaskId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]), PS_TASK_QEVENT) == OK)
				status = OK;
			else
				LOGERR((char *) "PowerSupply Task : SendToMsgQ: eventSend Error\n",0,0,0);
		}
		else
			status = OK;
	}
	else
		LOGERR((char *) "SCTask : SendToMsgQ: msgQSend Error\n",0,0,0);

	return status;
}
 

