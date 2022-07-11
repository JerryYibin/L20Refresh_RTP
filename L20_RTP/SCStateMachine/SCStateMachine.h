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
using namespace std;
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
		CALIBRATION,
		TEST,
		TEACH_MODE,
		BATCH_WELD,
		RESET_ALL_ALARM,
	};
	
public:
	static SCStateMachine* 	getInstance();
	virtual 				~SCStateMachine();
	void 					RunStateMachine();
	bool 					GetStateMachineState() const;
	int						GetCurrentStateIndex() const;
	void 					SetCoolingTimer(unsigned int delay, unsigned int duration);
	
	int 					LoadStatesHandler(int operation);
	
	void 					SelectWeldSequence(void);
	void					SelectHeightCalSequence(void);
	void 					XTickTock();
private:
	SCStateMachine();
	void deleteAll();
	void initStateMap();
private:
	unsigned int	m_StateIndex;
	vector<SCState*>* _objStateList;
	map<int, int>*	_objStateMap;
	unsigned int	m_CoolDelayTimer;
	unsigned int	m_CoolDurationTimer;
	bool			m_IsRunning;
	bool			m_IsLoading;
	static SCState* m_objState;
	static SCStateMachine* m_StateMachineObj;
};

#endif /* SCSTATEMACHINE_H_ */
