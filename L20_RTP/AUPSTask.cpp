/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "AUPSTask.h"
#include "ADC_SPI.h"
#include "DAC_SPI.h"
#include "PCStateMachine/PCStateMachine.h"
#include "GPIO.h"
#include "PCStateMachine/PCState.h"
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
AUPSTask::AUPSTask() {
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
AUPSTask::~AUPSTask() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief  - Run state machine here
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void AUPSTask::PDOUploadRequest()
{
	// run state machine here...
	PCStateMachine::RunStateMachine();
	ADC_AD7689::RunADCSample();
	DAC_TLV5604::SetAmplitude(PCStateMachine::PC_RX->TargetAmplitude);
	
}

/**************************************************************************//**
* \brief  - Read relevant data from AUPS.
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
void AUPSTask::PDODownloadRequest()
{
	PCStateMachine::PC_TX->Amplitude = ADC_AD7689::GetAmplitude();
	PCStateMachine::PC_TX->Frequency = ADC_AD7689::GetFrequency();
	PCStateMachine::PC_TX->Power 	 = ADC_AD7689::GetPower();
	
	if(vxbGpioGetValue(GPIO::I_OL_PSOUI) == GPIO_VALUE_HIGH)
	{
		SetCoreState(BIT_MASK(POWER_OVERLOAD));
	}
	else
	{
		SetCoreState(BIT_MASK(SONICS_ON_OFF_STATUS));
	}
}

