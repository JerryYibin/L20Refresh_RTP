/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ACTUATORTASK_H_
#define ACTUATORTASK_H_

#include "SCTask.h"
#define DEBOUNCE_TIME	50 //50ms
class ActuatorTask: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to UIC */
		TO_ACT_TASK_PRESSURE_SET = 0,
	};
	ActuatorTask();
	virtual					~ActuatorTask();
	void 					ProcessTaskMessage					(MESSAGE& message) override;
	bool					GetPB1								();
	
	virtual void			PDOUploadRequest 					() = 0;
	virtual void 			PDODownloadRequest					() = 0;
	virtual bool			IsMoving							() = 0;
	virtual unsigned int	GetMaxSpeed							() = 0;
	virtual void			InitHeightSystem					() = 0;
	virtual void 			ScanInputs							() = 0;
	
	static unsigned int		GetCoreState						();
	static void				SetCoreState						(unsigned int coreState);
	static void				ClearCoreState						(unsigned int coreState);
	
	static ActuatorTask*	GetInstance							();	
	
	static void				Actuator_System_Task				(void);
protected:
	MSG_Q_ID		CTRL_MSG_Q_ID;
	MSG_Q_ID		UI_MSG_Q_ID;
	MSG_Q_ID 		MAINTENANCE_MSG_Q_ID;
	
	static unsigned int CoreState;
	static ActuatorTask* _ACObj;
	static UINT32		Tick_1ms;
private:
	int					m_DebounceCount;
	unsigned int 		m_PB1;
};

#endif /* ACTUATORTASK_H_ */
