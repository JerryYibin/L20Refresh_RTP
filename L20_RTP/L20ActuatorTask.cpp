/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "L20ActuatorTask.h"
#include "HeightEncoder.h"
#include "Utility.h"
#include "commons.h"
#include "DAC_SPI.h"
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
	ACStateMachine::AC_RX->TargetPressure = CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure;
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
	if((Tick_1ms & 1000) == 0)
		return;
	else
//	if(RXBackup.TargetPressure != ACStateMachine::AC_RX->TargetPressure)
	{
		tmpPressure.DAC_Pressure = Utility::Pressure2HEX(ACStateMachine::AC_RX->TargetPressure);
#if INCLUDE_TI_AM5708_JN
		iResult = _ObjDCan->Sending(&tmpPressure);
#elif INCLUDE_TI_AM5708_BRANSON
		//TODO It is the temporarily approach for the pressure setting on Branson level 1 board only. 
		iResult = DAC_TLV5604::SetPressure(tmpPressure.DAC_Pressure);
#else
		iResult = OK;
#endif
		if(iResult == ERROR)	
			SetCoreState(ERR_PRESSURE_SET);
		else
			ClearCoreState(ERR_PRESSURE_SET);
//		RXBackup.TargetPressure = ACStateMachine::AC_RX->TargetPressure;
	}
}

/**************************************************************************//**
* \brief  - Read relevant data from Actuator.
* 			
*			INT32  	ActualHeight;
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
* \brief   - To check if the horn is still moving
*
* \param   - None.
*
* \return  - None
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
	// the sum of delta array should be the distance per 20ms.
	// in order to change the velocity unit to mm/s, the value need to time 50, right? 
	tmpSpeed = (DeltaPosition[3] * 60 + DeltaPosition[2] * 60 + DeltaPosition[1] * 40 + DeltaPosition[0] * 40); 
#if HEIGHT_CALIBRATE_DBG	
//	LOG("Current Count = %d\n", EncoderPosition);
//	for(int i = 0; i < 4; i++)
//	{
//		LOG("DeltaPosition[%d] = %d\n", i, DeltaPosition[i]);
//	}
//	LOG("tmpSpeed = %d\n", tmpSpeed);
	LOG("ABS(Speed) = %d\n", _ABS(tmpSpeed));
#endif
	return  _ABS(tmpSpeed);
}


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


bool L20ActuatorTask::IsMoving()
{
	return m_IsMoving;
}

unsigned int L20ActuatorTask::GetMaxSpeed()
{
	return MaxSpeed;
}

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
}

