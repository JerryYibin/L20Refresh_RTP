/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef ExternalManager_H_
#define ExternalManager_H_
#include "Common.h"
#include "SCTask.h"
#include "CommunicationInterface.h"
#include "Connectivity.h"
#include "ExternalData.h"
#define SAMPLE500MSEC 500
class ExternalManager : public SCTask 
{

public:
	enum CMD_ID
	{
		AFTERWELD,
		REPLYWELDDATA,
		REPLYPOWERCURVE,
		REPLYHEIGHTCURVE,
		REPLYFREQUENCYCURVE,
		SENDHEARTBEAT
	};
	virtual ~ExternalManager();
	virtual int Update();
	int Send(int cmd);
	static ExternalManager* GetInstance();
	CommunicationInterface* GetCommunicateObj();
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;	
	
private:

	ExternalManager();
	static ExternalManager* ExternalEthobj;
	CommunicationInterface* m_ptrCom;
	ETHERNET 	m_PrevEthernetConfig;
	static unsigned int Tick1MS;
	ExternalData* m_ptrData;
	
	int closeSocketEvent();
	int linkSocketEvent();
	int readSocketEvent();
	bool requireUpdateSocket ();
	void refreshExternalEthobj();
	int senddata(ExternalData::ETHMESSAGE* message);
	ExternalManager (const ExternalManager&) 			= delete;
	ExternalManager& operator=(const ExternalManager&) = delete;
};

#endif /* ExternalManager_H_ */
