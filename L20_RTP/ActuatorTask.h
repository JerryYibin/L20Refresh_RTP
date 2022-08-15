/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ACTUATORTASK_H_
#define ACTUATORTASK_H_

#include "SCTask.h"

class ActuatorTask: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to UIC */
		TO_ACT_TASK_PRESSURE_SET,
	};
	ActuatorTask();
	virtual				~ActuatorTask();
	
	void 				ProcessTaskMessage					(MESSAGE& message) override;
	
	virtual void		PDOUploadRequest 					() = 0;
	virtual void 		PDODownloadRequest					() = 0;
	
	static unsigned int	GetCoreState						();
	static void			SetCoreState						(unsigned int coreState);
	static bool			IsMoving							();
	static unsigned int GetMaxSpeed							();	
	
	static void			Actuator_System_Task				(void);
private:
	MSG_Q_ID		CTRL_MSG_Q_ID;
	MSG_Q_ID		UI_MSG_Q_ID;
	MSG_Q_ID 		MAINTENANCE_MSG_Q_ID;
	
	static unsigned int CoreState;
	static ActuatorTask* _ACObj;
	
};

#endif /* ACTUATORTASK_H_ */
