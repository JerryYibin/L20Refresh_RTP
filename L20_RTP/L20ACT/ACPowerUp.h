/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACPOWERUP_H_
#define ACPOWERUP_H_

#include "../ACState.h"

class ACPowerUp: public ACState {
public:
	ACPowerUp();
	virtual ~ACPowerUp();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
};

#endif /* ACSTATEMACHINE_ACPOWERUP_H_ */
