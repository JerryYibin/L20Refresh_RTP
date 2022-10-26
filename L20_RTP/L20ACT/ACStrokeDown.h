/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACSTROKEDOWN_H_
#define ACSTROKEDOWN_H_

#include "../ACState.h"
#define MOVE_NOISE	1000/5 //1.00mm
class ACStrokeDown: public ACState {
public:
	ACStrokeDown();
	virtual ~ACStrokeDown();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
private:
	unsigned int 		m_MovedCount;

};

#endif /* ACSTATEMACHINE_ACSTROKEDOWN_H_ */
