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
	ActuatorTask();
	virtual				~ActuatorTask();
	
	void 				ProcessTaskMessage					(MESSAGE& message) override;
	
	virtual void		PDOUploadRequest 					() = 0;
	virtual void 		PDODownloadRequest					() = 0;
	
	static unsigned int	GetCoreState						();
	static void			SetCoreState						(unsigned int coreState);
	
	static void			Actuator_System_Task				(void);
private:
	MSG_Q_ID		CTRL_MSG_Q_ID;
	MSG_Q_ID 		MAINTENANCE_MSG_Q_ID;
	
	static unsigned int CoreState;
	
};

#endif /* ACTUATORTASK_H_ */
