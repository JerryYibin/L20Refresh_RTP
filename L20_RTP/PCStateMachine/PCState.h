/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/

#ifndef PSSTATE_H_
#define PSSTATE_H_
#include "../CommonProperty.h"
#include "../Utils/Utils.h"
#include "../GPIO.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
class PCState 
{
public:
	enum PCSTATES
	{
		PC_READY,
		PC_WELD_RUN,
		PC_SEEK_RUN,
		PC_WELD_HD_RUN,
		PC_ALARM, 
		PC_SCAN_RUN,
		PC_TEST_RUN,
		PC_ESTOP
	};
	
	enum PC_CONTROL_EVENTS
	{
		CTRL_PC_SONIC_DISABLE,
		CTRL_TMR_INTERVAL,
		CTRL_POWER_UP_RECIPE,
		CTRL_PC_CLEAR_ALARMS,
		CTRL_PREBURST_ENABLE,
		CTRL_WELDTEST,
		CTRL_AFTERBURST_ENABLE,
		CTRL_ESTOP
	};
	
	enum PC_STATUS_EVENTS
	{
		STATUS_TMR_INTERVAL,
		STATUS_POWER_UP_RECIPE,
		STATUS_PC_CLEAR_ALARMS,
		STATUS_PREBURST_ENABLE,
		STATUS_AFTERBURST_ENABLE,
		STATUS_SOURCE_LOST,
	};
	
	enum ErrorTypePC {
		NO_ERROR = 						0,
		ERR_POWER_OVERLOAD = 			0x01,
		ERR_WELDERROR = 				0x01,
		ERR_CURRENT_OVERLOAD = 			0x02 ,
		ERR_VOLTAGE_OVERLOAD = 			0x04,
		ERR_TEMP_OVERLOAD = 			0x08,
		ERR_DDSFREQ_LOWSEEKLIMIT =		0x10,
		ERR_DDSFREQ_HIGHSEEKLIMIT = 	0x20,
		ERR_DDSFREQ_LOWWELDLIMIT = 		0x40,
		ERR_DDSFREQ_HIGHWELDLIMIT = 	0x80,
		ERR_PHASELIMIT_TIMEERROR = 		0x100,
		ERR_15V_LOST=					0x800,
		ERR_PWR_AVG_OVERLOAD = 			0x10000,
	};
public:
	PCState() {CP = CommonProperty::getInstance();}
	virtual 		~PCState	() {};
public:
	virtual void 		Enter			() = 0;
	virtual void  		Loop			() = 0;
	virtual void  		Exit			() = 0;
	
	virtual void 		ChangeState		(PCSTATES nextState);
	virtual PCSTATES 	GetStateType	() {return  stateType;}
	
	CommonProperty 	*CP;
	PCSTATES 		stateType;
};

#endif /* PSSTATE_H_ */
