/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "SCTask.h"

class TemplateTask: public SCTask 
{
public:
	TemplateTask();
	~TemplateTask();
	
	MSG_Q_ID		GetMsgQID							() const;
	void 			ProcessTaskMessage					(MESSAGE& message);
	void 			Decode								(char *pRecvBuffer, MESSAGE& ProcessBuffer);
	
	static void Template_Task(void);
private:

private:
    
    MSG_Q_ID 			SELF_MSG_Q_ID;
    MSG_Q_ID			RELATE_MSG_Q_ID;

};

#endif /* DBINTERFACE_H_ */
