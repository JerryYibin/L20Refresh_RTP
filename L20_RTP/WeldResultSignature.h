/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#ifndef SIGNATURE_H_
#define SIGNATURE_H_
#include "WeldResultSignatureDefine.h"
#include "CommonProperty.h"
#include <vector>
#include "RecipeUI.h"

#define HMI_SIGNA_POINT_MAX           200
#define SELECT_POINT_NUM              198
#define SELECT_POINT_JUMP_THRESHOLD   0.1
#define SELECT_POINT_JUMP_MIN         1
#define SELECT_GRAPH_POINT_MIN        10

class WeldResultSignature{
public:
	enum EXTREME_POINTS
	{
	  NOT_EXTRENUN = 0,
	  IS_MIN_POINT,
	  IS_MAX_POINT
	};
private:
	static WeldResultSignatureDefine* ptrWeldSignature;
public:
	static vector<shared_ptr<WeldResultSignatureDefine>>* _OrignalSignature;
public:
	~WeldResultSignature();
	static int Set(const HMI_WELD_SIGNATURE*);
	static int Get(HMI_WELD_SIGNATURE*);
public:
	vector<GraphCurvePoint>   L20PowerGraphCurvePointVector;        /* Select power graph curve 200 points     */
	vector<GraphCurvePoint>   L20HeightGraphCurvePointVector;	     /* Select height graph curve 200 points    */
	vector<GraphCurvePoint>   L20FrquencyGraphCurvePointVector;     /* Select frquency graph curve 200 points  */
	vector<GraphCurvePoint>   L20AmplitudeGraphCurvePointVector;    /* Select Amplitude graph curve 200 points */
    void   SelectGraphCurvePointsHandler(int iGraphType);
    static WeldResultSignature*	GetInstance();
private:
	WeldResultSignature();
	void getGraphOriginData(vector<int>& vtOriginData, int iGraphType);
	void selectPointsFromOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint);
	void returnOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint);
	void getExtremumPoint(vector<int> vtOriginData, GraphCurvePoint& stuMinPoint, GraphCurvePoint& stuMaxPoint);
	int  checkIsExtremumPoint(GraphCurvePoint stuFirstPoint, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint);
	void selectSlopeChangePoint(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPointTmp, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint);
	void selectTargetPointFromSlopeChange(vector<GraphCurvePoint> vtSelectPointTmp, GraphCurvePoint stuMinPoint, GraphCurvePoint stuMaxPoint, vector<GraphCurvePoint>& vtSelectPoint);
	void selectAveragePointsFromOriginData(vector<int> vtOriginData, vector<GraphCurvePoint>& vtSelectPoint);
private:
	static WeldResultSignature	*m_WSObj;
};

#endif
