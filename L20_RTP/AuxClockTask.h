/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
**********************************************************************************************************/

#ifndef AUXCLOCKTASK_H_
#define AUXCLOCKTASK_H_

#include "SCTask.h"
#define O_TEST_PIN 0x5d //GPIO3_29
class AuxClockTask: public SCTask 
{
public:
static AuxClockTask*	getInstance			();
	static	void 		deleteInstance		();
	static	void		AuxClock_Task		(void* _obj);
protected:
	virtual	void        ProcessTaskMessage	(MESSAGE& message){} 
private:
	static AuxClockTask	*m_AuxTaskObj;
	AuxClockTask();
	virtual ~AuxClockTask();
	/* Following code is only for the auxiliary clock debug testing */
	void debugFlipGPIOLevel();
};

#endif /* AUXCLOCKTASK_H_ */
