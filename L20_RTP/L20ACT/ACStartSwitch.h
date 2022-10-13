/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef L20ACT_ACSTARTSWITCH_H_
#define L20ACT_ACSTARTSWITCH_H_

#include "../ACState.h"
#define DELAY50MSEC 50
#define DELAY250MSEC 250
class ACStartSwitch: public ACState 
{
public:
	ACStartSwitch();
	virtual ~ACStartSwitch();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
	
private:
	unsigned int 		m_PB1;
};







#endif /* L20ACT_ACSTARTSWITCH_H_ */
