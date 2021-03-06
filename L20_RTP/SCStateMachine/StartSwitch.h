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
#define DELAY50MSEC 50
#define DELAY250MSEC 250
class StartSwitch: public SCState {
public:
	enum PBSTATE
	{
		WAIT_PRESSED = 0,
		HOLD_PRESSED_DEBOUNCE,
		STILL_PRESSED
	};
private:
	PBSTATE m_PBState;
public:
	StartSwitch();
	virtual ~StartSwitch();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* STARTSWITCH_H_ */
