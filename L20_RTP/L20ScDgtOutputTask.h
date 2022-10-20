/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef L20SCDGTOUTPUTTASK_H_
#define L20SCDGTOUTPUTTASK_H_

#include "ScDgtOutput.h"

class L20ScDgtOutputTask: public ScDgtOutputTask 
{
public:
					L20ScDgtOutputTask		();
	virtual 		~L20ScDgtOutputTask		();
protected:
	virtual int		SetDgtReadyOutput		() override;
	virtual int		ResetDgtReadyOutput		() override;
	virtual int		SetDgtSonicsOutput		() override;
	virtual int		ResetDgtSonicsOutput	() override;
	virtual int		SetDgtAlarmOutput		() override;
	virtual int		ResetDgtAlarmOutput		() override;

};

#endif /* L20SCDGTOUTPUTTASK_H_ */
