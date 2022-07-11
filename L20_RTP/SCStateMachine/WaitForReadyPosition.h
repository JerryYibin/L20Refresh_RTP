/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef WAITFORREADYPOSITION_H_
#define WAITFORREADYPOSITION_H_

#include "SCState.h"

class WaitForReadyPosition: public SCState {
public:
	WaitForReadyPosition();
	virtual ~WaitForReadyPosition();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* WAITFORREADYPOSITION_H_ */
