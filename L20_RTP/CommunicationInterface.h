/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMUNICATIONINTERFACE_H_
#define COMMUNICATIONINTERFACE_H_

#include "CommunicationImplementer.h"
#include "Common.h"

class CommunicationInterface
{
protected:
	enum COMMUNICATION_TYPE
	{
		HMI_SOCKET_CONNECTION,
		ACT_SOCKET_CONNECTION,
		EXT_SOCKET_CONNECTION,
		DIG_SOCKET_CONNECTION,
		EIP_FIELDBUS_CONNECTION,
		SENSOR_CAN_CONNECTION
	};
protected:
	CommunicationImplementer* GetCommunicationImp();
public:
	CommunicationInterface(int type);
	virtual ~CommunicationInterface();
public:
	virtual int ErrorHandling() = 0;
	virtual int Sending(const void* data) = 0;
	virtual int Receiving(void* data) = 0;
private:
	COMMUNICATION_TYPE m_typeCommunication;
	CommunicationImplementer* m_objImplementor;
};

#endif /* COMMUNICATIONINTERFACE_H_ */
