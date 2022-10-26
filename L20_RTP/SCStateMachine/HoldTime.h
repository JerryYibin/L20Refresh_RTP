/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef HOLDTIME_H_
#define HOLDTIME_H_

#include "SCState.h"

class HoldTime: public SCState {
private:
	unsigned int m_iHoldTime;
public:
	HoldTime();
	virtual ~HoldTime();
public:
	virtual void Enter	()	override;
	virtual void Loop	()	override;
	virtual void Exit	()	override;
	virtual void Fail	()	override;
};

#endif /* HOLDTIME_H_ */
