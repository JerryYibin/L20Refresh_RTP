/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
     As you see the L20ActuatorTask class is inherited by Actuator Task so 
     there are some pure virtual functions that defined in the Actuator Task need to 
     implement with specific code of itself owned following different hardware design as following...
     1. virtual void			PDOUploadRequest 					() = 0;
	 2. virtual void 			PDODownloadRequest					() = 0;
	 3. virtual bool			IsMoving							() = 0;
	 4. virtual unsigned int	GetMaxSpeed							() = 0;
	 5. virtual void			InitHeightSystem					() = 0;
	 6. virtual void 			ScanInputs							() = 0;
     In the future all the actuator shall connect with SC using Ethernet cable so 
     PDOUploadRequest and PDODownloadRequest shall be for Ethernet based actuators going forward.
     For L20 Refresh it is the special case, the actuator connect with SC using CAN bus and some Analog signal cables, 
     so there are some specific implement in the PDOUploadRequest and PDODownloadRequest here.
     If the some system without actuator that is similar with DCX, 
     the PDOUploadRequest and PDODownloadRequest of specific class shall be empty.    
 
***************************************************************************/

#include "L20ActuatorTask.h"
#include "HeightEncoder.h"
#include "Utility.h"
#include "commons.h"
#include "DAC_SPI.h"
#include "Recipe.h"
#include "AuxMotionUI.h"
//#include "ACState.h"
CommunicationInterface_CAN* L20ActuatorTask::_ObjDCan 	= nullptr;
ACStateMachine::RxPDO_AC L20ActuatorTask::RXBackup;
/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
L20ActuatorTask::L20ActuatorTask() {
	_ObjDCan = CommunicationInterface_CAN::GetInstance();
	RXBackup.TargetPressure = 0;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &ACStateMachine::AC_RX->TargetPressure);
	InitMovingProcess();
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
L20ActuatorTask::~L20ActuatorTask() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief  - Set Target Pressure for actuator
* 			To update Pressure setting in each 1s.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void L20ActuatorTask::PDOUploadRequest()
{
	int iResult = OK;
	CommunicationInterface_CAN::TX_MESSAGE tmpPressure;
	//TODO Test level one board only
	if((Tick_1ms % 1000) != 0)
		return;
	else
	if(RXBackup.TargetPressure != ACStateMachine::AC_RX->TargetPressure)
	{
		tmpPressure.DAC_Pressure = Utility::Pressure2HEX(ACStateMachine::AC_RX->TargetPressure);
		iResult = _ObjDCan->Sending(&tmpPressure);

		if(iResult == ERROR)	
			SetCoreState(ERR_PRESSURE_SET);
		else
		{
			ClearCoreState(ERR_PRESSURE_SET);
			RXBackup.TargetPressure = ACStateMachine::AC_RX->TargetPressure;
		}
	}
}

/**************************************************************************//**
* \brief  - Read relevant data from Actuator as following
*			1. INT32  	ActualHeight;
*			2. UINT32	RawHeightCount;
*			3. Horn's status checking each 5ms
*			4. StartSwitch(PB1) Scanning
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void L20ActuatorTask::PDODownloadRequest()
{
	ACStateMachine::AC_TX->ActualHeight = HeightEncoder::GetInstance()->GetActualHeight(ACStateMachine::AC_RX->TargetPressure);
	ACStateMachine::AC_TX->RawHeightCount = HeightEncoder::GetInstance()->GetPositionCount();
	if(Tick_1ms % 5 == 0)
	{
		if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE)) == BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE))
			InitMovingProcess();
		else
			m_IsMoving = MovingCheckProcess();
		
	}
	
	ScanInputs();
}


/**************************************************************************//**
* \brief   - To check if the horn is still moving. 
* 			 There is the weighted average algorithm to calculate the speed. 
* 			 The function will be run by each 5ms. 
* 			 The MinMoveCount is to make sure the each element of array 
* 			 that is responsible for Weighted Average calculation should be filled out.  
*
* \param   - None.
*
* \return  - Horn's status
*
******************************************************************************/
bool L20ActuatorTask::MovingCheckProcess()
{
	//TODO will move it to actuator task for the function override for the different system
	bool isMoving = true;
	if(ACStateMachine::AC_TX->RawHeightCount != 0)
	{
		unsigned int tmpSpeed = WeightedAverageSpeed(ACStateMachine::AC_TX->RawHeightCount);
		if (tmpSpeed > MaxSpeed)
			MaxSpeed = tmpSpeed;
		if (MinMoveCount != 0)
			MinMoveCount--;
		else
		{
			if (tmpSpeed <= MIN_DOWN_SPEED)
			{
				isMoving = false;
			}
		}
	}
	else
		isMoving = true;
	return isMoving;
}

/**************************************************************************//**
* \brief   - Weighted Average Speed Calculation for the 5ms sampling
* 			index       Delta0,       Delta1,       Delta2,       Delta3,       OldPostion
* 			--------------------------------------------------------------------------------
* 			 0            0             0             0             0              0
* 			 1            0             0             0          Encoder        Encoder
* 			 2            0             0          Encoder     Encoder - old    Encoder
* 			 3            0          Encoder     Encoder - old Encoder - old    Encoder
* 			 4         Encoder     Encoder - old Encoder - old Encoder - old    Encoder
* 			 5		Encoder - old  Encoder - old Encoder - old Encoder - old    Encoder
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
unsigned int L20ActuatorTask::WeightedAverageSpeed(unsigned int EncoderPosition)
{
	int tmpSpeed = 0;
	DeltaPosition[0] = DeltaPosition[1];
	DeltaPosition[1] = DeltaPosition[2];
	DeltaPosition[2] = DeltaPosition[3];
	DeltaPosition[3] = EncoderPosition - OldPosition;
	OldPosition = EncoderPosition;
	// in order to change the velocity unit to mm/s, the value need to time 200, right? 
	tmpSpeed = (DeltaPosition[3] * 60 + DeltaPosition[2] * 60 + DeltaPosition[1] * 40 + DeltaPosition[0] * 40); 
#if HEIGHT_CALIBRATE_DBG	
//	LOG("Current Count = %d\n", EncoderPosition);
//	for(int i = 0; i < 4; i++)
//	{
//		LOG("DeltaPosition[%d] = %d\n", i, DeltaPosition[i]);
//	}
//	LOG("tmpSpeed = %d\n", tmpSpeed);
//	LOG("ABS(Speed) = %d\n", _ABS(tmpSpeed));
#endif
	return  _ABS(tmpSpeed);
}

/**************************************************************************//**
* \brief   - Initialize all the variables that related to the horn moving checking.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void L20ActuatorTask::InitMovingProcess()
{
	for(int i = 0; i < 4; i++)
	{
		DeltaPosition[i] = 0;
	}
	MinMoveCount = 10;
	MaxSpeed = 0;
	OldPosition = 0;
	m_IsMoving = true;
}

/**************************************************************************//**
* \brief   - To get the horn moving status.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
bool L20ActuatorTask::IsMoving()
{
	return m_IsMoving;
}

/**************************************************************************//**
* \brief   - To get the horn moving Maximum speed.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
unsigned int L20ActuatorTask::GetMaxSpeed()
{
	return MaxSpeed;
}

/**************************************************************************//**
* \brief   - To set height encoder initialized count as HEIGHT_HOME_COUNT.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void L20ActuatorTask::InitHeightSystem()
{
	HeightEncoder::GetInstance()->SetInitCount(HEIGHT_HOME_COUNT);
}

/**************************************************************************//**
* \brief   - Scan all the input I/O signal that is from actuator per each 1ms.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void L20ActuatorTask::ScanInputs()
{
	if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH) 
		ACStateMachine::AC_TX->ACInputs &= ~SS1MASK;
	else
		ACStateMachine::AC_TX->ACInputs |= SS1MASK;
	
	if(vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
		ACStateMachine::AC_TX->ACInputs &= ~SS2MASK;
	else
		ACStateMachine::AC_TX->ACInputs |= SS2MASK;
	
	if(vxbGpioGetValue(GPIO::I_ESTOPNC) == GPIO_VALUE_HIGH)
		ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_ESTOP);
	else
		ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_ESTOP);
	
	
}

/**************************************************************************//**
* \brief   - These commands are used to manually control the hardware from UIController
*   		 and sending the DOAUXMOTION command with one of the following parameters.
*   		 Note: Some commands were inherited from Legacy L20.
*
* \param   - enum AUXMOTION..
*
* \return  - None
*
******************************************************************************/
void L20ActuatorTask::DoAuxMotion(int motion)
{
	switch(motion)
	{
	case DO_HORN_DOWN:
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_HIGH);
		break;
	case DO_HORN_UP:
		vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
		break;
	case DO_COOLING_ON:
		vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_HIGH);
		break;
	case DO_COOLING_OFF:
		vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
		break;
	default:
		break;
	}
}
