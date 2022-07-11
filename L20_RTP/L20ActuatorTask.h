/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef L20ACTUATORTASK_H_
#define L20ACTUATORTASK_H_

#include "ActuatorTask.h"

class L20ActuatorTask: public ActuatorTask 
{
public:
	L20ActuatorTask();
	virtual				~L20ActuatorTask			();
	virtual void		PDOUploadRequest			() override;
	virtual void		PDODownloadRequest			() override;
	
};

#endif /* L20ACTUATORTASK_H_ */
