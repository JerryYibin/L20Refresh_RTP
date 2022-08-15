/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------

This Module is a Comb State for control Actuatmotions in Height Calibration and Check

Height Calibration Process: Automatically do Actuator Horn down/Hold/Horn Up cycle 3 times without shim, 
							get full zero and top position array

Height Check Process: 		Manually put a height-known shim on, do Horn down/Hold/Horn Up cycle once, 
							use the offset to do the correction on zero and top position array

***************************************************************************/

#include "HeightCalibrate.h"
#include "../ACStateMachine.h"
#include "../PCStateMachine.h"
#include "SCStateMachine.h"


/**************************************************************************//**
* \brief   - Constructor - 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
HeightCalibrate::HeightCalibrate() {
	m_Actions = SCState::INIT;
	m_State = SCState::CALIBRATE_CHECK;
	m_Index = 0;
	
	m_TaughtIdx.push_back(80); // Push in descending order
	m_TaughtIdx.push_back(50);
	m_TaughtIdx.push_back(20);
	m_IsCalibration = true;
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
HeightCalibrate::~HeightCalibrate() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}


/**************************************************************************//**
* 
* \brief   - HeightCalibrateCheck Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrate::Enter()
{
//	InitVariables(); 
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE); // Allow AC Move
	
	m_Index = 0;

}
/**************************************************************************//**
* 
* \brief   - HeightCalibrateCheck Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrate::Loop()
{
	unsigned int tmpHeight;
	if (ACStateMachine::AC_TX->ACState == ACState::AC_ALARM) 
	{
		m_Actions = SCState::FAIL;
		return;
	}
	
	switch(ACStateMachine::AC_TX->ACState)
	{
	case ACState::AC_READY:
		ACStateMachine::AC_RX->TargetPressure = m_TaughtIdx[m_Index] * PSI_FACTOR;
		if(m_Index < m_TaughtIdx.size())
		{
			ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_PART_CONTACT_ENABLE);//Contact Check Enable
			ACStateMachine::AC_RX->MasterState = SCState::DOWN_STROKE; 
		}
		else
		{
			m_Actions = SCState::JUMP;
		}
		break;
	case ACState::AC_DOWN_STROKE:
		if ((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_PART_CONTACT_FOUND)) == BIT_MASK(ACState::STATUS_PART_CONTACT_FOUND))
		{
			tmpHeight = getAverageHeight();
			if (tmpHeight >= 0)
			{
				CommonProperty::RawHeight[m_TaughtIdx[m_Index]].ZeroCount = (unsigned int)tmpHeight;
				ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_HOME_POSITION_ENABLE);
				ACStateMachine::AC_RX->MasterState = SCState::RETURN_STROKE;
			}
		}
		break;
	case ACState::AC_RETURN_STROKE:
		if((ACStateMachine::AC_TX->AC_StatusEvent & BIT_MASK(ACState::STATUS_HOME_POSITION_FOUND)) == BIT_MASK(ACState::STATUS_HOME_POSITION_FOUND))//Contact Check Enable)		
		{
			tmpHeight = getAverageHeight();
			if (tmpHeight >= 0)
			{
				CommonProperty::RawHeight[m_TaughtIdx[m_Index]].TopCount = (unsigned int)tmpHeight;
				CommonProperty::RawHeight[m_TaughtIdx[m_Index]].Calibrated = true;
				m_Index++;
			}
		}
		break;
	}
	
		
//			if (SCStateMachine::SC_RX.MasterEvents == SCStateMachine::CTRL_ACTUATOR_CHECK_ENABLE)
//			{
//				ACStateMachine::AC_TX->CheckHeight = (unsigned int)temp_height;
//			}




//				ACStateMachine::AC_RX->TargetPressure = m_CurrentPressureSetting;


}

/**************************************************************************//**
* 
* \brief   - HeightCalibrate Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrate::Exit()
{
	m_Actions = SCState::INIT; 
}

/**************************************************************************//**
* 
* \brief   - HeightCalibrateCheck Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrate::Fail()
{
	if (ProcessAlarmHandler() == true)
		m_Actions = SCState::ABORT;
	LOG("Height Calibrate Check Alarm process!\n");
}



/**************************************************************************//**
* 
* \brief   - Init Variables used in Height Calibration or Check Process 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrate::InitProcess()
{
	if (m_Index == 0)
	{
		for (int i = 0; i<= MAXPSI; i++)
		{
			CommonProperty::RawHeight[i].ZeroCount = 0;
			CommonProperty::RawHeight[i].TopCount = 0;
			CommonProperty::RawHeight[i].Calibrated = false;
		}
		m_CurrentPressureSetting = ACStateMachine::AC_RX->TargetPressure; // Store current SPI Setting before enter calibration
	}
	ACStateMachine::AC_RX->TargetPressure = m_TaughtIdx[m_Index] * PSI_FACTOR;
	
//	else if (SCStateMachine::SC_RX.MasterEvents == SCStateMachine::CTRL_ACTUATOR_CHECK_ENABLE)
//	{	
//		ACStateMachine::AC_RX->TargetPressure = SCStateMachine::SC_RX.Params[P_PRESSURE]; 
//	}
	
}


/**************************************************************************//**
* 
* \brief   - Get Average Height base on the height Sample taken in 500 ms. 
*
* \param   - None.
*
* \return  - height.
*
******************************************************************************/
int HeightCalibrate::getAverageHeight() 
{
	static unsigned int tmpHeightSum = 0, sample = 0, timeout = 0;
	int height = -1;

	if (timeout % 50 == 0)
	{
		tmpHeightSum += ACStateMachine::AC_TX->RawHeightCount;
		sample++;
		if (sample == 10)
		{
			height = (int)(tmpHeightSum / sample);
			tmpHeightSum = 0;
			sample = 0;
		}
		timeout = 0;
	}
	else
	{
		timeout++;
	}

	return height;
}



