/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Database Interface task  
     
Changed the C code "DataInterface_Task" to c++ static function. 
It's a good change for DataInterface_Task so DataInterface_Task can access all the source of
DataInterface owned using the class object pointer. 
 
**********************************************************************************************************/

#include "DataInterface.h"

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
DataInterface::DataInterface()
{
	SELF_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID_REQUEST = CP->getMsgQId(Data_Task + "/Request");

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
DataInterface::~DataInterface() 
{
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
void DataInterface::ProcessTaskMessage(MESSAGE& message)
{

	switch(message.msgID)
	{
	default:
		LOGERR((char *)"Data_Interface_T : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Processing the Database Interface Data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void DataInterface::DataInterface_Task(void)
{
	MESSAGE		ProcessBuffer;
	char		MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	

	DataInterface *Interface = new(nothrow) DataInterface();
	if(NULL != Interface)
	{
		while(Interface->bIsTaskRunStatus())
		{
			if(msgQReceive(Interface->SELF_MSG_Q_ID, MsgQBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
				Interface->Decode(MsgQBuffer, ProcessBuffer);
				Interface->ProcessTaskMessage(ProcessBuffer);
			}
		}
		
		delete Interface;
	}
	else
	{
		LOGERR((char *)"DATA_INTERFACE_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	Interface = NULL;
	taskSuspend(taskIdSelf());
}
