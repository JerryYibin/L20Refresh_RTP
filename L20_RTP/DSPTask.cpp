/**************************************************************************

	  Copyright (c) Branson Ultrasonics Corporation, 1996-2018

	 This program is the property of Branson Ultrasonics Corporation
	 Copying of this software is expressly forbidden, without the prior
	 written consent of Branson Ultrasonics Corporation.

***************************************************************************/

#include "DSPTask.h"

/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
DSPTask::DSPTask() {
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
DSPTask::~DSPTask() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief  - Process RX PDO event received from the control task to read data
* 			from internal shared memory (SD), run the native state machine
* 			and write relevant data to the SM.
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void DSPTask::PDOUploadRequest()
{
}

/**************************************************************************//**
* \brief  - Process TX PDO event received from the control task to write
* 			data to internal shared memory (SD) after relevant data was
* 			read from the SM.
* 			
*			UINT32  Amplitude;
*			INT32  	Phase;
*			UINT32  Power;
*			UINT32  Frequency;
*			UINT32  Current;
*			UINT32  PC_StatusEvent;
*			UINT32	PCState;
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void DSPTask::PDODownloadRequest()
{
}

