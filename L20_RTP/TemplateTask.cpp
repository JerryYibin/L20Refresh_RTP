/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Database Interface task for DB related read or write operations  
 
**********************************************************************************************************/

#include "TemplateTask.h"

extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of UserInterface.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
TemplateTask::TemplateTask()
	:SELF_MSG_Q_ID(0)
	,RELATE_MSG_Q_ID(0)
{
	// Load the data message Q name
//	SELF_MSG_Q_ID = CP->getMsgQId(/*Task Name*/);
//	RELATE_MSG_Q_ID = CP->getMsgQId(/*Task Name*/);
	

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
TemplateTask::~TemplateTask() 
{

}

/*************************************************************************//**
 * \brief   - Process the received message from UI_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void TemplateTask::ProcessTaskMessage(MESSAGE& message)
{

	switch(message.msgID)
	{
	default:
		LOGERR((char *)"Template_T : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
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
void TemplateTask::Decode(char *pRecvBuffer, MESSAGE& ReqMessage)
{
	memcpy(&ReqMessage.msgID, pRecvBuffer, sizeof(ReqMessage.msgID));
	memcpy(ReqMessage.Buffer, pRecvBuffer + sizeof(ReqMessage.msgID), MAX_SIZE_OF_MSG_LENGTH - sizeof(ReqMessage.msgID));
}


/**************************************************************************//**
* 
* \brief   - Retrieve the Control task's message queue ID.  
*
* \param   - None.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
MSG_Q_ID TemplateTask::GetMsgQID() const
{
	return SELF_MSG_Q_ID;
}

/**************************************************************************//**
* 
* \brief   - Processing the Database Interface Data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void TemplateTask::Template_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	MSG_Q_ID 	SELF_MSG_Q_ID;

	TemplateTask *Temp = new(nothrow) TemplateTask();
	if(NULL != Temp)
	{
		SELF_MSG_Q_ID = Temp->GetMsgQID();
		while(Temp->bIsTaskRunStatus())
		{
			if(msgQReceive(SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
				Temp->Decode(MsgQBuffer, ProcessBuffer);
				Temp->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete Temp;
	}
	else
	{
		LOGERR((char *)"DATA_INTERFACE_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	Temp = NULL;
	taskSuspend(taskIdSelf());
}
