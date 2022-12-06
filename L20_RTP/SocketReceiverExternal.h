/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef SOCKETRECEIVEREXTERNAL_H_
#define SOCKETRECEIVEREXTERNAL_H_


#include "SCTask.h"
class SocketReceiver_External: public SCTask 
{
public:
	SocketReceiver_External();
	~SocketReceiver_External();

	static  void 	Socket_External_Task			(void);
protected:
	virtual void	ProcessTaskMessage		        (MESSAGE& message) override;

};


#endif /* SOCKETRECEIVEREXTERNAL_H_ */
