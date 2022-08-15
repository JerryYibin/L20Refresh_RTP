/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "L20ActuatorTask.h"
#include "HeightEncoder.h"
#include "Utility.h"
//#include "ACState.h"
CommunicationInterface_CAN* L20ActuatorTask::_ObjDCan 	= nullptr;
ACStateMachine::RxPDO_AC L20ActuatorTask::RXBackup;
UINT32 L20ActuatorTask::Tick_1ms 						= 0;
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
	unsigned int status = GetCoreState();
	CommunicationInterface_CAN::TX_MESSAGE tmpPressure;
	if(RXBackup.TargetPressure != ACStateMachine::AC_RX->TargetPressure)
	{
		tmpPressure.DAC_Pressure = Utility::Pressure2HEX(ACStateMachine::AC_RX->TargetPressure);
		if(_ObjDCan->Sending(&tmpPressure) == ERROR)
		{
			status |= ERR_PRESSURE_SET;
			SetCoreState(status);
		}
		else
		{
			status &= ~ERR_PRESSURE_SET;
			SetCoreState(status);
		}
		RXBackup.TargetPressure = ACStateMachine::AC_RX->TargetPressure;
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
	ACStateMachine::AC_TX->ActualHeight = HeightEncoder::GetActualHeight();
	ACStateMachine::AC_TX->RawHeightCount = HeightEncoder::GetPositionCount();
	Tick_1ms++;
	if(Tick_1ms % 5 == 0)
	{
		if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE)) == BIT_MASK(ACState::STATUS_AC_MOVE_DISABLE))
			InitMovingProcess();
		else
			m_IsMoving = MovingCheckProcess();
		
	}
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
