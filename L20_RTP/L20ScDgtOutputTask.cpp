/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
 Specific Output GPIO process   
 
***************************************************************************/

#include "L20ScDgtOutputTask.h"
#include "GPIO.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"		
}

/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
L20ScDgtOutputTask::L20ScDgtOutputTask() {
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
L20ScDgtOutputTask::~L20ScDgtOutputTask() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Set L20 Ready Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::SetDgtReadyOutput()
{
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_HIGH);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Reset L20 Ready Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::ResetDgtReadyOutput()
{
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Set L20 Sonics Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::SetDgtSonicsOutput()
{
	vxbGpioSetValue(GPIO::O_SPARE, GPIO_VALUE_HIGH);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Reset L20 Sonics Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::ResetDgtSonicsOutput()
{
	vxbGpioSetValue(GPIO::O_SPARE, GPIO_VALUE_LOW);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Set L20 Alarm Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::SetDgtAlarmOutput()
{
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_HIGH);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Reset L20 Alarm Signal
*
* \param   - None.
*
* \return  - OK.
*
******************************************************************************/
int L20ScDgtOutputTask::ResetDgtAlarmOutput()
{
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
	return OK;
}
