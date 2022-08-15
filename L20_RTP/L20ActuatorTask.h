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
#define MIN_DOWN_SPEED	600 //3mm/s
class L20ActuatorTask: public ActuatorTask 
{
private:
	static CommunicationInterface_CAN* 				_ObjDCan;
	static ACStateMachine::RxPDO_AC					RXBackup;
	static UINT32									Tick_1ms;
	UINT32											MaxSpeed;
	UINT32 											MinMoveCount;
	UINT32											OldPosition;
	UINT32 											DeltaPosition[4];
	CommunicationInterface_CAN::TX_MESSAGE 			m_Pressure;
	UINT32											m_ActualUpTime;
	UINT32											m_ActualDownTime;
	UINT32											m_ActualTotalTime;
	bool											m_IsMoving;
public:
	L20ActuatorTask();
	virtual				~L20ActuatorTask			();
	virtual void		PDOUploadRequest			() override;
	virtual void		PDODownloadRequest			() override;
	bool				IsMoving					();
	unsigned int		GetMaxSpeed					();
private:
	bool		 		MovingCheckProcess			();
	unsigned int		WeightedAverageSpeed		(unsigned int EncoderPosition);
	void				InitMovingProcess			();
};

#endif /* L20ACTUATORTASK_H_ */
