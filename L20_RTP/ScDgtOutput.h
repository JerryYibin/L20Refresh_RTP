/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SCDGTOUTPUTTASK_H_
#define SCDGTOUTPUTTASK_H_

#include "SCTask.h"

class ScDgtOutputTask : public SCTask
{
public:
	enum MESSAGE_IDENTIFY
	{
		TO_DGT_OUTPUT_TASK_READY_SET,
		TO_DGT_OUTPUT_TASK_SONICS_SET,
		TO_DGT_OUTPUT_TASK_ALARM_SET
	};

	ScDgtOutputTask();
	~ScDgtOutputTask();
	
	static void				ScDgtOutput_Task(void);
protected:
	virtual void 			ProcessTaskMessage(MESSAGE& message) override;
private:
    MSG_Q_ID 	SELF_MSG_Q_ID;
    MSG_Q_ID	UI_MSG_Q_ID;

};

#endif /* SCDGTOUTPUTTASK_H_ */
