/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#ifndef P1SCDGTOUTPUTTASK_H_
#define P1SCDGTOUTPUTTASK_H_

#include "ScDgtOutput.h"

class P1ScDgtOutputTask: public ScDgtOutputTask 
{
public:
					P1ScDgtOutputTask		();
	virtual 		~P1ScDgtOutputTask		();
protected:
	virtual int 	SetDgtReadyOutput		() override;
	virtual int 	ResetDgtReadyOutput		() override;
	virtual int 	SetDgtSonicsOutput		() override;
	virtual int 	ResetDgtSonicsOutput	() override;
	virtual int 	SetDgtAlarmOutput		() override;
	virtual int 	ResetDgtAlarmOutput		() override;
};

#endif /* P1SCDGTOUTPUTTASK_H_ */
