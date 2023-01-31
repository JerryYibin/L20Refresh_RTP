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
class ExternalManager : public SCTask 
{

public:

	enum MESSAGE_IDENTIFY
	{
		TO_EXT_TASK_AFTER_WELD_REQ,
		TO_EXT_TASK_WELD_DATA_REQ,
		TO_EXT_TASK_POWER_CURVE_REQ,
		TO_EXT_TASK_HEIGHT_CURVE_REQ,
		TO_EXT_TASK_FREQUENCY_CURVE_REQ,
		TO_EXT_TASK_RECALL_RECIPE_REQ,
		TO_EXT_TASK_RECALL_RECIPE_RESPONSE,
	};
	
	
	virtual ~ExternalManager();
	virtual int Update();
	static  void 	External_Manager_Task			(void);
	static ExternalManager* GetInstance();
	CommunicationInterface* GetCommunicateObj();
	
protected:
	virtual void 	ProcessTaskMessage					(MESSAGE& message) override;	
private:

	ExternalManager();
	static ExternalManager* ExternalEthobj;
	CommunicationInterface* m_ptrCom;
	ETHERNET 	m_PrevEthernetConfig;
	ExternalData* m_ptrData;
	
	MSG_Q_ID		UI_MSG_Q_ID;
	MSG_Q_ID		DATA_MSG_Q_ID_REQ;
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
