/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SCDGTINPUT_H_
#define SCDGTINPUT_H_

#include "SCTask.h"

class ScDgtInputTask: public SCTask 
{
public:
	ScDgtInputTask();
	~ScDgtInputTask();
	
	static void				ScDgtInput_Task(void);
protected:
	virtual void 			ProcessTaskMessage(MESSAGE& message) override;
private:
    
    MSG_Q_ID 			SELF_MSG_Q_ID;
    MSG_Q_ID			UI_MSG_Q_ID;

};

#endif /* SCDGTINPUT_H_ */
