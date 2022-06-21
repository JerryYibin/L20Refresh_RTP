/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "SCTask.h"

/*
 *
 */
class UserInterface: public SCTask 
{
public:
	UserInterface();
	~UserInterface();
	
	static  void UserInterface_Task			(void);
protected:
	virtual void ProcessTaskMessage			(MESSAGE& message);	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID CTRL_MSG_Q_ID;
	MSG_Q_ID DATA_MSG_Q_ID_CTRL;
	MSG_Q_ID DATA_MSG_Q_ID_DATA;
	MSG_Q_ID DATA_MSG_Q_ID_REQ;
	MSG_Q_ID INTERFACE_MSG_Q_ID;
};

#endif /* USERINTERFACE_H_ */
