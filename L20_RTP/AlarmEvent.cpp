/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
	The AlarmData is for the AlarmData process only.   
 
***************************************************************************/

#include "AlarmEvent.h"
/**************************************************************************//**
* 
* \brief   - Constructor Initialize Alarm event
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
AlarmEvent::AlarmEvent() 
{
	//The alarm event instance need to send to data task for database using message queue;
	memset(m_strDescription, 0, RECORD_LEN); 
	memset(m_strAlarmLog, 0, RECORD_LEN);
	memset(m_strTimeStamp, 0, 20);
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
AlarmEvent::~AlarmEvent() 
{
	// TODO Auto-generated destructor stub
}

