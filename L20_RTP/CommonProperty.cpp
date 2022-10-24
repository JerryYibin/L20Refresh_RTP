/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Common property class functions for Real time and Non real time data communication
 
**********************************************************************************************************/


#include "CommonProperty.h"
/* static initializations */
CommonProperty*							CommonProperty::m_CPObj							= nullptr;
const char*								CommonProperty::cTaskName[TOTAL_NUM_OF_TASK]	= {0};
WELD_RESULT								CommonProperty::WeldResult;
WELD_RESULT				                CommonProperty::WeldResultForUI[RESULT_FOR_UI_MAX];
SYSTEM_INFO								CommonProperty::SystemInfo;
vector<WELD_SIGNATURE>					CommonProperty::WeldSignatureVector;
HMI_WELD_SIGNATURE_POINTS	            CommonProperty::WeldSignatureForUI[RESULT_FOR_UI_MAX];

/**************************************************************************//**
* \brief   - Constructor - Read default recipe and load into MAP.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
CommonProperty::CommonProperty()
{
	#ifdef DEBUG
		logMsg("---------CommonProperty constructor called---------",0,0,0,0,0,0);
	#endif
	
	SystemInfo.psLifeCounter = 0;
	SystemInfo.psFrequency = 20;
	SystemInfo.psWatt = 4000;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  -    CommonProperty* Object 
*
******************************************************************************/
CommonProperty* CommonProperty::getInstance()
{
	return (m_CPObj != nullptr) ? m_CPObj : (m_CPObj = new CommonProperty());
}

/**************************************************************************//**
* \brief   - Returns the particular task's message queue ID.
*
* \param   - string TaskName.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
MSG_Q_ID CommonProperty::getMsgQId(string TaskName)
{
	return m_MsgQMap.find(TaskName)->second;
}

/**************************************************************************//**
* \brief   - Store all the message queues id created for business logic.
*
* \param   - string TaskName, MSG_Q_ID.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
void CommonProperty::setMsgQId(string TaskName, MSG_Q_ID mid)
{
	m_MsgQMap.insert(pair<string, MSG_Q_ID>(TaskName, mid));
}

/**************************************************************************//**
* \brief   - Returns the particular task ID.
*
* \param   - string TaskName.
*
* \return  - TASK_ID.
*
******************************************************************************/
TASK_ID	CommonProperty::getTaskId(string TaskName)
{
	return m_TaskIdMap.find(TaskName)->second;
}

/**************************************************************************//**
* \brief   - Store all the tasks id created for business logic.
*
* \param   - string TaskName, TASK_ID.
*
* \return  - TASK_ID.
*
******************************************************************************/
void CommonProperty::setTaskId(string TaskName, TASK_ID tid)
{
	m_TaskIdMap.insert(pair<string, TASK_ID>(TaskName, tid));
}



/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty::deleteInstance()
{
	if(m_CPObj != nullptr)
		delete m_CPObj;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
CommonProperty::~CommonProperty()
{
	m_CPObj = NULL;
}

