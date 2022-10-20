/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACALARM_H_
#define ACALARM_H_

#include "../ACState.h"

class ACAlarm: public ACState {
public:
	ACAlarm();
	virtual ~ACAlarm();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
};

#endif /* ACALARM_H_ */
