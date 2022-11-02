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
#include "../ControlTask.h"
#include "../HeightEncoder.h"
#include "SCStateMachine.h"

#define TIMEDELAY 1000
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
	m_State = SCState::CALIBRATE;
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
	ACStateMachine::AC_RX->MasterEvents &= ~BIT_MASK(ACState::CTRL_AC_MOVE_DISABLE); // Allow AC Move
	m_HornDownTimeout = 0;
	m_HornHoldTimeout = 0;
	m_HornUpTimeout = 0;
	HeightEncoder::HeightProperty.TotalTime = 0;
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
		if((m_HornUpTimeout == 0) && (m_HornHoldTimeout == 0) && (m_HornDownTimeout == 0))
		{
			// At the first time running when AC in the Ready state.
			HeightEncoder::RawHeight.TopCount = ACStateMachine::AC_TX->RawHeightCount;
			ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_PART_CONTACT_ENABLE);//Contact Check Enable
			ACStateMachine::AC_RX->MasterState = SCState::DOWN_STROKE; 
	
		}
		else
		{
			if(m_HornUpTimeout != 0)
			{
				HeightEncoder::RawHeight.TopCount = ACStateMachine::AC_TX->RawHeightCount;
				HeightEncoder::HeightProperty.HornUpTime = m_HornUpTimeout;
				HeightEncoder::HeightProperty.TotalTime += m_HornUpTimeout;
				m_Actions = SCState::JUMP;
			}
		}

		break;
	case ACState::AC_DOWN_STROKE:
		m_HornDownTimeout++;
		break;
	case ACState::AC_HOLD:
		if(m_HornDownTimeout != 0)
		{
			HeightEncoder::HeightProperty.HornDownTime = m_HornDownTimeout;
			HeightEncoder::HeightProperty.TotalTime += m_HornDownTimeout;
			m_HornDownTimeout = 0;
		}
		if (m_HornHoldTimeout >= TIMEDELAY)
		{
			HeightEncoder::HeightProperty.TotalTime += m_HornHoldTimeout;
			HeightEncoder::RawHeight.ZeroCount = ACStateMachine::AC_TX->RawHeightCount;
			HeightEncoder::HeightProperty.ActualHeight = ACStateMachine::AC_TX->ActualHeight;
			HeightEncoder::HeightProperty.Stroke = (HeightEncoder::RawHeight.TopCount - HeightEncoder::RawHeight.ZeroCount) * RESOLUTION;
			ACStateMachine::AC_RX->MasterEvents |= BIT_MASK(ACState::CTRL_ULS_ENABLE);
			ACStateMachine::AC_RX->MasterState = SCState::RETURN_STROKE;
		}
		else
			m_HornHoldTimeout++;
		break;
	case ACState::AC_RETURN_STROKE:
		m_HornUpTimeout++;
		break;
	default:
		break;
	}
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
	MESSAGE message;
	m_Actions = SCState::INIT; 
	HeightEncoder::RawHeight.Calibrated = true;
	message.msgID = ControlTask::TO_CTRL_SC_RESPONSE;
	SendToMsgQ(message, CTL_MSG_Q_ID);
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
