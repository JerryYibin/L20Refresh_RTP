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
public:
	DataTask();
	~DataTask();
	
	static void Data_Task(void);
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;
private:
    int 		ConnectDB								();
    int 		CloseDB									(); 
    void		processTaskControlMessage				();
    void		processTaskDataMessage					();
    void		processTaskRequestMessage				();
    
private:
    MSG_Q_ID 			SELF_MSG_Q_ID_DATA;
    MSG_Q_ID 			SELF_MSG_Q_ID_REQUEST;
	MSG_Q_ID			SELF_MSG_Q_ID_CTRL;
	MSG_Q_ID			UI_MSG_Q_ID;
	MSG_Q_ID			INTERFACE_MSG_Q_ID;
	MSG_Q_ID			CTRL_MSG_Q_ID;
	
	DBAccess			*m_DbConn;
};

#endif /* DATATASK_H_ */
