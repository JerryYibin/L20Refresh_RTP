/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     SC Digtial Output task for all the output signals of external operations  
 
**********************************************************************************************************/

#include "ScDgtOutput.h"
#include "GPIO.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"		
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of ScDgtOutputTask.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
ScDgtOutputTask::ScDgtOutputTask()
	:SELF_MSG_Q_ID(0)
	,UI_MSG_Q_ID(0)
{
	// Load the data message Q name
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T]);
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
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
ScDgtOutputTask::~ScDgtOutputTask()
{

}

/*************************************************************************//**
 * \brief   - Process the received message from OUTPUT_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void ScDgtOutputTask::ProcessTaskMessage(MESSAGE& message)
{
	int status = GPIO_VALUE_LOW;
	memcpy(&status, message.Buffer, sizeof(int));
	switch(message.msgID)
	{
	case TO_DGT_OUTPUT_TASK_READY_SET:
		if(status == GPIO_VALUE_LOW)
			vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
		else if(status == GPIO_VALUE_HIGH)
			vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_HIGH);
		else
		{
			
		}
		break;
	case TO_DGT_OUTPUT_TASK_SONICS_SET:
		if(status == GPIO_VALUE_LOW)
			vxbGpioSetValue(GPIO::O_SPARE, GPIO_VALUE_LOW);
		else if(status == GPIO_VALUE_HIGH)
			vxbGpioSetValue(GPIO::O_SPARE, GPIO_VALUE_HIGH);
		else
		{
			
		}
		break;
	case TO_DGT_OUTPUT_TASK_ALARM_SET:
		if(status == GPIO_VALUE_LOW)
			vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
		else if(status == GPIO_VALUE_HIGH)
			vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
		else
		{
			
		}
		break;
	default:
		LOGERR((char *)"ScDgtOutput : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Processing the Digital Output Data for external.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ScDgtOutputTask::ScDgtOutput_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	

	ScDgtOutputTask *DigitalOutput = new(nothrow) ScDgtOutputTask();

	if(nullptr != DigitalOutput)
	{
		/* ScDgtOutput Task loop and the bIsTaskRun flag enabled when task created */
		while(DigitalOutput->bIsTaskRunStatus())
		{
			if(msgQReceive(DigitalOutput->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
				DigitalOutput->Decode(MsgQBuffer, ProcessBuffer);
				DigitalOutput->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete DigitalOutput;
	}
	else
	{
		LOGERR((char *)"DIGITAL_OUTPUT_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	DigitalOutput = nullptr;
	taskSuspend(taskIdSelf());
}
