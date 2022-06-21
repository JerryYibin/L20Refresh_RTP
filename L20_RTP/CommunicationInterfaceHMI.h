/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMUNICATIONINTERFACEHMI_H_
#define COMMUNICATIONINTERFACEHMI_H_

#include "CommunicationInterface.h"
#include "SSLWrapper.h"
#include "EmrsnSocket.h"
#include "QuickCRC32Algorithm.h"
#include <semLib.h>
/*
 *
 */

class CommunicationInterface_HMI: public CommunicationInterface 
{
public:
	struct CLIENT_MESSAGE
	{
		UINT32	msgID;
		UINT32  msgLen;
		UINT32	rspCode;
		char	Buffer[MAX_SIZE_OF_MSG_BUFF];
		UINT32  crc32;
	};
public:
	static CommunicationInterface_HMI* GetInstance();
	virtual ~CommunicationInterface_HMI();

	virtual int ErrorHandling() override;
	virtual int Sending(const void* data) override;
	virtual int Receiving(void* data) override;
private:
	enum COMMUNICATION_LINK
	{
		LINK_ERROR = -1,
		LINK_TCPIP,
		LINK_SSL,
		LINK_DONE
	};
	
	int m_nDescriptor;
	EmrsnSocket* m_objImplementor;
	SSLWrapper* m_objSSL;
	COMMUNICATION_LINK m_nStep;
	static CommunicationInterface_HMI* m_obj;
	static SEM_ID m_semaphoreMutex;	/* ID of mutex semaphore*/

	CommunicationInterface_HMI();
	bool establishTCPIPSocket();
	bool establishSSLServer();
	void closeConnection();
	bool parseProtocolPackage(const char* 			buffer, const int 	buflen,	CLIENT_MESSAGE* ptrMsg);
	bool buildProtocolPackage(const CLIENT_MESSAGE* ptrMsg,	char* 		buffer, int* 			buflen);
};

#endif /* COMMUNICATIONINTERFACEHMI_H_ */
