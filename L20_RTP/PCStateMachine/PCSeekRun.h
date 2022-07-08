/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/

#ifndef PSSEEKRUN_H_
#define PSSEEKRUN_H_

#include "PCState.h"

class PCSeekRun: public PCState 
{
public:
	PCSeekRun();
	virtual ~PCSeekRun();
protected:
	virtual void 		Enter		();
	virtual void		Loop		();
	virtual void		Exit		();
};

#endif /* PSSEEKRUN_H_ */
