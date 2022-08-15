/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/

#ifndef PSWELDRUN_H_
#define PSWELDRUN_H_

#include "../PCState.h"

class PCWeldRun: public PCState 
{
public:
	PCWeldRun();
	virtual 			~PCWeldRun	();
protected:
	virtual void 		Enter		() override;
	virtual void		Loop		() override;
	virtual void		Exit		() override;
	
};

#endif /* PSWELDRUN_H_ */
