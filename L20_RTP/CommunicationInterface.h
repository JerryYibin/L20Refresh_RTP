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
public:
	enum COMMUNICATION_LINK
	{
		LINK_ERROR = -1,
		LINK_CONFIG,
		LINK_TCPIP,
		LINK_DONE,
	};
protected:
	enum COMMUNICATION_TYPE
	{
		HMI_SOCKET_CONNECTION,
		ACT_SOCKET_CONNECTION,
		TCP_SOCKET_CONNECTION,
		DIG_SOCKET_CONNECTION,
		EIP_FIELDBUS_CONNECTION,
		SENSOR_CAN_CONNECTION
	};
protected:
	CommunicationImplementer* GetCommunicationImp();
public:
	CommunicationInterface(int type);
	virtual ~CommunicationInterface();
	virtual int ErrorHandling() = 0;
	virtual int Sending(const void* data) = 0;
	virtual int Receiving(void* data) = 0;
	virtual void Close();
	virtual int GetLinkStepIndex();
private:
	COMMUNICATION_TYPE m_typeCommunication;
	CommunicationImplementer* m_objImplementor;
};

#endif /* COMMUNICATIONINTERFACE_H_ */
