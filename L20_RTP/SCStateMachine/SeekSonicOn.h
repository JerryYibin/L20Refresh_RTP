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
#define DELAY25MSEC 25
class SeekSonicOn: public SCState
{
public:
	SeekSonicOn();
	~SeekSonicOn();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* SEEK_H_ */
