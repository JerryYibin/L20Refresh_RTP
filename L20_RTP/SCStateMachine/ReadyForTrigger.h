/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ---------------------------- 
 It is the common state for all the feature that need to trigger from the request of HMI  
 
***************************************************************************/

#ifndef READYFORTRIGGER_H_
#define READYFORTRIGGER_H_

#include "SCState.h"
//#include "SCAction.h"

class ReadyForTrigger: public SCState/*, public SCAction*/
{
public:
	ReadyForTrigger();
	virtual ~ReadyForTrigger();
public:
	virtual void 	Enter() 	override;
	virtual void 	Loop() 		override;
	virtual void 	Exit() 		override;
	virtual void 	Fail() 		override;
	static int		Execute(void* _obj);
};



#endif /* READYFORREQUEST_H_ */
