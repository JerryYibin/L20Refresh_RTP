/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef CONTROLTASK_H_
#define CONTROLTASK_H_

#include "SCTask.h"

/*
 *
 */
class ControlTask: public SCTask 
{
public:
	ControlTask();
	~ControlTask();
	
	static	void 	Control_Task				(void);
protected:
	virtual	void	ProcessTaskMessage			(MESSAGE& message) override;	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID 		UI_MSG_Q_ID;
	MSG_Q_ID 		DATA_MSG_Q_ID_CTRL;
};

#endif /* CONTROLTASK_H_ */
