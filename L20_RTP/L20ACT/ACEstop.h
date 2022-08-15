/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACESTOP_H_
#define ACESTOP_H_

#include "../ACState.h"

class ACEstop: public ACState {
public:
	ACEstop();
	virtual ~ACEstop();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
};

#endif /* ACSTATEMACHINE_ACESTOP_H_ */
