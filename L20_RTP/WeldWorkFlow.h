/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
**********************************************************************************************************/

#ifndef WELDWORKFLOW_H_
#define WELDWORKFLOW_H_

#include "WorkFlow.h"

class WeldWorkFlow: public WorkFlow {
public:
	enum WELD_WORKFLOW
	{
		SETUP,
		BATCH
	};
private:
	unsigned int				m_OperationMode;
public:
	WeldWorkFlow();
	virtual ~WeldWorkFlow();
public:
	virtual void 	InitProcess			(void) override;
	virtual void 	TriggerProcess		(void) override;
	virtual int		RunProcess			(void) override;
	virtual int 	UpdateResult		(void) override;
	virtual void	ResetProcess		(void) override;
	virtual void	SetOperationMode	(unsigned int operation);
	virtual int		GetOperationMode	();
};

#endif /* WELDWORKFLOW_H_ */
