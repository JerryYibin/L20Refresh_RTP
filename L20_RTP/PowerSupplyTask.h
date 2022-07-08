/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef POWERSUPPLYTASK_H_
#define POWERSUPPLYTASK_H_

#include "SCTask.h"
class PowerSupplyTask: public SCTask 
{
public:
	enum PS_TX_STATUS
	{
		SONICS_ON_OFF_STATUS,
		POWER_OVERLOAD,    
		VOLTAGE_OVERLOAD,
		CURRENT_OVERLOAD,
		TEMPERATURE_OVERLOAD
	};
	
public:
	PowerSupplyTask();
	virtual				~PowerSupplyTask();
	
	void 				ProcessTaskMessage					(MESSAGE& message) override;
	
	virtual void		PDOUploadRequest 					() = 0;
	virtual void 		PDODownloadRequest					() = 0;
	
	static unsigned int	GetCoreState						();
	static void			SetCoreState						(unsigned int coreState);
	
	static void			PowerSupply_Task					(void);
private:
	MSG_Q_ID		CTRL_MSG_Q_ID;
	MSG_Q_ID 		MAINTENANCE_MSG_Q_ID;
	
	static unsigned int m_CoreState;
	
};

#endif /* POWERSUPPLYTASK_H_ */
