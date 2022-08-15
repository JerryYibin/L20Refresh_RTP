/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
  
***************************************************************************/

#ifndef PSALARM_H_
#define PSALARM_H_

#include "../PCState.h"

class PCAlarm: public PCState 
{
public:
	PCAlarm();
	virtual 			~PCAlarm	();
protected:
	virtual void 		Enter		();
	virtual void		Loop		();
	virtual void		Exit		();
};

#endif /* PSALARM_H_ */
