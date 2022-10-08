/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef DSPTASK_H_
#define DSPTASK_H_

#include "PowerSupplyTask.h"

class DSPTask: public PowerSupplyTask 
{
public:
	DSPTask();
	virtual				~DSPTask					();
	virtual void		PDOUploadRequest			() override;
	virtual void		PDODownloadRequest			() override;
	virtual void		CalculateStartFrequency		() override;
};

#endif /* DSPTASK_H_ */
