/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "AUPSTask.h"
#include "ADC_SPI.h"
#include "DAC_SPI.h"
#include "PCStateMachine.h"
#include "GPIO.h"
#include "PCState.h"
#include "Utils.h"
#include "Utility.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
PCStateMachine::RxPDO_PC AUPSTask::RXBackup;
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
	RXBackup.TargetAmplitude = 0;
	PCStateMachine::PC_RX->TargetAmplitude = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude;

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
	unsigned short tmpAmplitude;
	ADC_AD7689::RunADCSample();
	if(RXBackup.TargetAmplitude != PCStateMachine::PC_RX->TargetAmplitude)
	{
		tmpAmplitude = Utility::Amplitude2HEX(PCStateMachine::PC_RX->TargetAmplitude);
		DAC_TLV5604::SetAmplitude(tmpAmplitude);
		RXBackup.TargetAmplitude = PCStateMachine::PC_RX->TargetAmplitude;
	}
	
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

