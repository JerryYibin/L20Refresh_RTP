/*
 * CommunicationInterfaceCAN.h
 *
 *  Created on: May 12, 2022
 *      Author: JerryW.Wang
 */

#ifndef COMMUNICATIONINTERFACECAN_H_
#define COMMUNICATIONINTERFACECAN_H_

#include "CommunicationInterface.h"
#include "EmrsnDCan.h"
#include <semLib.h>

class CommunicationInterface_CAN: public CommunicationInterface
{
public:
	struct TX_MESSAGE
	{
		unsigned short DAC_Pressure;
	};
	struct RX_MESSAGE
	{
		unsigned short ADC_Height;
		unsigned short ADC_Width;
		unsigned short ADC_Pressure;
	};
	
	union DAC
	{
		unsigned short DAC_Cfg;
		struct DAC_REGISTER
		{
			unsigned short Reserved	: 2;
			unsigned short Value	: 10;
			unsigned short Speed	: 1;
			unsigned short Power	: 1;
			unsigned short Channel	: 2;
		}DAC_Reg;
	};
public:
	static CommunicationInterface_CAN* GetInstance();
	virtual ~CommunicationInterface_CAN();
	
	virtual int ErrorHandling() override;
	virtual int Sending(const void* data) override;
	virtual int Receiving(void* data) override;
private:
	enum COMMUNICATION_LINK
	{
		LINK_ERROR = -1,
		LINK_DCAN,
		LINK_DONE
	};
	EmrsnDCan* m_objImplementor;
	COMMUNICATION_LINK m_nStep;
	static CommunicationInterface_CAN* m_obj;
	static SEM_ID m_semaphoreMutex; /* ID of mutex semaphore*/
	
	CommunicationInterface_CAN();
	
	bool establishDCANSocket();
	void closeConnection();
	bool parseProtocolPackage(const char* buffer, const int buflen, RX_MESSAGE* ptrMsg);
	bool buildProtocolPackage(const TX_MESSAGE* ptrMsg, char* buffer, int* buflen);
};

#endif /* COMMUNICATIONINTERFACECAN_H_ */
