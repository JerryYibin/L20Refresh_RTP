/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef TESTSONICSWORKFLOW_H_
#define TESTSONICSWORKFLOW_H_

#include "WorkFlow.h"
#include "SonicsTestUI.h"
class TestSonicsWorkFlow: public WorkFlow 
{
private:
	unsigned int				m_Amplitude;
public:
	static SONICS_TEST 	TestResult;
	static unsigned int	TriggerCounter;
public:
	TestSonicsWorkFlow();
	virtual ~TestSonicsWorkFlow();
public:
	virtual void 	InitProcess			(void) override;
	virtual void 	TriggerProcess		(void) override;
	virtual int 	UpdateResult		(void) override;
	virtual int 	RunProcess			(void) override;
	virtual void	ResetProcess		(void) override;
	virtual void 	SetAmplitude		(unsigned int amplitude);
};

#endif /* TESTSONICSWORKFLOW_H_ */
