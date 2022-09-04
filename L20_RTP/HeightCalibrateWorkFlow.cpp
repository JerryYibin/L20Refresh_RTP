/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#include "HeightCalibrateWorkFlow.h"
#include "SCStateMachine/SCStateMachine.h"
#include "ACStateMachine.h"
#include "HeightEncoder.h"
#include "commons.h"
#define FIRST_PRESSURE		80
#define SECOND_PRESSURE		50
#define THIRD_PRESSURE		20
#define MIN_POINT_SIZE		2
#define MIN_PRESSURE_SET	10
/**************************************************************************//**
* \brief   - Constructor - 
* There are three times horn down and horn up with the 20, 50 and 80 PSI during the height calibration.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
HeightCalibrateWorkFlow::HeightCalibrateWorkFlow() {
	m_CalibrateReference.push_back(FIRST_PRESSURE); // Push in descending order
	m_CalibrateReference.push_back(SECOND_PRESSURE);
	m_CalibrateReference.push_back(THIRD_PRESSURE);
	m_CalibrateStep = 0;
	m_OperationMode = IDLE;
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
HeightCalibrateWorkFlow::~HeightCalibrateWorkFlow() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Initialize height calibration process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrateWorkFlow::InitProcess(void)
{
	m_CalibrateStep = 0;
	m_Pressure = m_CalibrateReference[0] * PRESSURE_FACTOR;
	for(int i = 0; i < PRESSURE_NUM; i++)
	{
		HeightEncoder::HeightCalibratedMap[i].Calibrated = false;
	}
}

/**************************************************************************//**
* 
* \brief   - Trigger height calibration process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrateWorkFlow::TriggerProcess(void)
{
	HeightEncoder::HeightProperty.Pressure = m_Pressure;
	ACStateMachine::AC_RX->TargetPressure = m_Pressure;
	SCStateMachine::getInstance()->ExecuteStateAction(SCState::READY_FOR_TRIGGER);
}

/**************************************************************************//**
* 
* \brief   - Update height calibration Table.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int HeightCalibrateWorkFlow::UpdateResult(void)
{
	vector<int> CalibratedPoint;
	int MinPoint = 0, MaxPoint = 0, StartPoint = 0, EndPoint = 0;
	unsigned int iCount = 0;
	for (auto iter = HeightEncoder::HeightCalibratedMap.begin(); iter != HeightEncoder::HeightCalibratedMap.end(); ++iter)	//Separate map of pairs<pressure, encoder count> to two vectors
	{
		if (iter->second.Calibrated == true)
		{
			CalibratedPoint.push_back(iter->first);
		}
	}

	if (CalibratedPoint.size() < MIN_POINT_SIZE)
		return ERROR;
	MinPoint = CalibratedPoint.front();
	MaxPoint = CalibratedPoint.back();
	while (iCount < CalibratedPoint.size())
	{

		StartPoint = CalibratedPoint[iCount];
		if ((iCount + 1) < CalibratedPoint.size())
		{
			EndPoint = CalibratedPoint[iCount + 1];
			if(calculateHeightPoint(MinPoint, MaxPoint, StartPoint, EndPoint) == ERROR)
				break;
		}
		iCount++;
	};
	
	if(iCount != CalibratedPoint.size())
		return ERROR;

	for (int i = 0; i < MIN_PRESSURE_SET; i++)
	{
		HeightEncoder::HeightCalibratedMap[i].ZeroCount = HEIGHT_BASIC_COUNT;
		HeightEncoder::HeightCalibratedMap[i].TopCount = HEIGHT_HOME_COUNT;
		HeightEncoder::HeightCalibratedMap[i].Calibrated = false;
	}
#if HEIGHT_CALIBRATE_DBG
	for(int i = 0; i < HeightEncoder::HeightCalibratedMap.size(); i++)
	{
		LOG("PSI = %d Zero = %d\n", i, HeightEncoder::HeightCalibratedMap[i].ZeroCount);
	}
#endif
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Run height calibration Work Flow that should be compatible with calibration and check work flow.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int HeightCalibrateWorkFlow::RunProcess(void)
{
	STATE state = WorkFlow::ONGOING;
	int index = 0;
	switch(m_OperationMode)
	{
	case HEIGHT_CALIBRATE:
		m_CalibrateStep++;
		index = (int)(HeightEncoder::HeightProperty.Pressure / PRESSURE_FACTOR);
		HeightEncoder::HeightCalibratedMap[index] = HeightEncoder::RawHeight;
		if(m_CalibrateStep < m_CalibrateReference.size())
		{
			m_Pressure = m_CalibrateReference[m_CalibrateStep] * PRESSURE_FACTOR;
			TriggerProcess();
		}
		else
		{
			if(UpdateResult() == OK)
				state = WorkFlow::FINISH;
			else
				state = WorkFlow::INVALID;
		}
		break;
	case HEIGHT_CHECK:
		m_RawCount = HeightEncoder::RawHeight.ZeroCount;
		state = WorkFlow::FINISH;
		break;
	default:
		break;
	}
	return state;
}

/**************************************************************************//**
* 
* \brief   - Set Pressure 
*
* \param   - press * 1000.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrateWorkFlow::SetPressure(unsigned int pressure)
{
	m_Pressure = pressure;
}

void HeightCalibrateWorkFlow::SetCorrectHeight(unsigned int height)
{
	unsigned int correctZeroHeight = m_RawCount - height / RESOLUTION;
	HeightEncoder::RawHeight.ZeroCount = correctZeroHeight;
	unsigned int index = m_Pressure / PRESSURE_FACTOR;
	if((index > MIN_PRESSURE_SET) && (index < HeightEncoder::HeightCalibratedMap.size()))
		HeightEncoder::HeightCalibratedMap[index] = HeightEncoder::RawHeight;
}

/**************************************************************************//**
* 
* \brief   - Set Operation mode to Calibration/ Check 
*
* \param   - operation.
*
* \return  - None.
*
******************************************************************************/
void HeightCalibrateWorkFlow::SetOperationMode(unsigned int operation)
{
	m_OperationMode = operation;
}

int	HeightCalibrateWorkFlow::GetOperationMode()
{
	return m_OperationMode;
}

/**************************************************************************//**
* 
* \brief   - Calculate each point of the height calibration table following the point of got from horn down and horn up(20 psi, 50 psi, 80 psi)
*
* \param   - min is the minimum calibrated data, max is the maximum calibrated data, 
* start is the current start point that is picked up from calibrated data group.
* end is the current end point that is picked up from calibrated data group as well.  
*
* \return  - None.
*
******************************************************************************/
int HeightCalibrateWorkFlow::calculateHeightPoint(unsigned int min, unsigned int max, unsigned int start, unsigned int end)
{
	float Slop_Zero, C_Zero, Slop_Top, C_Top;
	HEIGHT_ENCODER tmpHeight;
	if(end == start)
		return ERROR;
	auto iterMin = HeightEncoder::HeightCalibratedMap.find(min);
	auto iterMax = HeightEncoder::HeightCalibratedMap.find(max);
	auto iterStart = HeightEncoder::HeightCalibratedMap.find(start);
	auto iterEnd = HeightEncoder::HeightCalibratedMap.find(end);

	int DeltaCount = iterEnd->second.ZeroCount - iterStart->second.ZeroCount;
	Slop_Zero = (float)(DeltaCount) / (end - start);
	C_Zero = iterStart->second.ZeroCount - Slop_Zero * start;
	DeltaCount = iterEnd->second.TopCount - iterStart->second.TopCount;
	Slop_Top = (float)(DeltaCount) / (end - start);
	C_Top = iterStart->second.TopCount - Slop_Top * start;

	if ((min == start) && (max != end))
	{
		for (unsigned int i = MIN_PRESSURE_SET; i < end; i++)
		{
			if (i != start)
			{
				tmpHeight.Calibrated = false;
				tmpHeight.ZeroCount = (float)Slop_Zero * i + C_Zero;
				tmpHeight.TopCount = (float)Slop_Top * i + C_Top;
				HeightEncoder::HeightCalibratedMap[i] = tmpHeight;
			}
		}
	}
	else if ((min != start) && (max != end))
	{
		for (int i = start + 1; i < end; i++)
		{
			tmpHeight.Calibrated = false;
			tmpHeight.ZeroCount = (float)Slop_Zero * i + C_Zero;
			tmpHeight.TopCount = (float)Slop_Top * i + C_Top;
			HeightEncoder::HeightCalibratedMap[i] = tmpHeight;
		}
	}
	else if ((min != start) && (max == end))
	{
		for (int i = start + 1; i < PRESSURE_NUM; i++)
		{
			if (i != end)
			{
				tmpHeight.Calibrated = false;
				tmpHeight.ZeroCount = (float)Slop_Zero * i + C_Zero;
				tmpHeight.TopCount = (float)Slop_Top * i + C_Top;
				HeightEncoder::HeightCalibratedMap[i] = tmpHeight;
			}
		}
	}
	else
	{
		for (int i = MIN_PRESSURE_SET; i < PRESSURE_NUM; i++)
		{
			if ((i != end) && (i != start))
			{
				tmpHeight.Calibrated = false;
				tmpHeight.ZeroCount = (float)Slop_Zero * i + C_Zero;
				tmpHeight.TopCount = (float)Slop_Top * i + C_Top;
				HeightEncoder::HeightCalibratedMap[i] = tmpHeight;
			}
		}
	}
	return OK;
}
