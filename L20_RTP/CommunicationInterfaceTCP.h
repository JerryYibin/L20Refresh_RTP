/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef COMMUNICATIONINTERFACETCP_H_
#define COMMUNICATIONINTERFACETCP_H_
#include "CommunicationInterface.h"
#include "SSLWrapper.h"
#include "EmrsnSocket.h"
#include <semLib.h>
#include "CommonProperty.h"
#include <string>
#include <cstring>
using namespace std;
#define MAX_RX		128
#define	MAX_EXTERN_MSG_BUFF MAX_RX- 8 * sizeof(UINT8)
#define MSG_LEN 7
#define TCP_Test 0

class CommunicationInterfaceTCP: public CommunicationInterface
{
public:	
	struct TX_MESSAGE
	{
		UINT8   DLEHEAD = 0x2;
		UINT8   STXSTHEAD = 0x2;
		UINT32  FUNID;
		UINT32  Length;
		char	Buffer[MAX_EXTERN_MSG_BUFF];
		UINT32  Checksum;
		UINT8   DLETAIL = 0x10;
		UINT8   STXTAIL = 0x2;	
	};
		
public:
	virtual ~CommunicationInterfaceTCP();
	static CommunicationInterfaceTCP* GetInstance();
	
	virtual int ErrorHandling() override;
	virtual int Sending(const void* data) override;
	virtual int Receiving(void* data) override;
	virtual void Close() override;
	virtual int GetLinkStepIndex() override;
private:
	
	struct RX_DATA_STRUCT
	{
		UINT8   DLEHEAD = 0x2;
		UINT8   STXSTHEAD = 0x2;
		UINT16  FUNID;
		UINT16  Length;
		char	Buffer[MAX_EXTERN_MSG_BUFF];
		UINT16  Checksum;
		UINT8   DLETAIL = 0x10;
		UINT8   STXTAIL = 0x2;	
	};
	
	EmrsnSocket* m_objImplementor;
	static CommunicationInterfaceTCP* _objComTCP;
	static SEM_ID TCPSendMutex;	/* ID of mutex semaphore*/
	int m_nDescriptor;
	COMMUNICATION_LINK m_nStep;
	CommunicationInterfaceTCP();
	bool parseProtocolPackage(const char* buffer, const int buflen,	char* ptrMsg);
	bool buildProtocolPackage(const TX_MESSAGE* ptrMsg,	char* buffer, int* buflen);
	bool establishTCPIPSocket();
	void closeConnection();
	bool linkConfig();
};

#endif /* COMMUNICATIONINTERFACETCP_H_ */
