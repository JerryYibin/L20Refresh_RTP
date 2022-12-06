/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     ---------------------------- MODULE DESCRIPTION ----------------------------
     As you see the AUPSTask class is inherited by PowerSupply Task so 
     there are some pure virtual functions that are defined in the PowerSupply Task need to 
     implement with specific code of itself owned following different hardware design as following...
	 1. virtual void		PDOUploadRequest			() override;
	 2. virtual void		PDODownloadRequest			() override;
	 3. virtual void		CalculateStartFrequency		() override;
     In the future all the Sonics generator shall use PCB roadmap design implemented by the DSP core so 
     PDOUploadRequest and PDODownloadRequest shall be for DSP core based sonics generators going forward.
     For L20 Refresh it is the special case, the sonics generator implemented by the separated AUPS board 
     connects with SC using some ADC& DAC and I/O signals cable, 
     so there are some ADC & DAC specific implement in the PDOUploadRequest and PDODownloadRequest here.
 
***************************************************************************/

#include "AUPSTask.h"
#include "ADC_SPI.h"
#include "DAC_SPI.h"
#include "PCStateMachine.h"
#include "GPIO.h"
#include "PCState.h"
#include "Utils.h"
#include "Utility.h"
#include "Recipe.h"
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
* \brief  - For PDO upload request there are something needs to do as following...
* 			1. Trigger ADC Sampling within each 1ms
* 			2. To set Amplitude if the new coming is not equal to current one.
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

/**************************************************************************//**
* \brief  - Calculate Start Frequency after each seek finished. 
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void AUPSTask::CalculateStartFrequency()
{
	//TODO It seems there is nothing need to do for AUPS, because the start frequency can be saved automatically.
}
