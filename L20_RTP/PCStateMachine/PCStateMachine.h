/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 Power supply state machine  
 
***************************************************************************/

#ifndef PSSTATEMACHINE_H_
#define PSSTATEMACHINE_H_
#include "PCState.h"
#include "PCReady.h"
#include "PCWeldRun.h"
#include "PCEstop.h"
#include "PCAlarm.h"
#include "PCSeekRun.h"
class PCStateMachine 
{
public:
	struct RxPDO_PC
	{
		UINT32	MasterState;
		UINT32	TargetAmplitude;
		UINT32	MasterEvents;
		UINT32  StartFrequency;
		UINT32  AmplitudeRampTime;
		INT32   AmpProportionalGain;
		INT32   AmpIntegralGain;
		INT32   AmpDerivativeGain;
		INT32   PhaseProportionalGain;
		INT32   PhaseIntegralGain;
		INT32   PhaseDerivativeGain;
	};
	
	struct TxPDO_PC
	{
		UINT32  Amplitude;
		INT32  	Phase;
		UINT32  Power;
		UINT32  Frequency;
		UINT32  Current;
		UINT32  PC_StatusEvent;
		UINT32	PCState;
	};
	static TxPDO_PC *PC_TX;
	static RxPDO_PC *PC_RX;
public:
	virtual						~PCStateMachine		();
	static void					RunStateMachine		();
	static PCState::PCSTATES	GetStateType		();
	static void					ChangeState			(PCState::PCSTATES);
protected:	
	PCReady*			m_ReadyObj;
	PCWeldRun*			m_WeldRunObj;
	PCEstop*			m_EstopObj;
	PCAlarm*			m_AlarmObj;
	PCSeekRun*			m_SeekRunObj;
private:
	static PCState*			CurrentStateObj;
	static PCStateMachine* 	StateMachineObj;
private:
	static PCStateMachine*	GetInstance();
	PCStateMachine();
	
};

#endif /* PSSTATEMACHINE_H_ */
