/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SCSTATEMACHINE_H_
#define SCSTATEMACHINE_H_
#include <list>
#include "SCState.h"
#include "SCAction.h"
using namespace std;

#define P_PRESSURE 0
#define P_SHIM 1
class SCStateMachine {
public:
	enum OPERATION
	{
		NO_OPERATION = -1,
		POWER_UP,
		WELD,
		SEEK,
		FIND_PART_CONTACT_OP,
		HORN_RETRACT,
		RESET_ALARM,
		SCAN,
		HEIGHT_CALIBRATE_READY,
		TEST,
		TEACH_MODE,
		BATCH_WELD,
		RESET_ALL_ALARM,
		END_OPERATION
	};
	
public:
	static SCStateMachine* 	getInstance();
	virtual 				~SCStateMachine();
	void 					RunStateMachine();
	bool 					GetStateMachineState() const;
	int						GetCurrentStateIndex() const;
	void 					SetCoolingTimer(unsigned int delay, unsigned int duration);
	
	int 					LoadStatesHandler(int operation);
	
	int						ExecuteStateAction(SCState::STATE stateIdx);
	
	void 					SelectWeldSequence(void);
	void					SelectHeightCalibrateSequence(void);
	void					SelectSonicsTestSequence(void);
	void 					XTickTock();
	unsigned int			GetCoreState						();
	void					SetCoreState						(unsigned int coreState);
	void					ClearCoreState						(unsigned int coreState);
private:
	SCStateMachine();
	void deleteAll();
	void initStateMap();
	void addActionToMap(SCState::STATE stateIdx, SCState* _action);
private:
	vector<SCState*>* 				_objStateList;
	map<SCState::STATE, int>*		_objStateMap;
	map<SCState::STATE, SCState*>* _objActionMap;
	unsigned int					m_StateIndex;
	unsigned int					m_CoolDelayTimer;
	unsigned int					m_CoolDurationTimer;
	bool							m_IsRunning;
	bool							m_IsLoading;
	static SCState* 				m_objState;
	static SCStateMachine* 			m_StateMachineObj;
	static SEM_ID m_semaphoreMutex; /* ID of mutex semaphore*/
	static unsigned int 			CoreState;
};

#endif /* SCSTATEMACHINE_H_ */
