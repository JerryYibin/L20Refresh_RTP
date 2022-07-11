/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Actuator state machine  
 
***************************************************************************/

#ifndef ACSTATEMACHINE_H_
#define ACSTATEMACHINE_H_
#include "ACState.h"
class ACStateMachine 
{
public:
	struct RxPDO_AC
	{
		UINT32	MasterState;
		UINT32	TargetPressure;
		UINT32	MasterEvents;
		UINT32	ACOutputs;
	};

	struct TxPDO_AC
	{
		UINT32  ActualForce;
		INT32 	ActualDistance;
		UINT32  AC_StatusEvent;
		UINT32  ACState;
		UINT32  ACInputs;
	};
	
	static TxPDO_AC *AC_TX;
	static RxPDO_AC *AC_RX;
public:
	virtual						~ACStateMachine		();
	static void					RunStateMachine		();
	static ACState::ACSTATES	GetStateType		();
	static void					ChangeState			(ACState::ACSTATES);
protected:	
private:
	static ACState*			CurrentStateObj;
	static ACStateMachine* 	StateMachineObj;
private:
	static ACStateMachine*	GetInstance();
	ACStateMachine();
	
};

#endif /* ACSTATEMACHINE_H_ */
