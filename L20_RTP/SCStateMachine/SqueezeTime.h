/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SQUEEZETIME_H_
#define SQUEEZETIME_H_

#include "SCState.h"

class SqueezeTime: public SCState {
private:
	unsigned int m_iSqueezeTime;
public:
	SqueezeTime();
	virtual ~SqueezeTime();
public:
	virtual void Enter	() 	override;
	virtual void Loop	() 	override;
	virtual void Exit	() 	override;
	virtual void Fail	() 	override;
};

#endif /* SQUEEZETIME_H_ */
