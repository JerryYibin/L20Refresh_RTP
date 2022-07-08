/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     The class is only responsible for the 1ms auxiliary clock interrupt handling.
 
**********************************************************************************************************/

#include "AuxClockTask.h"
#include "Utility.h"
#include "SCStateMachine/SCStateMachine.h"
#include "GPIO.h"
#include "HeightEncoder.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}

AuxClockTask* AuxClockTask::m_AuxTaskObj = nullptr;
CommunicationInterface_CAN::TX_MESSAGE AuxClockTask::m_Pressure;
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
	_objDCan = CommunicationInterface_CAN::GetInstance();
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
		if(SCStateMachine::getInstance()->GetStateMachineState() == false)
		{
			m_Pressure.DAC_Pressure = Utility::Pressure2HEX(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure); 
			if(auxClockObj->_objDCan->Sending(&m_Pressure) == ERROR)
			{
				//TODO need to send a alarm to UIC.
			}
		}
	}

	SCStateMachine::getInstance()->RunStateMachine();
	if(eventSend(auxClockObj->CP->getTaskId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]), PS_TASK_RX_EVENT) != OK)
		LOGERR((char*) "AUX_T: eventSend: Error\n",0,0,0);
	if(eventSend(auxClockObj->CP->getTaskId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]), PS_TASK_TX_EVENT) != OK)
		LOGERR((char*) "AUX_T: eventSent: Error\n",0,0,0);
//	auxClockObj->debugFlipGPIOLevel();
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
	static int voltage = GPIO_VALUE_HIGH;
	if(voltage == GPIO_VALUE_HIGH)
		voltage = GPIO_VALUE_LOW;
	else
		voltage = GPIO_VALUE_HIGH;
    if(vxbGpioSetValue(O_TEST_PIN, voltage) != OK)
    {
        printf("failed to output value\n");
    }
}

