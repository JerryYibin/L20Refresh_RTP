/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACWELDHOLD_H_
#define ACWELDHOLD_H_

#include "../ACState.h"

class ACWeldHold: public ACState {
public:
	ACWeldHold();
	virtual ~ACWeldHold();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
};

#endif /* ACSTATEMACHINE_ACWELDHOLD_H_ */
