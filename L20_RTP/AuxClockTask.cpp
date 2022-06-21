/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     The class is only responsible for the 1ms auxiliary clock interrupt handling.
 
**********************************************************************************************************/

#include "AuxClockTask.h"
extern "C"
{
	#include "customSystemCall.h"	
}

AuxClockTask* AuxClockTask::m_AuxTaskObj = nullptr;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
AuxClockTask::AuxClockTask() {
	// TODO Auto-generated constructor stub
//	debugAssignGPIO();
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AuxClockTask::~AuxClockTask() 
{
	m_AuxTaskObj = nullptr;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - AuxClockTask* Object 
*
******************************************************************************/
AuxClockTask* AuxClockTask::getInstance()
{
	return m_AuxTaskObj ? m_AuxTaskObj : (m_AuxTaskObj = new AuxClockTask());
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AuxClockTask::deleteInstance()
{
	if(m_AuxTaskObj != nullptr)
		delete m_AuxTaskObj;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AuxClockTask::AuxClock_Task(void* _obj)
{
	static unsigned int tick1 = 0;
	AuxClockTask* auxClockObj = (AuxClockTask*)_obj;
	tick1++;
	if((tick1 % 1000) == 0)
	{
		LOGERR((char *) "AuxClock : AuxClock running!!!",0,0,0);
	}
//	auxClockObj->debugFlipGPIOLevel();
}

/**************************************************************************//**
* \brief   - Test PIN Initialization
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AuxClockTask::debugAssignGPIO()
{
//    if(GpioAlloc(TEST_PIN) != OK)
//    {
//        printf("failed to allocate pin, try to free it first\n");
//        return;
//    }
//    
//    if(GpioSetDir(TEST_PIN, GPIO_DIR_OUTPUT) != OK)
//    {
//        printf("failed to switch to output direction\n");
//        return;
//    }
//    
//    if(GpioSetValue(TEST_PIN, GPIO_VALUE_HIGH) != OK)
//    {
//        printf("failed to output value\n");
//        return;
//    }
}

/**************************************************************************//**
* \brief   - GPIO level flip.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void AuxClockTask::debugFlipGPIOLevel()
{
//	static int voltage = GPIO_VALUE_HIGH;
//	if(voltage == GPIO_VALUE_HIGH)
//		voltage = GPIO_VALUE_LOW;
//	else
//		voltage = GPIO_VALUE_HIGH;
//    if(GpioSetValue(TEST_PIN, voltage) != OK)
//    {
//        printf("failed to output value\n");
//    }
}

