/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef STARTSWITCH_H_
#define STARTSWITCH_H_

#include "SCState.h"

class StartSwitch: public SCState {
public:
	StartSwitch();
	virtual ~StartSwitch();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
};

#endif /* STARTSWITCH_H_ */
