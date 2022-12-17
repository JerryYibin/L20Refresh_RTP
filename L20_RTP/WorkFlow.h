/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

	The Work Flow shall be a element in Control Task.
	The specific work flow shall be instantiated following the operation mode,
	such as height calibration, teach mode, sequence, production batch size, etc. 
 
***************************************************************************/

#ifndef WORKFLOW_H_
#define WORKFLOW_H_

class WorkFlow {
public:
	enum STATE
	{
		INVALID = -1,
		ONGOING = 0,
		FINISH
	};
public:
	WorkFlow(){};
	virtual ~WorkFlow(){};
public:
	virtual void 	InitProcess		(void) = 0;
	virtual void 	TriggerProcess	(void) = 0;
	virtual int		RunProcess		(void) = 0;
	virtual int 	UpdateResult	(void) = 0;
	virtual void	ResetProcess	(void) = 0;
};

#endif /* WORKFLOW_H_ */
