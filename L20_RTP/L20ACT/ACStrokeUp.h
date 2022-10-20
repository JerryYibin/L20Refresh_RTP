/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACSTROKEUP_H_
#define ACSTROKEUP_H_

#include "../ACState.h"
class ACStrokeUp: public ACState {
public:
	ACStrokeUp();
	virtual ~ACStrokeUp();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
private:
	unsigned int 		m_MovedCount;
};

#endif /* ACSTATEMACHINE_ACSTROKEUP_H_ */
