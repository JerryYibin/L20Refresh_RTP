/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef L20ACTUATORTASK_H_
#define L20ACTUATORTASK_H_

#include "ActuatorTask.h"
#include "CommunicationInterfaceCAN.h"
#include "ACStateMachine.h"
#define MIN_DOWN_SPEED	6000 //3mm/s
class L20ActuatorTask: public ActuatorTask 
{
private:
	struct COOLING
	{
		UINT32 Delay;
		UINT32 Duration;
	};
private:
	static CommunicationInterface_CAN* 				_ObjDCan;
	static ACStateMachine::RxPDO_AC					RXBackup;
	UINT32											MaxSpeed;
	UINT32 											MinMoveCount;
	INT32											OldPosition;
	INT32 											DeltaPosition[4];
	CommunicationInterface_CAN::TX_MESSAGE 			m_Pressure;
	UINT32											m_ActualUpTime;
	UINT32											m_ActualDownTime;
	UINT32											m_ActualTotalTime;
	bool											m_IsMoving;
	static COOLING	CoolingTimer;
public:
							L20ActuatorTask();
	virtual					~L20ActuatorTask			();
	virtual void			PDOUploadRequest			() override;
	virtual void			PDODownloadRequest			() override;
	virtual bool			IsMoving					() override;
	virtual unsigned int	GetMaxSpeed					() override;
	virtual void			InitHeightSystem			() override;
	virtual void			ScanInputs					() override;
	virtual	void		 	DoAuxMotion					(int motion) override;
	virtual void			SetCoolingTimer				() override;
	virtual void		 	ResetCoolingTimer			() override;
private:
	bool		 			MovingCheckProcess			();
	unsigned int			WeightedAverageSpeed		(unsigned int EncoderPosition);
	void					InitMovingProcess			();
	void					RunCoolingTimer				();
};

#endif /* L20ACTUATORTASK_H_ */
