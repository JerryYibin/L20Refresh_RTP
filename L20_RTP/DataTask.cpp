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
 * \brief   - Process the received message.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void DataTask::processMessage(MESSAGE_DB tmpMsg)
{
	switch(tmpMsg.msgData.cmd)
	{
	case CMD_INSERT:
        m_DbConn->dataInsert(tmpMsg.msgData.db, tmpMsg.msgData.data);
        break;
	case CMD_UPDATE:
        printf("DataTask: update data in db %d\n", tmpMsg.msgData.db);
        break;
	case CMD_QUERY:
        {
        char cmd[100];
        sprintf(cmd, "select * from %s order by ID desc limit 1;", m_DbConn->tableName[tmpMsg.msgData.db]);
        string str = m_DbConn->ExecuteQuery(cmd);
        printf("DataTask: query data from db %d: %s\n", tmpMsg.msgData.db, str.c_str());
        break;
        }
    case CMD_CLEAR:
        {
        char cmd[100];
        sprintf(cmd, "delete from %s;",
                m_DbConn->tableName[tmpMsg.msgData.db]);
        m_DbConn->ExecuteInsert(cmd);
        sprintf(cmd, "UPDATE sqlite_sequence SET seq = 0 WHERE name='%s';",
                m_DbConn->tableName[tmpMsg.msgData.db]);
        m_DbConn->ExecuteInsert(cmd);
        break;
        }
	default:
		LOGERR((char *)"DataTask: --------Unknown Message CMD----------- : ", tmpMsg.msgData.cmd, 0, 0);
		break;
	}
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
	MESSAGE_DB tmpMsg;
    while(1)
    {
	    if(msgQReceive(SELF_MSG_Q_ID_CTRL, (char *)&tmpMsg, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
        {
    		/* process Control message firstly */
    		processMessage(tmpMsg);
        }
        else if(msgQReceive(SELF_MSG_Q_ID_DATA, (char *)&tmpMsg, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
        {
    		/* then, prcess Data message */
    		processMessage(tmpMsg);
        }
        else if(msgQReceive(SELF_MSG_Q_ID_REQUEST, (char *)&tmpMsg, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
        {
    		/* last, process Request message */
    		processMessage(tmpMsg);
        }
        else
        {
    		/* no message at all */
            break;
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
