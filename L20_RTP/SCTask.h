/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
SCTask is the parent class for each specific task classes so 
Decode function should be same with each child task, 
and we should have the a unify ProcessTaskMessage interface definition in here.  
 
***************************************************************************/

#ifndef SCTASK_H_
#define SCTASK_H_

#include "Common.h"
#include "CommonProperty.h"
#include "HMI_LRC_comms.h"

class SCTask
{
public:
	enum TASK_MSG_PRIORITY
	{
		PRIORITY_CONTROL,
		PRIORITY_DATA,
		PRIORITY_REQUEST
	};
public:
	SCTask(){
		SELF_MSG_Q_ID = MSG_Q_ID_NULL;
		CP = CommonProperty::getInstance();
	};
	virtual ~SCTask(){};
	virtual MSG_Q_ID		GetMsgQID				() const;
	virtual bool			bIsTaskRunStatus		() {return CP->m_bTaskRun;}
protected:
	MSG_Q_ID 				SELF_MSG_Q_ID;
	virtual void 			Decode					(const char *pRecvBuffer, MESSAGE& ReqMessage);
	virtual STATUS			SendToMsgQ 				(MESSAGE& msgBuffer, const MSG_Q_ID& msgQID, _Vx_ticks_t waitType = NO_WAIT);
	virtual void			ProcessTaskMessage		(MESSAGE& message) = 0;	
	CommonProperty 			*CP;
};



#endif /* SCTASK_H_ */
