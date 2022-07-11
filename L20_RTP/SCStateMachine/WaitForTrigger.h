/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef WAITFORTRIGGER_H_
#define WAITFORTRIGGER_H_

#include "SCState.h"
#define DELAY6SEC 6000
#define MINHORNDOWNSPEED 30 //3mm/s
class WaitForTrigger: public SCState {
public:
	enum HORNSTATE
	{
		WAIT_MOVE = 0,
		WAIT_TOUCH
	};
private:
	HORNSTATE m_HornState;
	unsigned int m_TriggerHeight;
	unsigned int m_StartHeight;
	unsigned int m_MaxSpeed;
	unsigned int m_MinMoveCount;
public:
	WaitForTrigger();
	virtual ~WaitForTrigger();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
private:
	unsigned int AverageSpeed(unsigned int EncoderPosition);
};

#endif /* WAITFORTRIGGER_H_ */
