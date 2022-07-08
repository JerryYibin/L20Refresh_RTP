/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PowerSupplyTask.h"
#include "AUPSTask.h"
#include "DSPTask.h"
#include "PCStateMachine/PCState.h"
#include "Utils.h"
/* Static member variables are initialized */
unsigned int PowerSupplyTask::m_CoreState = 0;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PowerSupplyTask::PowerSupplyTask() {
	// TODO Auto-generated constructor stub

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
PowerSupplyTask::~PowerSupplyTask() {
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
void PowerSupplyTask::ProcessTaskMessage(MESSAGE& message)
{
	
}

/**************************************************************************//**
* \brief  	- Specific power supply core status
*
* \param	- None
*
* \return 	- uint32_t
*
******************************************************************************/
unsigned int PowerSupplyTask::GetCoreState()
{
	return (m_CoreState == PCState::NO_ERROR || m_CoreState == BIT_MASK(SONICS_ON_OFF_STATUS)) ? PCState::NO_ERROR : m_CoreState;
}

/**************************************************************************//**
* \brief  	- Set specific power supply core status
*
* \param	- None
*
* \return 	- uint32_t
*
******************************************************************************/
void PowerSupplyTask::SetCoreState(unsigned int coreState)
{
	m_CoreState = coreState;
}

/**************************************************************************//**
* \brief  - Power task entry point and constructor. Handles the
* 			process/system data from/to the SM and incoming messages.
* 			The code will instantiate the related child power task following system type,
* 			such as AUPS or DSP.
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void PowerSupplyTask::PowerSupply_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	UINT32		events;

	PowerSupplyTask *PSTask = new(nothrow) AUPSTask();
	if(NULL != PSTask)
	{
		while(PSTask->bIsTaskRunStatus())
		{
			// wait for any one event
			if(eventReceive(PS_TASK_TX_EVENT | PS_TASK_RX_EVENT | PS_TASK_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
			{
				if(events & PS_TASK_RX_EVENT)
				{
					// process outgoing power supply data to SM here...
					PSTask->PDOUploadRequest();
				}
				if(events & PS_TASK_TX_EVENT)
				{
					// process incoming power supply data from SM here...
					PSTask->PDODownloadRequest();
				}
				if(events & PS_TASK_QEVENT)
				{
					// process incoming messages
					PSTask->Decode(MsgQBuffer, ProcessBuffer);
					PSTask->ProcessTaskMessage(ProcessBuffer);
				}
			}
		}
		
		delete PSTask;
	}
	else
	{
		LOGERR((char *)"POWERSUPPLY_T : ----------------Memory allocation failed----------------", 0, 0, 0);
	}
	PSTask = NULL;
	taskSuspend(taskIdSelf());
}
