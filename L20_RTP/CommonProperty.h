/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMONPROPERTY_H_
#define COMMONPROPERTY_H_

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "Common.h"
#include "WeldResults.h"
#include "SystemInformationUI.h"
#include "SystemType.h"
#include "HeightCalibrationUI.h"
#include "WeldResultSignature.h"
#define TOTAL_NUM_OF_TASK			CommonProperty::END_OF_TASKS
#define PRESSURE_NUM 81
using namespace std;

#define RESULT_FOR_UI_MAX 50

class CommonProperty {
public:
	enum TASK_INDEX
	{
		CTRL_T		= 0,
		ACTUATOR_PROCESS_T,
		POWER_SUPPLY_T,
		ACTUATOR_SYSTEM_T,
		UI_T,
		DATA_T,
		DATA_INTERFACE_T,
		ALARM_T,
		BARCODE_READER_T,
		DGTIN_T,
		DGTOUT_T,
		MAINTENANCE_T,
		
		CONSOLE_T,
		HMI_SOCKET_T,
		ACT_SOCKET_T,
		CAN_SOCKET_T,
		FWUPGRADE_T,
		EXT_SOCKET_T,
		EXT_MANAGER_T,
		MAIN_T,
		END_OF_TASKS
	};

public:

public:						/* Public member functions */
	static CommonProperty*	getInstance		();
	static void 			deleteInstance	();

	TASK_ID					getTaskId		(string TaskName);
	MSG_Q_ID				getMsgQId		(string TaskName);
	void 					setTaskId		(string TaskName, TASK_ID tid);
	void					setMsgQId		(string TaskName, MSG_Q_ID mid);

public:						/* Public member data */
	
	/* Constant array for task names  */
	static const char* 		cTaskName[TOTAL_NUM_OF_TASK];

	bool					m_bTaskRun;
	// static member for global access
	static SYSTEM_INFO				SystemInfo;
	
private:					/* Private member functions */
	
	CommonProperty				();
	~CommonProperty				();
		
private:					/* Private member data */
	map<string, MSG_Q_ID>	m_MsgQMap;
	map<string, TASK_ID>	m_TaskIdMap;
	static CommonProperty	*m_CPObj;	
};

#endif /* COMMONPROPERTY_H_ */
