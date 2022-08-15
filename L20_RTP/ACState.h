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
#include "CommonProperty.h"
#include "Utils.h"
#include "GPIO.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
#define NO_ERROR				0
#define ERR_PART_WINDOW			BIT(1)
#define ERR_PRESSURE_SET		BIT(2)
#define ERR_RECALIBRATE			BIT(3)
#define ERR_HEIGHT_SYSTEM		BIT(4)
#define ERR_HOME_POSITION		BIT(5)
#define ERR_STARTSWITCH_LOST	BIT(6)
//#define ERR_HEIGHT_SENSOR	BIT(6)

#define HORN_MOVE_TIMEOUT			2000 
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
		AC_HOLD,
	// 	HOMEING,
	// 	FORCE_CALIBRATION,
	// 	JOGGING_UP,
	// 	JOGGING_DOWN,
		AC_ESTOP,
	// 	SERVO_RESET,
		AC_STARTSWICH
	};
	
	enum AC_CONTROL_EVENTS
	{
		CTRL_AC_MOVE_DISABLE,
		CTRL_STEP_FORCE,
		CTRL_AC_CLEAR_ALARMS,
		CTRL_ESTOP,
		CTRL_WELD_CYCLE_ENABLE,
		CTRL_HOME_POSITION_ENABLE,
		CTRL_READY_CHECK_ENABLE,
		CTRL_PART_CONTACT_ENABLE
		
	};
	
	enum AC_STATUS_EVENTS
	{
		STATUS_PART_CONTACT_FOUND,
		STATUS_EN_FAULT,
		STATUS_HOME_POSITION_FOUND,
		STATUS_READY_SIGNAL_ON,
		STATUS_WELD_CYCLE_ENABLE,
		STATUS_SET_ENCODER_ENABLE,
		STATUS_AC_MOVE_DISABLE,
		STATUS_START_SWITCH_PRESSED
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
	
	ACSTATES 		stateType;
protected:
	unsigned int 		m_Timeout;
	CommonProperty 	*CP;

};

#endif /* ACSTATE_H_ */
