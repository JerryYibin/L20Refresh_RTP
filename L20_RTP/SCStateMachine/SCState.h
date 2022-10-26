/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SCSTATE_H_
#define SCSTATE_H_
#include "../CommonProperty.h"
#include "../GPIO.h"
#include "../Logger.h"
#include "../Utility.h"
#include "../ScDgtOutput.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
//TODO SC should be a interface class so every function should be true virtual.
//TODO variable stateType should be private attribute for each child object so it should be initilized in constructor.
//TODO GetStatetType is the true virtual function as well.
#define ALARMBEEPDELAY 1000
class SCState {
public:
	enum STATE
	{
		NO_STATE = -1,
		PRE_READY,
		READY,
		START_SWITCH,
		SEEK_SONIC_ON,
		WAIT_FOR_TRIGGER,
		SQUEEZE_TIME,
		WELD_PRESSURE_SET,
		WELD_SONIC_ON,

		HOLD,
		WAIT_FOR_READY_POSITION,
		CALIBRATE,
		HEIGHTCORRECT,
		READY_FOR_TRIGGER,
		DOWN_STROKE,
		RETURN_STROKE
	};
	enum ACTIONS
	{
		INIT = 0,
		LOOP,						/* Normal state, move on to next value        */
		FAIL,						/* Set FailFlag for later checking            */
		ABORT,						/* Set AbortFlag for later checking           */
		JUMP,						/* This state is complete, go to next state   */
		ALJUMPNORM,					/* This state is complete, go to next state,with alarms  */
		POP,						/* Return to PUSHed state previously saved    */
		PUSH,						/* Save current state index for possible POP  */
	};
public:
	ACTIONS			m_Actions;
	STATE			m_State;
	unsigned int	m_Timeout;
	unsigned int	m_StepIndex;	
public:
	SCState();
	virtual ~SCState();
public:
	virtual void Enter() = 0;	/* Function to call on entry          */
	virtual void Loop() = 0;	/* Function to call every pass        */
	virtual void Exit() = 0;	/* Function to call on exit           */
	virtual void Fail() = 0;	/* Function to call on alarm handling */
	
	//virtual bool SendToMsgQ(MESSAGE& msgBuffer, const int& msgQID);
	bool 					DefeatWeldAbortHandler	(void);
	bool 					ProcessAlarmHandler		(void);
	virtual STATUS			SendToMsgQ 				(MESSAGE& msgBuffer, const MSG_Q_ID& msgQID, _Vx_ticks_t waitType = NO_WAIT);
	void 					ChangeExtDgtOutput		(const ScDgtOutputTask::MESSAGE_IDENTIFY msgID);
private:
	void abortWeld(void);
	CommonProperty*							CP;
protected:
	MSG_Q_ID	UI_MSG_Q_ID;
	MSG_Q_ID	CTL_MSG_Q_ID;
	MSG_Q_ID	DGTOUT_MSG_Q_ID;
};

#endif /* SCSTATE_H_ */
