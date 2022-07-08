/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/

#ifndef PSESTOP_H_
#define PSESTOP_H_

#include "PCState.h"

class PCEstop: public PCState 
{
public:
	PCEstop();
	virtual 			~PCEstop	();
protected:
	virtual void 		Enter		();
	virtual void		Loop		();
	virtual void		Exit		();
};

#endif /* PSESTOP_H_ */
