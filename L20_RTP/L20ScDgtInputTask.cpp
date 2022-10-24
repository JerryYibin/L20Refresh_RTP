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
#include "Utils.h"
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
	memset(m_OnHoldDebounce, 0, BITS_SIZE);
	memset(m_ReleaseDebounce, 0, BITS_SIZE);
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
void L20ScDgtInputTask::GetDgtInputBits()
{	
	if(vxbGpioGetValue(GPIO::I_OPSIG) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(RESET);
	else
		m_UserIO |= BIT_MASK(RESET);
	
	if(vxbGpioGetValue(GPIO::I_ESTOPNC) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(ESTOP);
	else
		m_UserIO |= BIT_MASK(ESTOP);
	
	if(vxbGpioGetValue(GPIO::I_SWITCH_HOLD) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(POWER_OFF);
	else
		m_UserIO |= BIT_MASK(POWER_OFF);
	
	if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(PB1);
	else
		m_UserIO |= BIT_MASK(PB1);
	
	if(vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(PB2);
	else
		m_UserIO |= BIT_MASK(PB2);
	
	if(vxbGpioGetValue(GPIO::I_ESTOPNCR) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(ACT_STATUS);
	else
		m_UserIO |= BIT_MASK(ACT_STATUS);

	if(vxbGpioGetValue(GPIO::I_24V_LOW) == GPIO_VALUE_HIGH)
		m_UserIO &= ~BIT_MASK(LOST_24V);
	else
		m_UserIO |= BIT_MASK(LOST_24V);
	

}

