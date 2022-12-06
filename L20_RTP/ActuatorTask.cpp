/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "ActuatorTask.h"
#include "ACStateMachine.h"
#include "L20ActuatorTask.h"
#include "P1ActuatorTask.h"
#include "Recipe.h"
/* Static member variables are initialized */
unsigned int ActuatorTask::CoreState 				= 0;
ActuatorTask* ActuatorTask::_ACObj 					= nullptr;
UINT32 ActuatorTask::Tick_1ms 						= 0;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ActuatorTask::ActuatorTask() 
{
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_SYSTEM_T]);
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	m_DebounceCount = 0;
	m_PB1 = 0;
}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ActuatorTask::~ActuatorTask() {
	// TODO Auto-generated destructor stub
}

/*************************************************************************//**
 * \brief   - Process the received message from UI_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void ActuatorTask::ProcessTaskMessage(MESSAGE & message)
{
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));
	int motion;
	
	while(msgQReceive(SELF_MSG_Q_ID, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(tmpMsg.msgID)
		{
		case TO_ACT_TASK_PRESSURE_SET:
			Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &ACStateMachine::AC_RX->TargetPressure);
			break;
		case TO_ACT_TASK_AUX_MOTION:
			memcpy(&motion, tmpMsg.Buffer, sizeof(int));
			ActuatorTask::GetInstance()->DoAuxMotion(motion);
			break;
		default:
			LOGERR((char*)"Actuator_T: --------Unknown Message ID----------- : %d", tmpMsg.msgID, 0, 0);
			break;
		}
	}
}

/**************************************************************************//**
* \brief  	- Specific actuator core status
*
* \param	- None
*
* \return 	- uint32_t
*
******************************************************************************/
unsigned int ActuatorTask::GetCoreState()
{
	return CoreState;
}

/**************************************************************************//**
* \brief  	- Set specific actuator core status
*
* \param	- uint32_t
*
* \return 	- None
*
******************************************************************************/
void ActuatorTask::SetCoreState(unsigned int coreState)
{
	CoreState |= coreState;
}

/**************************************************************************//**
* \brief  	- Clear specific actuator core status
*
* \param	- uint32_t
*
* \return 	- None
*
******************************************************************************/
void ActuatorTask::ClearCoreState(unsigned int coreState)
{
	CoreState &= ~coreState;
}

/**************************************************************************//**
* \brief  	- Get specific actuator object following system type
*
* \param	- None
*
* \return 	- ActuatorTask object
*
******************************************************************************/
ActuatorTask* ActuatorTask::GetInstance()
{
	return (_ACObj != nullptr) ? _ACObj : (_ACObj = new(nothrow) L20ActuatorTask());
}

/**************************************************************************//**
* \brief  	- To check if PB1 has been already pressed with debounce time 50ms 
*
* \param	- None
*
* \return 	- PB1 pressed status.
*
******************************************************************************/
bool ActuatorTask::GetPB1()
{
	bool bResult = false;
	if(m_DebounceCount == 0)
	{
		if((ACStateMachine::AC_TX->ACInputs & SS1MASK) == SS1MASK)
		{
			m_PB1 = SS1MASK;
			m_DebounceCount++;
		}
		else if((ACStateMachine::AC_TX->ACInputs & SS2MASK) == SS2MASK)
		{
			m_PB1 = SS2MASK;
			m_DebounceCount++;
		}
		else
		{
			m_PB1 = 0;
		}
	}
	else
	{
		if((ACStateMachine::AC_TX->ACInputs & m_PB1) == m_PB1)
		{
			m_DebounceCount++;
		}
		else
		{
			m_DebounceCount = 0;
		}
	}
	if(m_DebounceCount < DEBOUNCE_TIME)
		bResult = false;
	else
	{
		m_PB1 = 0;
		m_DebounceCount = 0;
		bResult = true;
	}
	return bResult;
}

/**************************************************************************//**
* \brief  - Actuator task entry point and constructor. Handles the
* 			process/system data from/to the SM and incoming messages.
* 			The code will instantiate the related child power task following system type,
* 			such as L20 refresh or P1.
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void ActuatorTask::Actuator_System_Task(void)
{
	MESSAGE		ProcessBuffer;
	UINT32		events;

	ActuatorTask *ACTask = ActuatorTask::GetInstance();
	if(NULL != ACTask)
	{
		while(ACTask->bIsTaskRunStatus())
		{
			// wait for any one event
			
			if(eventReceive(ACT_TASK_TX_EVENT | ACT_TASK_RX_EVENT | ACT_TASK_1MS_EVENT | ACT_TASK_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				if(events & ACT_TASK_RX_EVENT)
				{
					// process outgoing power supply data to SM here...
					ACTask->PDOUploadRequest();
				}
				if(events & ACT_TASK_TX_EVENT)
				{
					// process incoming power supply data from SM here...
					ACTask->PDODownloadRequest();
				}
				if(events & ACT_TASK_1MS_EVENT)
				{
					ACStateMachine::RunStateMachine();
					Tick_1ms++;
				}
				if(events & ACT_TASK_QEVENT)
				{
					// process incoming messages
					ACTask->ProcessTaskMessage(ProcessBuffer);
				}
			}
		}
		
		delete ACTask;
	}
	else
	{
		LOGERR((char *)"ACTUATOR_T : ----------------Memory allocation failed----------------", 0, 0, 0);
	}
	ACTask = NULL;
	taskSuspend(taskIdSelf());
}
