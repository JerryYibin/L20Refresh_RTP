/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
   It is responsible for digital input IOs request of UI and 
   processing all the input interrupt. 
 
***************************************************************************/

#include "L20ScDgtInputTask.h"
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
L20ScDgtInputTask::L20ScDgtInputTask() 
{

    
    

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
L20ScDgtInputTask::~L20ScDgtInputTask() {

}

/**************************************************************************//**
* 
* \brief   - Set L20 Ready Signal
*
* \param   - None.
*
* \return  - IOStatus.
*
******************************************************************************/
int	L20ScDgtInputTask::GetDgtInputBits()
{
	USER_IO ioStatus;
	if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH)
		ioStatus.IOBits.PB1 = 0;
	else
		ioStatus.IOBits.PB1 = 1;
	if(vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
		ioStatus.IOBits.PB2 = 0;
	else
		ioStatus.IOBits.PB2 = 1;
	
	if(vxbGpioGetValue(GPIO::I_OPSIG) == GPIO_VALUE_HIGH)
		ioStatus.IOBits.Reset = 0;
	else
		ioStatus.IOBits.Reset = 1;
	
	return ioStatus.UserIOs;
}

void L20ScDgtInputTask::DgtInterruptHandler(void* _obj)
{
	
}
