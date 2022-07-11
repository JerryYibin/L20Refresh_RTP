/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACSTATE_H_
#define ACSTATE_H_
#include "../CommonProperty.h"
#include "../Utils/Utils.h"
#include "../GPIO.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
class ACState 
{
public:
	enum ACSTATES
	{
		AC_POWERUP,
		AC_READY,
		AC_DOWN_STROKE,
	// 	FORCE_CONTROL,
		AC_RETURN_STROKE,
		AC_ALARM,
	// 	FIND_PART_CONTACT,
		AC_WELD_HOLD,
	// 	HOMEING,
	// 	FORCE_CALIBRATION,
	// 	JOGGING_UP,
	// 	JOGGING_DOWN,
		AC_ESTOP,
		AC_GOING_TO_POSITION,
		AC_GOING_TO_HOME,
	// 	SERVO_RESET
		AC_CALIBRATE,
		AC_CHECK
	};
	
	enum AC_CONTROL_EVENTS
	{
		EVENT_STEP_FORCE,
		EVENT_AC_CLEAR_ALARMS
	};
	
	enum AC_STATUS_EVENTS
	{
		PART_CONTACT_FOUND,
		EN_FAULT,
		HOME_POS,
		READY_POS
	};
	
	enum ErrorTypeAC 
	{
	  ERR_NONE = 				0,
	  ERR_PART_WINDOW =			0x0001,
	  ERR_RECALIBRATE =			0x0004,
	};
public:
	ACState() {CP = CommonProperty::getInstance();}
	virtual 		~ACState	() {};
public:
	virtual void 		Enter			() = 0;
	virtual void  		Loop			() = 0;
	virtual void  		Exit			() = 0;
	
	virtual void 		ChangeState		(ACSTATES nextState);
	virtual ACSTATES 	GetStateType	() {return  stateType;}
	
	CommonProperty 	*CP;
	ACSTATES 		stateType;
};

#endif /* ACSTATE_H_ */
