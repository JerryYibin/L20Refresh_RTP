/*
 * ACStartSwitch.h
 *
 *  Created on: Jul 29, 2022
 *      Author: E1389159
 */

#ifndef L20ACT_ACSTARTSWITCH_H_
#define L20ACT_ACSTARTSWITCH_H_

#include "../ACState.h"
#define DELAY50MSEC 50
#define DELAY250MSEC 250
class ACStartSwitch: public ACState {
private:
	enum PBSTATE
	{
		WAIT_PRESSED = 0,
		HOLD_PRESSED_DEBOUNCE,
		STILL_PRESSED
	};
	PBSTATE m_PBState;
public:
	ACStartSwitch();
	virtual ~ACStartSwitch();
protected:
	virtual void 		Enter();
	virtual void		Loop();
	virtual void		Exit();
	
private:
	bool 		m_StartPress;
	bool		m_StillPressCheck;
};







#endif /* L20ACT_ACSTARTSWITCH_H_ */
