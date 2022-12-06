/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMUNICATIONINTERFACEDIG_H_
#define COMMUNICATIONINTERFACEDIG_H_

#include "EmrsnSocket.h"
#include "commons.h"
#include "CommunicationInterface.h"

#define DIG_SOCKET_PORT                        65100
#define DIG_SERVER_ADDRESS                    "150.150.150.50"

#define KEEPALIVEBUFFSIZE 					   24
#define RECORDBUFFSUZE 						   512

#define DIG_Test 0
#define MAX_RX		128
#define MSG_LEN 7

enum DIGDATATYPE
{
	DIGHEARTBEAT,
	CONFIG,
	DATA
};

class CommunicationInterfaceDIG: public CommunicationInterface
{
public:
	struct DIG_MESSAGE
	{
		UINT32	msgID;
		unsigned short  msgLen;
		char	Buffer[MAX_SIZE_OF_MSG_BUFF];
	};
	
	virtual ~CommunicationInterfaceDIG();
	static CommunicationInterfaceDIG*	GetInstance();

	virtual int ErrorHandling() override;
	virtual int Sending(const void* data) override;
	virtual int Receiving(void* data) override;
	virtual void Close() override;
	virtual int GetLinkStepIndex() override;
private:

	static CommunicationInterfaceDIG* _objComDIG;
	static SEM_ID DIGSendMutex;	/* ID of mutex semaphore*/
	
	EmrsnSocket* m_objImplementor;
	COMMUNICATION_LINK m_nStep;
	unsigned char m_TransBuffer_KeepAlive[KEEPALIVEBUFFSIZE];
	unsigned char m_TransBuffer_Config[RECORDBUFFSUZE];
	unsigned char m_TransBuffer_Data[RECORDBUFFSUZE];
	bool establishTCPIPSocket();
	void closeConnection();
	bool linkConfig();
	CommunicationInterfaceDIG();
	bool parseProtocolPackage(const char* buffer, const int buflen,	const char* ptrMsg);
	bool buildDIGSendingPackage(char *transBuffer, unsigned short* buflen, int recordType);
};

#endif /* COMMUNICATIONINTERFACEDIG_H_ */
