/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#ifndef L20SCDGTINPUTTASK_H_
#define L20SCDGTINPUTTASK_H_

#include "ScDgtInput.h"

class L20ScDgtInputTask: public ScDgtInputTask 
{
public:
							L20ScDgtInputTask		();
	virtual 				~L20ScDgtInputTask		();
protected:
	int						GetDgtInputBits			() override;
public:
	static void				DgtInterruptHandler		(void*);
};

#endif /* L20SCDGTINPUTTASK_H_ */
