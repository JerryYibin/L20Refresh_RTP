/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SETWELDPRESSURE_H_
#define SETWELDPRESSURE_H_

#include "SCState.h"
#define DELAY200MSEC 200
class SetWeldPressure: public SCState 
{
public:
	SetWeldPressure();
	~SetWeldPressure();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
};

#endif /* SCSTATEMACHINE_SETWELDPRESSURE_H_ */
