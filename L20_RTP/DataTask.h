/*
 * DataTask.h
 *
 *  Created on: Jan 27, 2022
 *      Author: JerryW.Wang
 */

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
	enum MESSAGE_TYPE
	{
		CONTRL,
		REQUEST,
		DATA
	};
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to DATA TASK */
		TO_DATA_TASK_WELD_RESULT_INSERT 				= 0,
		TO_DATA_TASK_WELD_SIGN_INSERT,
		TO_DATA_TASK_QUERY,
		TO_DATA_TASK_UPDATE,
		TO_DATA_TASK_DELETE,
		TO_DATA_TASK_CLEAR
	};
public:
	DataTask();
	~DataTask();
	
	static void Data_Task(void);
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;
private:
    int 		ConnectDB								();
    int 		CloseDB									(); 
    
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
