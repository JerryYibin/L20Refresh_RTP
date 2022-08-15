/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef WAITFORTRIGGER_H_
#define WAITFORTRIGGER_H_

#include "SCState.h"
#define DELAY6SEC 6000
#define MINHORNDOWNSPEED 30 //3mm/s
class WaitForTrigger: public SCState {

public:
	WaitForTrigger();
	virtual ~WaitForTrigger();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
};

#endif /* WAITFORTRIGGER_H_ */
