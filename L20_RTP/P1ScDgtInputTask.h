/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef P1SCDGTINPUTTASK_H_
#define P1SCDGTINPUTTASK_H_

#include "ScDgtInput.h"

class P1ScDgtInputTask: public ScDgtInputTask 
{
public:
							P1ScDgtInputTask();
	virtual 				~P1ScDgtInputTask();
protected:
	int						GetDgtInputBits			() override;
};

#endif /* P1SCDGTINPUTTASK_H_ */
