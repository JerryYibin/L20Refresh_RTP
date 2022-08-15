/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------

***************************************************************************/

#ifndef SCSTATEMACHINE_HEIGHTUPDATE_H_
#define SCSTATEMACHINE_HEIGHTUPDATE_H_


#include "SCState.h"
#define MAXPSI 80
#define RESOLUTION 5 //5um
#define INIT_COUNT 30000
#define HEIGHT_UNIT 1000 //convert mm to um

class HeightCorrect: public SCState {
public:
	HeightCorrect();
	virtual ~HeightCorrect();
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
private:

	int UpdateEncoderArray();
	int CalculateHeight(const int pressure, map<int, unsigned int>* _taughtmap);
	void HeightCorrection(unsigned int pressure, unsigned int shim, unsigned int count);
};



#endif /* SCSTATEMACHINE_HEIGHTUPDATE_H_ */
