/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef CONTROLTASK_H_
#define CONTROLTASK_H_

#include "SCTask.h"
#include "WorkFlow.h"

/*
 *
 */
class ControlTask: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to UIC */
		TO_CTRL_OPERATE_MODE_SET = 0,
		TO_CTRL_TRIGGER_HEIGHT_CALIBRATE,
		TO_CTRL_TRIGGER_HEIGHT_CHECK,
		TO_CTRL_HEIGHT_CORRECT,
		TO_CTRL_SC_RESPONSE
	};
	
public:
	ControlTask();
	~ControlTask();
	
	static	void 	Control_Task				(void);
protected:
	virtual	void	ProcessTaskMessage			(MESSAGE& message) override;	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID 				UI_MSG_Q_ID;
	MSG_Q_ID 				DATA_MSG_Q_ID_CTRL;
	
	int						m_OperationMode;
	static WorkFlow*		_WorkFlowObj;				
private:
	void			updateOperationMode				(char*);
	void 			initHeightCalibrateProcess		(void);
	void			triggerHeightCalibrateProcess 	(unsigned int pressure);
	void			responseStateMachineProcess		(void);
	void			updateHeightMapProcess			(void);
	void			calculateHeightPoint			(unsigned int min, unsigned int max, unsigned int start, unsigned int end);
};

#endif /* CONTROLTASK_H_ */
