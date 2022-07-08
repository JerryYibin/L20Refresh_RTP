/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef AUPSTASK_H_
#define AUPSTASK_H_

#include "PowerSupplyTask.h"

class AUPSTask: public PowerSupplyTask 
{
public:
	AUPSTask();
	virtual				~AUPSTask					();
	virtual void		PDOUploadRequest			() override;
	virtual void		PDODownloadRequest			() override;
};

#endif /* AUPSTASK_H_ */
