/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Database task for DB related read or write operations  
     
Changed the C code "Data_Task" to c++ static function. 
It's a good change for Data_Task so Data_Task can access all the source of
DataTask class owned using the class object pointer. 
 
**********************************************************************************************************/

#include "DataTask.h"
#include "Database/DBAccess_l20_db.h"

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
DataTask::DataTask()
{
	ConnectDB();
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	SELF_MSG_Q_ID_CTRL = CP->getMsgQId(Data_Task + "/Control");
	SELF_MSG_Q_ID_DATA = CP->getMsgQId(Data_Task + "/Data");
	SELF_MSG_Q_ID_REQUEST = CP->getMsgQId(Data_Task + "/Request");
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	INTERFACE_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	

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
DataTask::~DataTask() 
{
	CloseDB();
}

/*************************************************************************//**
 * \brief   - Connect Database.
 *
 * \param   - None.
 *
 * \return  - None.
 *
 ******************************************************************************/
int DataTask::ConnectDB()
{
	int nErrCode = SQLITE_ERROR;
	m_DbConn = new DBAccessL20DB();
	nErrCode = m_DbConn->ConnectDB();
	if(nErrCode > SQLITE_OK)
		LOGERR("DB Connection Open Error! ErrCode = %d\n",nErrCode, 0, 0);
	return nErrCode;
}

/*************************************************************************//**
 * \brief   - Close Database
 *
 * \param   - None.
 *
 * \return  - None.
 *
 ******************************************************************************/
int DataTask::CloseDB()
{
	int nErrCode = SQLITE_ERROR;
	nErrCode = m_DbConn->CloseDataBaseConnection();
	if(nErrCode > SQLITE_OK)
		LOGERR("DB Close Error! ErrCode = %d\n",nErrCode, 0, 0);
	return nErrCode;
}

/*************************************************************************//**
 * \brief   - Process the received message from DATA_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::ProcessTaskMessage(MESSAGE& message)
{
	do
	{
		/* assume no loop is necessary */
		m_bMsgEmpty = true;

		/* process Control message firstly */
		processTaskControlMessage();

		/* then, prcess Data message */
		processTaskDataMessage();

		/* last, process Request message */
		processTaskRequestMessage();
	}while(!m_bMsgEmpty);
}

/*************************************************************************//**
 * \brief   - Process the received Data Control message from DATA_MSG_Q_ID_CTRL.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::processTaskControlMessage()
{
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE	tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));

	/* all Control message must be processed */
	while(msgQReceive(SELF_MSG_Q_ID_CTRL, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(tmpMsg.msgID)
		{
		default:
			LOGERR((char *)"Control_DB_T : --------Unknown Message ID----------- : ", tmpMsg.msgID, 0, 0);
			break;
		}
	}
}

/*************************************************************************//**
 * \brief   - Process the received Data message from DATA_MSG_Q_ID_DATA.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::processTaskDataMessage()
{
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE	tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));

	if(msgQReceive(SELF_MSG_Q_ID_DATA, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(tmpMsg.msgID)
		{
		default:
			LOGERR((char *)"Data_DB_T : --------Unknown Message ID----------- : ", tmpMsg.msgID, 0, 0);
			break;
		}
		/* The loop is necessary for checking Control message as long as a Data message is received */
        m_bMsgEmpty = false;
	}
}

/*************************************************************************//**
 * \brief   - Process the received Data message from DATA_MSG_Q_ID_REQUEST.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::processTaskRequestMessage()
{
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE	tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));
	/* if there is any Data message received, a new loop is needed to check Control message firstly */
	if(m_bMsgEmpty == true)
	{
		if(msgQReceive(SELF_MSG_Q_ID_REQUEST, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
		{
			Decode(tmpMsgBuffer, tmpMsg);
			switch(tmpMsg.msgID)
			{
			default:
				LOGERR((char *)"Request_DB_T : --------Unknown Message ID----------- : ", tmpMsg.msgID, 0, 0);
				break;
			}
			/* The loop is necessary for checking Control message and Data message as long as a Request message. */
			m_bMsgEmpty = false;
		}
	}
}

/**************************************************************************//**
* 
* \brief   - Processing the Database Data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void DataTask::Data_Task(void)
{
	MESSAGE		ProcessBuffer;	
	UINT32		events;

	DataTask *DBInit = new(nothrow) DataTask();
	if(NULL != DBInit)
	{
		while(DBInit->bIsTaskRunStatus())
		{
			// wait for any one event
			if(eventReceive(DATA_TASK_EVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				DBInit->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete DBInit;
	}
	else
	{
		LOGERR((char *)"DATA_T : ----------------Memory allocation failed----------------", 0, 0, 0);
	}
	DBInit = NULL;
	taskSuspend(taskIdSelf());
}
