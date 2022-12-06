/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SEEK_H_
#define SEEK_H_

#include "SCState.h"
#define WELD_SEEK_ON_TIME 25
#define TEST_SEEK_ON_TIME 350	
class SeekSonicOn: public SCState
{
public:
	enum SEEK_TYPE
	{
		WELD_SEEK_ON = 0,
		TEST_SEEK_ON
	};
	
public:
	SeekSonicOn(SEEK_TYPE Type = WELD_SEEK_ON);
	~SeekSonicOn();
private:
	SEEK_TYPE m_SeekType;
	int		  m_SeekTime;
public:
	virtual void Enter() override;
	virtual void Loop() override;
	virtual void Exit() override;
	virtual void Fail() override;
};

#endif /* SEEK_H_ */
