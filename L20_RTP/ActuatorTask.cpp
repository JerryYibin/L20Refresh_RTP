/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "ActuatorTask.h"
#include "L20ActuatorTask.h"
#include "P1ActuatorTask.h"
/* Static member variables are initialized */
unsigned int ActuatorTask::CoreState = 0;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ActuatorTask::ActuatorTask() {
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
	return 0;
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
	CoreState = coreState;
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

	ActuatorTask *ACTask = new(nothrow) L20ActuatorTask();
	if(NULL != ACTask)
	{
		while(ACTask->bIsTaskRunStatus())
		{
			// wait for any one event
			if(eventReceive(ACT_TASK_TX_EVENT | ACT_TASK_RX_EVENT | ACT_TASK_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
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
