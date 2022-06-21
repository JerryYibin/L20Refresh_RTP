/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef DATAINTERFACE_H_
#define DATAINTERFACE_H_

#include "SCTask.h"

class DataInterface: public SCTask 
{
public:
	DataInterface();
	~DataInterface();
	
	static	void 	DataInterface_Task					(void);
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;
private:
	//Just list out all the external Message ID, don't include itself.
    MSG_Q_ID 			DATA_MSG_Q_ID_REQUEST;
};

#endif /* DATAINTERFACE_H_ */
