/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef TESTSONICON_H_
#define TESTSONICON_H_
#include "SCState.h"
#include "SonicsTestUI.h"
#define DEDAULT_WELD_TIME	500
using namespace std;
class TestSonicOn: public SCState 
{
public:
	TestSonicOn();
	virtual ~TestSonicOn();
public:
	virtual void 	Enter() override;
	virtual void 	Loop() override;
	virtual void 	Exit() override;
	virtual void 	Fail() override;
	static 	int		Execute(void* _obj);
};

#endif /* TESTSONICON_H_ */
