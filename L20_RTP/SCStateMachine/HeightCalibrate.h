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
//#define RESOLUTION 5 //5um
class HeightCalibrate: public SCState {
private:
	unsigned int m_HornDownTimeout;
	unsigned int m_HornHoldTimeout;
	unsigned int m_HornUpTimeout;
public:
	HeightCalibrate();
	virtual ~HeightCalibrate();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
};



#endif /* HEIGHTCALIBRATE_H_ */
