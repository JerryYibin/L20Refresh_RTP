/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SOCKETRECEIVERHMI_H_
#define SOCKETRECEIVERHMI_H_

#include "SCTask.h"

class SocketReceiver_HMI: public SCTask 
{
public:
	SocketReceiver_HMI();
	~SocketReceiver_HMI();

	static  void 	Socket_HMI_Task			(void);
protected:
	virtual void	ProcessTaskMessage		(MESSAGE& message) override;
private:
  
	MSG_Q_ID		UI_MSG_Q_ID;
	MSG_Q_ID		INTERFACE_MSG_Q_ID;
	MSG_Q_ID		DATA_MSG_Q_ID;
};

#endif /* SOCKETRECEIVERHMI_H_ */
