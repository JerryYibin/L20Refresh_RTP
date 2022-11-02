/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef P1ACTUATORTASK_H_
#define P1ACTUATORTASK_H_

#include "ActuatorTask.h"

class P1ActuatorTask: public ActuatorTask 
{
public:
	P1ActuatorTask();
	virtual ~P1ActuatorTask();
	virtual void		 PDOUploadRequest			() override;
	virtual void		 PDODownloadRequest			() override;
	virtual bool		 IsMoving					() override;
	virtual unsigned int GetMaxSpeed				() override;
	virtual void		 InitHeightSystem			() override;
	virtual void 		 ScanInputs					() override;
protected:
	virtual	void		 DoAuxMotion				(int motion) override;
};

#endif /* P1ACTUATORTASK_H_ */
