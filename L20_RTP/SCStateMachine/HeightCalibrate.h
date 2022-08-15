/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef HEIGHTCALIBRATE_H_
#define HEIGHTCALIBRATE_H_


#include "SCState.h"

#define DELAY6SEC 6000
#define MAXPSI 80
#define	PSI_FACTOR 1000

class HeightCalibrate: public SCState {
public:
	HeightCalibrate();
	virtual ~HeightCalibrate();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
private:
	vector<unsigned int> m_TaughtIdx;
	unsigned int m_Index;
	bool m_IsCalibration;
	int m_CurrentPressureSetting;

	void InitProcess();
	int getAverageHeight();
};



#endif /* HEIGHTCALIBRATE_H_ */
