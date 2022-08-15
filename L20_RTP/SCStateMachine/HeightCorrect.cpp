/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
HeightCorrect state is used for handle all Calibration, Check Correction related calculation.


The Zero Encoder Array and Top Encoder Array containing ENCODER points from 0PSI~ 80 PSI(with 1 PSI increment) 
will be calculated based on the encoder value (corresponding to specific reference pressure)
collected/updated during calibration or check process 

Incoming Command from HMI:

Correct: Do encoder array correction based on latest check results
Accept: Save current Encoder array to data base -- Handles outside this State
Exit: Overwrite Encoder Array with previous value stored in database-- Handles outside this State

***************************************************************************/

#include "HeightCorrect.h"
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
HeightCorrect::HeightCorrect() {
	m_Actions = SCState::INIT;
	m_State = SCState::HEIGHTCORRECT;
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

HeightCorrect::~HeightCorrect() {
	m_Actions = SCState::INIT;
	m_State = SCState::NO_STATE;
}


/**************************************************************************//**
* 
* \brief   - HeightCorrect Enter.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCorrect::Enter()
{
	m_Timeout = 0;
	UpdateEncoderArray();        //Update Encoder Array based on current taught values 
}


/**************************************************************************//**
* 
* \brief   - HeightCorrect Loop.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCorrect::Loop()
{

//	if (ACStateMachine::AC_TX->ACState == ACState::AC_ALARM)
//	{
//		m_Actions = SCState::FAIL;
//	}
//	
//	if (SCStateMachine::SC_RX.MasterEvents == SCStateMachine::CTRL_ACTUATOR_CORRECT_ENABLE) 
//	{
////		HeightCorrection(SCStateMachine::SC_RX.Params[P_PRESSURE],SCStateMachine::SC_RX.Params[P_SHIM], ACStateMachine::AC_TX->CheckHeight);
//		m_Actions = SCState::JUMP;
//	}
//	
//	if ((SCStateMachine::SC_RX.MasterEvents == SCStateMachine::CTRL_ACTUATOR_ACCEPT_ENABLE)||(SCStateMachine::SC_RX.MasterEvents == SCStateMachine::CTRL_ACTUATOR_EXIT_ENABLE))
//	{
//		m_Actions = SCState::JUMP;
//	}
  
}

/**************************************************************************//**
* 
* \brief   - HeightCorrect Exit.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCorrect::Exit()
{
	
}

/**************************************************************************//**
* 
* \brief   - HeightCorrect Fail.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/

void HeightCorrect::Fail()
{
	if (ProcessAlarmHandler() == true)
		m_Actions = SCState::ABORT;
	LOG("Height Correct Alarm process!\n");
}



/**************************************************************************//**
* 
* \brief   - Update Full Zero/Top Encoder Array for 0 PSI ~80 PSI
*
* \param   - None.
*
* \return  - Update Status -1: Update Fail
* 							0: Update Successfully
*
******************************************************************************/

int HeightCorrect::UpdateEncoderArray()
{
	map<int, unsigned int> _taughtZeroMap; // To Store all taught Zero points pairs <pressure, Zero encoder count > in map
	map<int, unsigned int> _taughtTopMap;  // To Store all taught Top points pairs <pressure, Top encoder count > in map
	
	for(int i = 0; i<PRESSURE_NUM ; i++)
	{
		if(CommonProperty::RawHeight[i].Calibrated == true)
		{
			_taughtZeroMap.insert(pair < int, unsigned int > (i, CommonProperty::RawHeight[i].ZeroCount));
			_taughtTopMap.insert(pair < int, unsigned int >  (i, CommonProperty::RawHeight[i].TopCount));
		}
	}
	
	if (_taughtZeroMap.size() < 2) // If less than 3 taught points, don't update the encoder array
		return ERROR;

    for (int i=0; i<= MAXPSI; i++)
    {
    	CommonProperty::RawHeight[i].ZeroCount = CalculateHeight(i,&_taughtZeroMap);
    	CommonProperty::RawHeight[i].TopCount = CalculateHeight(i,&_taughtTopMap);
    }
    
    return OK;
}


/**************************************************************************//**
* 
* \brief   - Calculate RawHeight(encoder value) based on input pressure
* 
*  Encoder value should be linear proportional to Pressure Value
*
* \param   - Param1: Pressure,  param2: pointer to map store taught point pairs<pressure, encoder count>
*
* \return  - Encoder value corresponding to Input pressure
* 					
******************************************************************************/

int HeightCorrect::CalculateHeight(const int pressure, map<int, unsigned int>* _taughtmap)
{
	int height = 0;
	float slop, C;
	
	vector<unsigned int> _pressureIdx;
	vector<unsigned int> _heightIdx;
	

	for (auto iter = _taughtmap->begin(); iter != _taughtmap->end(); ++iter)	//Separate map of pairs<pressure, encoder count> to two vectors
	{
		_pressureIdx.push_back(iter->first);
		_heightIdx.push_back(iter->second);										
	}
	

	for (int i = 0; i< _pressureIdx.size()-1; i++)   
	{

		if (i == 0)
		{
			if (pressure <_pressureIdx[i])  // For pressure drops before the first taught pressure, use the first two points to calculate
			{

				slop = (_heightIdx[i+1] - _heightIdx[i])/(_pressureIdx[i+1] - _pressureIdx[i]);
				C = _heightIdx[i] - slop * _pressureIdx[i];
				height = (int)(pressure * slop + C);
				break;
			}
		}

		if (i == (_pressureIdx.size()-2))
		{
			if(pressure >= _pressureIdx[i+1]) // For pressure drops after the last taught pressure, use the last two points to calculate
			{

				slop = (_heightIdx[i+1] - _heightIdx[i])/(_pressureIdx[i+1] - _pressureIdx[i]);
				C = _heightIdx[i] - slop * _pressureIdx[i];
				height = (int)(pressure * slop + C);
				break;
			}
		}

		if ((pressure >= _pressureIdx[i]) && (pressure < _pressureIdx[i+1]))// For pressure drops between two taught pressure
		{
			slop = (_heightIdx[i+1] - _heightIdx[i])/(_pressureIdx[i+1] - _pressureIdx[i]);
			C = _heightIdx[i] - slop * _pressureIdx[i];
			height = (int)(pressure * slop + C);
			break;
		}

	}


	return height;

}



/**************************************************************************//**
* 
* \brief   - Correct the encoder count of specific pressure based on the count and real shim height from last Check
* 
*  			 Encoder value should be linear proportional to Pressure Value
*
* \param   - Param1: Pressure,  param2: real height -- in mm unit ,  param3: Reading encoder count
*
* \return  - None
* 					
******************************************************************************/

void HeightCorrect::HeightCorrection(unsigned int pressure, unsigned int shim, unsigned int count)
{
	int offset;

	offset = (int)(shim * HEIGHT_UNIT/RESOLUTION) + CommonProperty::RawHeight[pressure].ZeroCount - count;

	CommonProperty::RawHeight[pressure].ZeroCount += offset;
	CommonProperty::RawHeight[pressure].TopCount += offset;
	CommonProperty::RawHeight[pressure].Calibrated = true;

	UpdateEncoderArray();

}


