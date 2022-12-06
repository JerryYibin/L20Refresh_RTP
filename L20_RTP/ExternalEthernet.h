/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef EXTERNALETHERNET_H_
#define EXTERNALETHERNET_H_
#include "Common.h"
#include "SCTask.h"
#include "CommunicationInterface.h"
#include "Connectivity.h"

#define SAMPLE500MSEC 500
class ExternalEthernet : public SCTask 
{

public:
	virtual ~ExternalEthernet();
	virtual int Update();
	static ExternalEthernet* GetInstance();
	CommunicationInterface* GetCommunicateObj();
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;	
	
private:

	ExternalEthernet();
	static ExternalEthernet* ExternalEthobj;
	CommunicationInterface* m_ptrCom;
	ETHERNET 	m_PrevEthernetConfig;
	static unsigned int Tick1MS;
	
	int closeSocketEvent();
	int linkSocketEvent();
	int readSocketEvent();
	bool requireUpdateSocket ();
	void refreshExternalEthobj();
	
	ExternalEthernet (const ExternalEthernet&) 			= delete;
	ExternalEthernet& operator=(const ExternalEthernet&) = delete;
};






#endif /* EXTERNALETHERNET_H_ */
