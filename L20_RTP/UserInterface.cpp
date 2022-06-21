/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Interface task 
     Changed the C code "UserInterface_Task" to c++ static function. 
It's a good change for UserInterface_Task so UserInterface_Task can access all the source of
UserInterface owned using the class object pointer. 
 
**********************************************************************************************************/

#include "UserInterface.h"

extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of UserInterface.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
UserInterface::UserInterface()
{
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	CTRL_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::CTRL_T]);
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_CTRL = CP->getMsgQId (Data_Task + "/Control");
	DATA_MSG_Q_ID_DATA = CP->getMsgQId (Data_Task + "/Data");
	DATA_MSG_Q_ID_REQ  = CP->getMsgQId (Data_Task + "/Request");

	INTERFACE_MSG_Q_ID	= CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
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
UserInterface::~UserInterface() {
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
void UserInterface::ProcessTaskMessage(MESSAGE& message)
{
	UINT32 msgLen = 0;
	UINT8 status;
	INT32 socketStatus = 0;
	string details;
	char signal;
	
	switch(message.msgID)
	{
	default:
		LOGERR((char *)"UI_T : --------Unknown Message ID----------- : %d",message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Processing the User data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UserInterface_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	
	UserInterface *UI = new(nothrow) UserInterface();
	
	if(NULL != UI)
	{
//		UI->PowerOnRequest(FW_VERSION);
//		UI->PowerOnRequest(POWER_ON_RECIPE_READ);
//		UI->PowerOnRequest(CALIBRATION_TIME_STAMP);
//		Counters::loadCounters();

		/* UserInterface Task loop and the bIsTaskRun flag enabled when task created */
		while(UI->bIsTaskRunStatus())
		{
			if(msgQReceive(UI->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR) 
			{
				UI->Decode(MsgQBuffer, ProcessBuffer);
				UI->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete UI;
	}
	else
	{
		LOGERR((char *)"UI_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	
	UI = NULL;	
	taskSuspend(taskIdSelf());
}
