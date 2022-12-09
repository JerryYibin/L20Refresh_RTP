/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef ESTOP_H_
#define ESTOP_H_
#include "SCState.h"
#define ESTOPBEEPDELAY 1000
class Estop: public SCState {
public:
	Estop();
	virtual ~Estop();
public:
	virtual void Enter	()	override;
	virtual void Loop	()	override;
	virtual void Exit	()	override;
	virtual void Fail	()	override;
};

#endif /* ESTOP_H_ */
