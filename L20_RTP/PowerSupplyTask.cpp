/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "PowerSupplyTask.h"
#include "PCStateMachine.h"
#include "AUPSTask.h"
#include "DSPTask.h"
#include "PCState.h"
#include "Utils.h"
/* Static member variables are initialized */
unsigned int PowerSupplyTask::CoreState = 0;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
PowerSupplyTask::PowerSupplyTask() {
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]);
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);

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
	char tmpMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0};
	MESSAGE	tmpMsg;
	memset(tmpMsg.Buffer, 0, sizeof(tmpMsg.Buffer));
	
	while(msgQReceive(SELF_MSG_Q_ID, tmpMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
	{
		Decode(tmpMsgBuffer, tmpMsg);
		switch(tmpMsg.msgID)
		{
		default:
			LOGERR((char *)"Power Supply_T : --------Unknown Message ID----------- : ", tmpMsg.msgID, 0, 0);
			break;
		}
	}
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
	return (CoreState == NO_ERROR || CoreState == BIT_MASK(SONICS_ON_OFF_STATUS)) ? NO_ERROR : CoreState;
}

/**************************************************************************//**
* \brief  	- Set specific power supply core status
*
* \param	- uint32_t
*
* \return 	- None
*
******************************************************************************/
void PowerSupplyTask::SetCoreState(unsigned int coreState)
{
	CoreState = coreState;
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
	UINT32		events;

	PowerSupplyTask *PSTask = new(nothrow) AUPSTask();
	if(nullptr != PSTask)
	{
		while(PSTask->bIsTaskRunStatus())
		{
			// wait for any one event
			if(eventReceive(PS_TASK_TX_EVENT | PS_TASK_RX_EVENT | PS_TASK_1MS_EVENT | PS_TASK_QEVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &events) != ERROR)
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
				if(events & PS_TASK_1MS_EVENT)
				{
					PCStateMachine::RunStateMachine();
				}
				if(events & PS_TASK_QEVENT)
				{
					// process incoming messages
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
