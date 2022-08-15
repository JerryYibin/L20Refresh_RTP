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
		HEIGHT_CALIBRATION,
		TEST,
		TEACH_MODE,
		BATCH_WELD,
		RESET_ALL_ALARM,
	};
	
	
//	enum SC_CONTROL_EVENTS
//	{
//		CTRL_ACTUATOR_CALIBRATE_ENABLE,
//		CTRL_ACTUATOR_CHECK_ENABLE,
//		CTRL_ACTUATOR_CORRECT_ENABLE,
//		CTRL_ACTUATOR_ACCEPT_ENABLE,
//		CTRL_ACTUATOR_EXIT_ENABLE,
//	};	
	
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
	void 					XTickTock();
private:
	SCStateMachine();
	void deleteAll();
	void initStateMap();
	void addActionToMap(SCState::STATE stateIdx, SCAction* _action);
private:
	vector<SCState*>* 				_objStateList;
	map<SCState::STATE, int>*		_objStateMap;
	map<SCState::STATE, SCAction*>* _objActionMap;
	unsigned int					m_StateIndex;
	unsigned int					m_CoolDelayTimer;
	unsigned int					m_CoolDurationTimer;
	bool							m_IsRunning;
	bool							m_IsLoading;
	static SCState* 				m_objState;
	static SCStateMachine* 			m_StateMachineObj;
};

#endif /* SCSTATEMACHINE_H_ */
