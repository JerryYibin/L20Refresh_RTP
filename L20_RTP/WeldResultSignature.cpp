/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#include "WeldResultSignature.h"
#include <algorithm>
#include <cmath>

WeldResultSignatureDefine* WeldResultSignature::ptrWeldSignature = WeldResultSignatureDefine::GetWeldSignature().get();
vector<shared_ptr<WeldResultSignatureDefine>>* WeldResultSignature::_OrignalSignature = new vector<shared_ptr<WeldResultSignatureDefine>>();
WeldResultSignature* WeldResultSignature::m_WSObj = nullptr;
/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResultSignature::WeldResultSignature()
{
	
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
WeldResultSignature::~WeldResultSignature(){
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Set UI Weld Results to SC Weld Results Signature
*
* \param   - HMI_WELD_SIGNATURE.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int WeldResultSignature::Set(const HMI_WELD_SIGNATURE* signature)
{
	if(signature == nullptr)
		return ERROR;
	unsigned int Frequency = signature->Frquency;
	unsigned int Power = signature->Power;
	unsigned int Height = signature->Height;
	ptrWeldSignature->Set(WeldResultSignatureDefine::FRQUENCY,	Frequency);
	ptrWeldSignature->Set(WeldResultSignatureDefine::POWER, 	Power);
	ptrWeldSignature->Set(WeldResultSignatureDefine::HEIGHT,	Height);
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get SC Weld Results  to UI Weld Results Signature
*
* \param   - HMI_WELD_SIGNATURE.
*
* \return  - if there is any error happened during the data setting, 
* 			 it will return ERROR; else it will return OK.
*
******************************************************************************/
int WeldResultSignature::Get(HMI_WELD_SIGNATURE* signature)
{
	if(signature == nullptr)
		return ERROR;

	signature->Frquency = ptrWeldSignature->Get(WeldResultSignatureDefine::FRQUENCY);
	signature->Power = ptrWeldSignature->Get(WeldResultSignatureDefine::POWER);
	signature->Height = ptrWeldSignature->Get(WeldResultSignatureDefine::HEIGHT);
	
	return OK;
}


/******************************************************************************
 * \brief   - Pick 200 points from the raw data for plotting the curve
 *
 * \param   - int iGraphType
 *
 * \return  - None
 *
 ******************************************************************************/
void WeldResultSignature::SelectGraphCurvePointsHandler(int iGraphType)
{
	int iOriSize = 0;
	//1. Filter original data by type
	vector<int> vtOriginData;
	getGraphOriginData(vtOriginData, iGraphType);
	iOriSize = vtOriginData.size();
	//2. Select 200 points to return to UI
	if(iOriSize > HMI_SIGNA_POINT_MAX)
	{
		//2.1 More than 200 pieces of data, perform data filtering
		switch(iGraphType)
		{
		case GRAPH_FRQUENCY:
			L20FrquencyGraphCurvePointVector.clear();
			selectPointsFromOriginData(vtOriginData, L20FrquencyGraphCurvePointVector);
			selectAveragePointsFromOriginData(vtOriginData, L20FrquencyGraphCurvePointVector);
			break;
		case GRAPH_POWER:
			L20PowerGraphCurvePointVector.clear();
			selectPointsFromOriginData(vtOriginData, L20PowerGraphCurvePointVector);
			selectAveragePointsFromOriginData(vtOriginData, L20PowerGraphCurvePointVector);
			break;
		case GRAPH_HEIGHT:
			L20HeightGraphCurvePointVector.clear();
			selectPointsFromOriginData(vtOriginData, L20HeightGraphCurvePointVector);
			selectAveragePointsFromOriginData(vtOriginData, L20HeightGraphCurvePointVector);
			break;
		case GRAPH_AMLITUDE:
			L20AmplitudeGraphCurvePointVector.clear();
			selectPointsFromOriginData(vtOriginData, L20AmplitudeGraphCurvePointVector);
			selectAveragePointsFromOriginData(vtOriginData, L20HeightGraphCurvePointVector);
			break;
		default:
			break;
		}
	}
	else if(iOriSize > 0 && iOriSize < HMI_SIGNA_POINT_MAX)
	{
		//2.2 Less than 200 pieces of data are sent directly to the UI
		switch(iGraphType)
		{
		case GRAPH_FRQUENCY:
			L20FrquencyGraphCurvePointVector.clear();
			returnOriginData(vtOriginData, L20FrquencyGraphCurvePointVector);
			break;
		case GRAPH_POWER:
			L20PowerGraphCurvePointVector.clear();
			returnOriginData(vtOriginData, L20PowerGraphCurvePointVector);
			break;
		case GRAPH_HEIGHT:
			L20HeightGraphCurvePointVector.clear();
			returnOriginData(vtOriginData, L20HeightGraphCurvePointVector);
			break;
		case GRAPH_AMLITUDE:
			L20AmplitudeGraphCurvePointVector.clear();
			returnOriginData(vtOriginData, L20AmplitudeGraphCurvePointVector);
			break;
		default:
			break;
		}
	}
	else
	{
		//2.3 No original data is obtained, return directly
		LOGERR((char *)"WeldResultSignature: GRAPH ORIGINAL DATA IS EMPTY\n", 0, 0, 0);
	}
}

/******************************************************************************
 * \brief   - Get the original data of different curves
 *
 * \param   - vector<int>& vtOriginData  
 *            int iGraphType.
 *
 * \return  - None
 *
 ******************************************************************************/
void WeldResultSignature::getGraphOriginData(vector<int>& vtOriginData, int iGraphType)
{
	int iOrignalsize = _OrignalSignature->size();

	if(iOrignalsize > 0)
	{
		switch(iGraphType)
		{
	    	case GRAPH_FRQUENCY:
	    	{
	    		for(vector<shared_ptr<WeldResultSignatureDefine>>::iterator iter = _OrignalSignature->begin(); iter != _OrignalSignature->end(); iter++)
	    		{
	    			int iFrquency = (*iter)->Get(WeldResultSignatureDefine::FRQUENCY);
	    			vtOriginData.push_back(iFrquency);
	    		}
	    		break;
	    	}
	    	case GRAPH_POWER:
	    	{
	    		for(vector<shared_ptr<WeldResultSignatureDefine>>::iterator iter = _OrignalSignature->begin(); iter != _OrignalSignature->end(); iter++)
	    		{
	    			int iPower = (*iter)->Get(WeldResultSignatureDefine::POWER);
	    			vtOriginData.push_back(iPower);
	    		}
	    		break;
	    	}
	    	case GRAPH_HEIGHT:
	    	{
	    		for(vector<shared_ptr<WeldResultSignatureDefine>>::iterator iter = _OrignalSignature->begin(); iter != _OrignalSignature->end(); iter++)
	    		{
	    			int iHeight = (*iter)->Get(WeldResultSignatureDefine::HEIGHT);
	    			vtOriginData.push_back(iHeight);
	    		}
	    		break;
	    	}
	    	case GRAPH_AMLITUDE:
	    	{
	    		for(vector<shared_ptr<WeldResultSignatureDefine>>::iterator iter = _OrignalSignature->begin(); iter != _OrignalSignature->end(); iter++)
	    		{
	    			int iAmplitude = (*iter)->Get(WeldResultSignatureDefine::AMPLITUDE);
	    			vtOriginData.push_back(iAmplitude);
	    		}
	    		break;
	    	}
	    	default:
	    		LOGERR((char *)"WeldResultSignature: GRAPH TYPE IS ERROR(%d)\n", iGraphType, 0, 0);
	    		break;  
		}
		
	}
}

/******************************************************************************
 * \brief   - Less than 200 points, directly return to the original data
 *
 * \param   - vector<int> vtOriginData 
 *            vector<GraphCurvePoint>& vtSelectPoint  
 *
 * \return  - None
 *
 ******************************************************************************/
void  WeldResultSignature::returnOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint)
{
	int iDataIndex = 0;
	int iOriginDataSize = 0;
	iOriginDataSize = vtOriginData.size();

	for(iDataIndex = 0; iDataIndex < iOriginDataSize; iDataIndex++)
	{
		GraphCurvePoint stuPointTmp;
		stuPointTmp.m_pointX = iDataIndex;
		stuPointTmp.m_pointY = vtOriginData[iDataIndex];
		vtSelectPoint.push_back(stuPointTmp);
	}
}

/******************************************************************************
 * \brief   - More than 200 points, evenly select 200 points with slope change
 *
 * \param   - vector<int> vtOriginData 
 *            vector<GraphCurvePoint>& vtSelectPoint
 *
 * \return  - None
 *
 ******************************************************************************/
 void  WeldResultSignature::selectPointsFromOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint)
 {
	 GraphCurvePoint stuMinPoint;
	 GraphCurvePoint stuMaxPoint;
	 vector<GraphCurvePoint> vtSelectSlopePoint;
	 //1. Find the maximum and minimum coordinates of the original data
	 getExtremumPoint(vtOriginData, stuMinPoint, stuMaxPoint);
	 //2. Find the point where the slope changes from the original data
	 selectSlopeChangePoint(vtOriginData, vtSelectSlopePoint, stuMinPoint, stuMaxPoint);
	 //3. Pick targets point for drawing the curve from the points where the slope changes
	 selectTargetPointFromSlopeChange(vtSelectSlopePoint, stuMinPoint, stuMaxPoint, vtSelectPoint);
 }

/******************************************************************************
 * \brief   - Find the maximum and minimum points of the original data
 *
 * \param   - vector<int> vtOriginData
 *            GraphCurvePoint& stuMinPoint
 *            GraphCurvePoint& stuMaxPoint
 *              
 *
 * \return  - None
 *
 ******************************************************************************/
 void WeldResultSignature::getExtremumPoint(vector<int> vtOriginData, GraphCurvePoint& stuMinPoint, GraphCurvePoint& stuMaxPoint)
 {
	 //1. maximum point
	 int iMaxValue    = *max_element(vtOriginData.begin(),vtOriginData.end()); 
	 int iMaxPosition = max_element(vtOriginData.begin(),vtOriginData.end()) - vtOriginData.begin();
	 //2. minimum point
	 int iMinValue    = *min_element(vtOriginData.begin(),vtOriginData.end());
	 int iMinPosition = min_element(vtOriginData.begin(),vtOriginData.end()) - vtOriginData.begin();
	 //3. Return the maximum and minimum points
	 stuMaxPoint.m_pointX = iMaxPosition;
	 stuMaxPoint.m_pointY = iMaxValue;
	 stuMinPoint.m_pointX = iMinPosition;
	 stuMinPoint.m_pointY = iMinValue;
 }

/******************************************************************************
 * \brief   - Check if this point is an extreme point
 *
 * \param   - GraphCurvePoint stuFirstPoint
 *            GraphCurvePoint stuMinPoint
 *            GraphCurvePoint stuMaxPoint
 *              
 *
 * \return  - None
 *
 ******************************************************************************/
int WeldResultSignature::checkIsExtremumPoint(GraphCurvePoint stuFirstPoint, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint)
{
    int iIsExtremumPoint = 0;
    // Check if the first point is the maximum or minimum point
    if((stuFirstPoint.m_pointX == stuMinPoint.m_pointX) && (stuFirstPoint.m_pointY == stuMinPoint.m_pointY))
    {
      iIsExtremumPoint = IS_MIN_POINT;
    }
    else if((stuFirstPoint.m_pointX == stuMaxPoint.m_pointX) && (stuFirstPoint.m_pointY == stuMaxPoint.m_pointY))
    {
      iIsExtremumPoint = IS_MAX_POINT;
    }
    else
    {
      iIsExtremumPoint = NOT_EXTRENUN;
    }

    return iIsExtremumPoint;
}

/******************************************************************************
 * \brief   - Find the point where the slope changes from the original data
 *
 * \param   - vector<int> vtOriginData 
 *            vector<GraphCurvePoint>& vtSelectPointTmp
 *            GraphCurvePoint stuMinPoint
 *            GraphCurvePoint stuMaxPoint
 *              
 *
 * \return  - None
 *
 ******************************************************************************/
void WeldResultSignature::selectSlopeChangePoint(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPointTmp, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint)
{
	int i = 0;
	int iOriginDataSize = vtOriginData.size();
	GraphCurvePoint stuPointPreTmp;
	float fSlopeTmp = 0.0;
  
	for(i=0; i<iOriginDataSize; i++)
	{
		float fSlopePre;
		GraphCurvePoint stuPointPre;
		stuPointPre.m_pointX = i;
		stuPointPre.m_pointY = vtOriginData[i];
		int iDetaX = abs(stuPointPre.m_pointX - stuPointPreTmp.m_pointX);
		int iDetaY = abs(stuPointPre.m_pointY - stuPointPreTmp.m_pointY);
		//1. If it is an extreme point,jump over
		if(checkIsExtremumPoint(stuPointPre, stuMinPoint, stuMaxPoint) != NOT_EXTRENUN)
		{
			continue;
		}
		//2. The slope does not change, drop it.
		if(iDetaX == 0 || iDetaY == 0)
		{
			continue;
		}
		//3. Calculate the slope value,and select the point where some rate changes
		fSlopePre = (float)iDetaY / (float)iDetaX;
		if(fSlopePre != fSlopeTmp)
		{
			fSlopeTmp = fSlopePre;
			stuPointPreTmp = stuPointPre;
			vtSelectPointTmp.push_back(stuPointPreTmp);
		}
	}
}

/******************************************************************************
 * \brief   - Pick target points for drawing the curve from the points where the slope changes
 *
 * \param   - vector<GraphCurvePoint> vtSelectPointTmp
 *            GraphCurvePoint stuMinPoint
 *            GraphCurvePoint stuMaxPoint
 *            vector<GraphCurvePoint>& vtSelectPoint
 *              
 *
 * \return  - None
 *
 ******************************************************************************/
void WeldResultSignature::selectTargetPointFromSlopeChange(vector<GraphCurvePoint> vtSelectPointTmp, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint, vector<GraphCurvePoint>& vtSelectPoint)
{
	int iSelectPointSize = vtSelectPointTmp.size();
	int i = 0;
	int j = 0;
	int iCountNum = 0;
	vector<GraphCurvePoint> vtSelectPointTarget;
	vector<int> vtSelectPointX;
	int intervalNum = SELECT_POINT_JUMP_MIN;
	// 1. Get the point interval
	if(iSelectPointSize >= SELECT_POINT_NUM)
	{
		float intervalNumTmp = (float)iSelectPointSize / (float)SELECT_POINT_NUM;
		intervalNum = round(intervalNumTmp);
		if((intervalNumTmp - (float)intervalNum) > SELECT_POINT_JUMP_THRESHOLD)
		{
			intervalNum += SELECT_POINT_JUMP_MIN;
		}
	}
	else
	{
		intervalNum = SELECT_POINT_JUMP_MIN;
	}
	//2. Get target point
	for(i = 0; i < iSelectPointSize; i = i + intervalNum)
	{
		if(i >= iSelectPointSize)
		{
			break;
		}
		if(iCountNum >= SELECT_POINT_NUM)
		{
			break;
		}
		vtSelectPointTarget.push_back(vtSelectPointTmp[i]);
		vtSelectPointX.push_back(vtSelectPointTmp[i].m_pointX);
		iCountNum++;
	}
	vtSelectPointTarget.push_back(stuMinPoint);
	vtSelectPointTarget.push_back(stuMaxPoint);
	vtSelectPointX.push_back(stuMinPoint.m_pointX);
	vtSelectPointX.push_back(stuMaxPoint.m_pointX);
	//3. Sort by abscissa from smallest to largest
	sort(vtSelectPointX.begin(), vtSelectPointX.end());
	for(i = 0; i < vtSelectPointX.size(); i++)
	{
		for(j =0; j< vtSelectPointTarget.size(); j++)
		{
			if(vtSelectPointX[i] == vtSelectPointTarget[j].m_pointX)
			{
				vtSelectPoint.push_back(vtSelectPointTarget[j]);
				break;
			}
		}
	}
}

/******************************************************************************
 * \brief   - If the selected points are less than 10, take an average of 200 points
 *
 * \param   - vector<int> vtOriginData
 * 			  vector<GraphCurvePoint>& vtSelectPoint
 *              
 *
 * \return  - None
 *
 ******************************************************************************/
void  WeldResultSignature::selectAveragePointsFromOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint)
{
	int i = 0;
	int intervalNum = SELECT_POINT_JUMP_MIN;
	int vtOriginDataSize = vtOriginData.size();
	
	if(vtSelectPoint.size() < SELECT_GRAPH_POINT_MIN)
	{
		vtSelectPoint.clear();
		float intervalNumTmp = (float)vtOriginDataSize / (float)HMI_SIGNA_POINT_MAX;
		intervalNum = round(intervalNumTmp);
		if((intervalNumTmp - (float)intervalNum) > SELECT_POINT_JUMP_THRESHOLD)
		{
			intervalNum += SELECT_POINT_JUMP_MIN;
		}
		
		for(i =0; i < vtOriginDataSize; i = i + intervalNum)
		{
			GraphCurvePoint GraphCurvePointTmp;
			GraphCurvePointTmp.m_pointX = i;
			GraphCurvePointTmp.m_pointY = vtOriginData[i];
			vtSelectPoint.push_back(GraphCurvePointTmp);
		}
	}
}

/******************************************************************************
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - WeldResultSignature* Object 
*
******************************************************************************/
WeldResultSignature* WeldResultSignature::GetInstance()
{
	return (m_WSObj != nullptr) ? m_WSObj : (m_WSObj = new WeldResultSignature());
}


