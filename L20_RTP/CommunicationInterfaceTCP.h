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
#include "ExternalData.h"
using namespace std;
#define	MAX_EXTERN_MSG_BUFF MAX_LEN- MIN_RX_MSG_LEN * sizeof(UINT8)
#define MAX_VECTOR_LENGTH 5000 /*Assume the average length of the recv message is 100 bytes,
								then max hole 50 records of message*/
#define MIN_RX_MSG_LEN 20 //Min length of the receive message is 20 Bytes
class CommunicationInterfaceTCP: public CommunicationInterface
{
public:
	virtual ~CommunicationInterfaceTCP();
	static CommunicationInterfaceTCP* GetInstance();
	
	virtual int ErrorHandling() override;
	virtual int Sending(const void* data) override;
	virtual int Receiving(void* data) override;
	virtual void Close() override;
	virtual int GetLinkStepIndex() override;
private:
	enum FUNCTIONID
	{
		WELDDATAOUTPUT = 1,
		POWERCURVEOUTPUT,
		HEIGHTCURVEOUTPUT,
		FREQUENCYCURVEOUTPUT,
		WELDDATAREQ,
		POWERCURVEREQ,
		HEIGHTCURVEREQ,
		FREQUENCYCURVEREQ
	};
	struct TIME
	{
		UINT32 			Year;
		UINT32 			Month;
		UINT32			Day;
		UINT32			Hour;
		UINT32			Minute;
		UINT32 			Second;
	};
	struct TX_MESSAGE
	{
		UINT8   DLEHEAD1 = 0x10;
		UINT8   DLEHEAD2 = 0x02;
		UINT8   STXSTHEAD1 = 0xAA;
		UINT8   STXSTHEAD2 = 0x55;
		UINT32  FUNID;
		UINT32  Length;
		const char*	_Buffer = nullptr;
		UINT32  Checksum;
		TIME Time;
		UINT8   DLETAIL1 = 0x55;
		UINT8   DLETAIL2 = 0xAA;
		UINT8   STXTAIL1 = 0x02;
		UINT8   STXTAIL2 = 0x10;	
	};
	struct RX_DATA_STRUCT
	{
		UINT8   DLEHEAD1 = 0x10;
		UINT8   DLEHEAD2 = 0x02;
		UINT8   STXSTHEAD1 = 0xAA;
		UINT8   STXSTHEAD2 = 0x55;
		UINT32  FUNID;
		UINT32  Length;
		char	Buffer[MAX_EXTERN_MSG_BUFF];
		UINT32  Checksum;
		UINT8   DLETAIL1 = 0x55;
		UINT8   DLETAIL2 = 0xAA;
		UINT8   STXTAIL1 = 0x02;
		UINT8   STXTAIL2 = 0x10;	
	};
	
	enum RX_IDX
	{
		HEAD1 = 0,
		HEAD2,
		HEAD3,
		HEAD4,
		FUNID1,
		FUNID2,
		FUNID3,
		FUNID4,
		LEN1,
		LEN2,
		LEN3,
		LEN4,
		CS1,
		CS2,
		CS3,
		CS4,
		TAIL1,
		TAIL2,
		TAIL3,
		TAIL4
	};
	
	EmrsnSocket* m_objImplementor;
	static CommunicationInterfaceTCP* _objComTCP;
	static SEM_ID TCPSendMutex;	/* ID of mutex semaphore*/
	int m_nDescriptor;
	COMMUNICATION_LINK m_nStep;
	vector<char> dataVector;
	CommunicationInterfaceTCP();
	bool parseProtocolPackage(MESSAGE* ptrMsg);
	bool buildProtocolPackage(const ExternalData::ETHMESSAGE* ptrMsg,	char* buffer, int* buflen);
	bool establishTCPIPSocket();
	void closeConnection();
	bool linkConfig();
	unsigned int checksumCalculate(const void *buf,const unsigned int len);
	bool findValidMessage(vector<char>&vtData, RX_DATA_STRUCT* ptrMsg );
	void GetTimeInfo(TIME*_timedata);
};

#endif /* COMMUNICATIONINTERFACETCP_H_ */
