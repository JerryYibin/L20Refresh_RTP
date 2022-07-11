/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef PREREADY_H_
#define PREREADY_H_

#include "SCState.h"
#define TOP_CHECK_TIMEOUT	1000
#define RDCHECK				300/5 
#define TWICE				2
#define MAX_HEIGHT			38005
class PreReady: public SCState {
public:
	PreReady();
	virtual ~PreReady();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* PREREADY_H_ */
