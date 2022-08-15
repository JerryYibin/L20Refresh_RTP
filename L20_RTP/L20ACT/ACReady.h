/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACREADY_H_
#define ACREADY_H_

#include "../ACState.h"

class ACReady: public ACState {
public:
	ACReady();
	virtual ~ACReady();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
};

#endif /* ACREADY_H_ */
