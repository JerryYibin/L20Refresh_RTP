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
		TO_DGT_OUTPUT_TASK_READY_RESET,
		TO_DGT_OUTPUT_TASK_SONICS_SET,
		TO_DGT_OUTPUT_TASK_SONICS_RESET,
		TO_DGT_OUTPUT_TASK_ALARM_SET,
		TO_DGT_OUTPUT_TASK_ALARM_RESET
	};

	ScDgtOutputTask();
	~ScDgtOutputTask();
	static ScDgtOutputTask* GetInstance			(void);
	static void				ScDgtOutput_Task	(void);
protected:
	virtual int 			SetDgtReadyOutput	()	= 0;
	virtual int 			ResetDgtReadyOutput	()	= 0;
	virtual int 			SetDgtSonicsOutput	()	= 0;
	virtual int 			ResetDgtSonicsOutput()	= 0;
	virtual int 			SetDgtAlarmOutput	()	= 0;
	virtual int 			ResetDgtAlarmOutput	()	= 0;
	virtual void 			ProcessTaskMessage	(MESSAGE& message) override;
private:
    MSG_Q_ID 	SELF_MSG_Q_ID;
    MSG_Q_ID	UI_MSG_Q_ID;
    
    static ScDgtOutputTask* _DgtOutputObj;

};

#endif /* SCDGTOUTPUTTASK_H_ */
