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
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to PS */
		TO_PS_TASK_AMPLITUDE_UPDATE = 0,
	};
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
	virtual						~PowerSupplyTask();
	
	void 						ProcessTaskMessage				(MESSAGE& message) override;
	
	virtual void				PDOUploadRequest 				() = 0;
	virtual void 				PDODownloadRequest				() = 0;
	virtual void				CalculateStartFrequency			() = 0;
	
	static unsigned int			GetCoreState					();
	static void					SetCoreState					(unsigned int coreState);
	static void					ClearCoreState					(unsigned int coreState);
	
	static PowerSupplyTask*		GetInstance						();
	
	static void					PowerSupply_Task				(void);
private:
	MSG_Q_ID				CTRL_MSG_Q_ID;
	MSG_Q_ID				UI_MSG_Q_ID;
	MSG_Q_ID 				MAINTENANCE_MSG_Q_ID;
	
	static unsigned int 	CoreState;
	static PowerSupplyTask*	_PCObj;
protected:
	static UINT32			Tick_1ms;
	
};

#endif /* POWERSUPPLYTASK_H_ */
