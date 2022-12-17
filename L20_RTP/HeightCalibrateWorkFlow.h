/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef HEIGHTCALIBRATEWORKFLOW_H_
#define HEIGHTCALIBRATEWORKFLOW_H_

#include "WorkFlow.h"
#include <vector>

using namespace std;
class HeightCalibrateWorkFlow: public WorkFlow 
{
public:
	enum HEIGHT_WORKMODE
	{
		IDLE = 0,
		HEIGHT_CALIBRATE,
		HEIGHT_CHECK,
	};
private:
	unsigned int				m_Pressure;
	unsigned int				m_RawCount;
	vector<unsigned int> 		m_CalibrateReference;
	unsigned int 				m_CalibrateStep;
	unsigned int				m_OperationMode;
	STATE 						m_State;
private:
	int calculateHeightPoint(unsigned int min, unsigned int max, unsigned int start, unsigned int end);
public:
	HeightCalibrateWorkFlow();
	virtual ~HeightCalibrateWorkFlow();
public:
	virtual void 	InitProcess			(void) override;
	virtual void 	TriggerProcess		(void) override;
	virtual int 	UpdateResult		(void) override;
	virtual int 	RunProcess			(void) override;
	virtual void	ResetProcess		(void) override;
	virtual void 	SetPressure			(unsigned int pressure);
	virtual void 	SetCorrectHeight	(unsigned int height);
	virtual void 	SetOperationMode	(unsigned int operation);
	virtual int		GetOperationMode	();
};

#endif /* HEIGHTCALIBRATEWORKFLOW_H_ */
