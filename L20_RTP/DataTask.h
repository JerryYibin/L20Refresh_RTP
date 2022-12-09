/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Database task for DB related read or write operations    
 
**********************************************************************************************************/

#ifndef DATATASK_H_
#define DATATASK_H_

#include "SCTask.h"
#include "Database/DBAccess.h"
/*
 *
 */
class DataTask: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to DATA TASK */
    	TO_DATA_TASK_OPEN_DB,
    	TO_DATA_TASK_CLOSE_DB,

    	TO_DATA_TASK_WELD_RECIPE_QUERY_ALL,
		TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE,
		TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE,
    	TO_DATA_TASK_WELD_RECIPE_QUERY,
    	TO_DATA_TASK_WELD_RECIPE_INSERT,
    	TO_DATA_TASK_WELD_RECIPE_UPDATE,
    	TO_DATA_TASK_WELD_RECIPE_DELETE,
    	TO_DATA_TASK_WELD_RECIPE_CLEAR,
		TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC,
		TO_DATA_TASK_WELD_RECIPE_TOTAL_NUMBER,
		TO_DATA_TASK_WELD_RECIPE_RENAME,

    	TO_DATA_TASK_WELD_RESULT_QUERY,
		TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE,
		TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE,
    	TO_DATA_TASK_WELD_RESULT_INSERT,
    	TO_DATA_TASK_WELD_RESULT_DELETE,
    	TO_DATA_TASK_WELD_RESULT_CLEAR,

    	TO_DATA_TASK_WELD_SIGN_QUERY,
    	TO_DATA_TASK_WELD_SIGN_INSERT,
    	TO_DATA_TASK_WELD_SIGN_DELETE,
    	TO_DATA_TASK_WELD_SIGN_CLEAR,

        TO_DATA_TASK_ALARM_LOG_QUERY,
		TO_DATA_TASK_ALARM_LOG_QUERY_LATEST_PAGE,
		TO_DATA_TASK_ALARM_LOG_QUERY_NEXT_PAGE,
        TO_DATA_TASK_ALARM_LOG_INSERT,
        TO_DATA_TASK_ALARM_LOG_CLEAR,
    	TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY,
    	TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE,

    	TO_DATA_TASK_DB_VERSION_QUERY,

    	TO_DATA_TASK_USER_PROFILE_QUERY,
    	TO_DATA_TASK_USER_PROFILE_UPDATE,

    	TO_DATA_TASK_PRIVILEGE_CONFIG_QUERY,
    	TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE,

    	TO_DATA_TASK_PWR_SUPPLY_QUERY,
    	TO_DATA_TASK_PWR_SUPPLY_UPDATE,

    	TO_DATA_TASK_TEACH_MODE_SET_QUERY,
    	TO_DATA_TASK_TEACH_MODE_SET_UPDATE,

    	TO_DATA_TASK_SYS_CONFIG_QUERY,
    	TO_DATA_TASK_SYS_CONFIG_UPDATE,

		TO_DATA_TASK_ACTIVE_RECIPE_QUERY,
    	TO_DATA_TASK_ACTIVE_RECIPE_UPDATE,
		TO_DATA_TASK_ETHERNET_CONFIG_UPDATE,
	};
public:
	DataTask();
	~DataTask();
	
	static void Data_Task(void);
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;
private:
    int 		ConnectDB					();
    int 		CloseDB						(); 
    int			InitData					();
    void		sendErrorCode				(int ErrCode);
    void		sendTotalNumber				(int Number);
    
private:
    MSG_Q_ID 			SELF_MSG_Q_ID_DATA;
    MSG_Q_ID 			SELF_MSG_Q_ID_REQUEST;
	MSG_Q_ID			SELF_MSG_Q_ID_CTRL;
	MSG_Q_ID			UI_MSG_Q_ID;
	MSG_Q_ID			INTERFACE_MSG_Q_ID;
	MSG_Q_ID			CTRL_MSG_Q_ID;
	
	static DBAccess		*_ObjDBConn;
	bool				m_bMsgEmpty;
};

#endif /* DATATASK_H_ */
