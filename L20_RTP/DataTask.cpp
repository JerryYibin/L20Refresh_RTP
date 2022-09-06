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
#undef PERFORMANCE_MEASURE
#ifdef PERFORMANCE_MEASURE
#include <timerDev.h>
#endif
#include "DataTask.h"
#include "Database/DBAccess_l20_db.h"
extern "C"
{
	#include "customSystemCall.h"	
}

DBAccess* DataTask::_ObjDBConn = nullptr;

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
	int nErrCode = SQLITE_OK;
    if(_ObjDBConn == nullptr)
        {
    	_ObjDBConn = new DBAccessL20DB();
    	nErrCode = _ObjDBConn->ConnectDB();
    	if(nErrCode > SQLITE_OK)
    		LOGERR("DB Connection Open Error! ErrCode = %d\n",nErrCode, 0, 0);
        }
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
	int nErrCode = SQLITE_OK;
    if(_ObjDBConn != nullptr)
        {
    	nErrCode = _ObjDBConn->CloseDataBaseConnection();
    	if(nErrCode > SQLITE_OK)
    		LOGERR("DB Close Error! ErrCode = %d\n",nErrCode, 0, 0);
        else
            _ObjDBConn = nullptr;
        }
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
	switch(message.msgID)
	{
	case TO_DATA_TASK_OPEN_DB:
	    ConnectDB();
        break;
	case TO_DATA_TASK_CLOSE_DB:
	    CloseDB();
        break;
	case TO_DATA_TASK_WELD_RECIPE_INSERT:
		{
#ifdef PERFORMANCE_MEASURE
		UINT32 m_startTime = sysTimestampLock();
#endif 
		_ObjDBConn->StoreWeldRecipe(message.Buffer);
#ifdef PERFORMANCE_MEASURE
        {
		UINT32 m_endTime = sysTimestampLock();
        printf("single StoreWeldRecipe took %u microseconds\n", (m_endTime-m_startTime)*1000000/sysTimestampFreq());
        }
#endif
		break;
		}
	case TO_DATA_TASK_WELD_RESULT_INSERT:
		{
#ifdef PERFORMANCE_MEASURE
		UINT32 m_startTime = sysTimestampLock();
#endif 
		_ObjDBConn->StoreWeldResult(message.Buffer);
#ifdef PERFORMANCE_MEASURE
        {
		UINT32 m_endTime = sysTimestampLock();
        printf("single StoreWeldResult took %u microseconds\n",
            (m_endTime-m_startTime)*1000000/sysTimestampFreq());
        }
#endif
		break;
		}
	case TO_DATA_TASK_WELD_SIGN_INSERT:
		_ObjDBConn->StoreWeldSignature(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_RESULT_QUERY:
        _ObjDBConn->QueryWeldResult(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_RECIPE_QUERY:
        _ObjDBConn->QueryWeldRecipe(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_SIGN_QUERY:
        _ObjDBConn->QueryWeldSignature(message.Buffer);
		break;
	case TO_DATA_TASK_WELD_RECIPE_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_RECIPE);
		break;
	case TO_DATA_TASK_WELD_RESULT_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_RESULT);
		break;
	case TO_DATA_TASK_WELD_SIGN_DELETE:
        _ObjDBConn->DeleteOldest(TABLE_WELD_SIGNATURE);
		break;
#ifdef UNITTEST_DATABASE
	case TO_DATA_TASK_WELD_RECIPE_CLEAR:
        _ObjDBConn->ClearTable(TABLE_WELD_RECIPE);
		break;
	case TO_DATA_TASK_WELD_RESULT_CLEAR:
        _ObjDBConn->ClearTable(TABLE_WELD_RESULT);
		break;
	case TO_DATA_TASK_WELD_SIGN_CLEAR:
        _ObjDBConn->ClearTable(TABLE_WELD_SIGNATURE);
		break;
#endif
	default:
		LOGERR((char *)"DataTask: --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
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
	bool		isRunning = true;
	
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	

	DataTask *DBInit = new(nothrow) DataTask();
	if(NULL != DBInit)
	{
		while(DBInit->bIsTaskRunStatus())
		{
			// wait for any one event
			if(eventReceive(DATA_TASK_EVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				isRunning = true;
				do{
					if(msgQReceive(DBInit->SELF_MSG_Q_ID_CTRL, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
					{
						DBInit->Decode(MsgQBuffer, ProcessBuffer);
						DBInit->ProcessTaskMessage(ProcessBuffer);
					}
					else if(msgQReceive(DBInit->SELF_MSG_Q_ID_DATA, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
					{
						DBInit->Decode(MsgQBuffer, ProcessBuffer);
						DBInit->ProcessTaskMessage(ProcessBuffer);
					}
					else if(msgQReceive(DBInit->SELF_MSG_Q_ID_REQUEST, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
					{
						DBInit->Decode(MsgQBuffer, ProcessBuffer);
						DBInit->ProcessTaskMessage(ProcessBuffer);
					}
					else
						isRunning = false;
				
				}while(isRunning);
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
