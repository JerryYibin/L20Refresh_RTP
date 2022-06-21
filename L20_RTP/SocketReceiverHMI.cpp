/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Socket Receiver task for the communication with UI controller
Changed the C code "Socket_HMI_Task" to c++ static function. 
It's a good change for Socket_HMI_Task so Socket_HMI_Task can access all the source of
SocketReceiver_HMI owned using the class object pointer. 
 
**********************************************************************************************************/


#include "SocketReceiverHMI.h"
#include "CommunicationInterfaceHMI.h"
#include "HMI_LRC_comms.h"
extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of SocketReceiver.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SocketReceiver_HMI::SocketReceiver_HMI()
{
	// Load the data message Q name
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
	INTERFACE_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T]);
	
	// Load the data message Q name
	string Data_Task(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	DATA_MSG_Q_ID = CP->getMsgQId(Data_Task + "/Control");
}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SocketReceiver_HMI::~SocketReceiver_HMI()
{
	// TODO Auto-generated destructor stub
}

/*************************************************************************//**
 * \brief   - Process the received message from client socket
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void SocketReceiver_HMI::ProcessTaskMessage(MESSAGE& message)
{
	switch(message.msgID)
	{
	case RES_HEART_BEAT_IND:
		sendHeartbeatResponse();
		break;
	default:
		LOGERR((char *)"SocketReceiver_HMI_T : --------Unknown Message ID----------- : ", message.msgID, 0, 0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for heart beat request with parts  per minute
* 			 counter. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver_HMI::sendHeartbeatResponse()
{
	CommunicationInterface_HMI::CLIENT_MESSAGE sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	
	sendMsg.msgID	= RES_HEART_BEAT_IND;
	sendMsg.msgLen  = 0;
	sendMsg.rspCode = 0;
	
	HEARTBEAT stHeartbeat;
	stHeartbeat.AlarmCode = 0;
	stHeartbeat.Amplitude = CommonProperty::m_WeldResults.Get(WeldResults::ACTUAL_AMPLITUDE);
	stHeartbeat.CycleCounter = CommonProperty::m_WeldResults.Get(WeldResults::CYCLE_COUNTER);
	stHeartbeat.PeakPower = CommonProperty::m_WeldResults.Get(WeldResults::PEAK_POWER);
	stHeartbeat.PostHeight = CommonProperty::m_WeldResults.Get(WeldResults::POST_HEIGHT);
	stHeartbeat.PreHeight = CommonProperty::m_WeldResults.Get(WeldResults::PREHEIGHT);
	stHeartbeat.RecipeNumber = CommonProperty::m_WeldResults.Get(WeldResults::RECIPE_NUM);
	stHeartbeat.TotalEnergy = CommonProperty::m_WeldResults.Get(WeldResults::TOTAL_ENERGY);
	stHeartbeat.TriggerPress = CommonProperty::m_WeldResults.Get(WeldResults::TRIGGER_PRESSURE);
	stHeartbeat.WeldPress = CommonProperty::m_WeldResults.Get(WeldResults::WELD_PRESSURE);
	stHeartbeat.WeldTime = CommonProperty::m_WeldResults.Get(WeldResults::WELD_TIME);
	
	memcpy(sendMsg.Buffer, &stHeartbeat, sizeof(HEARTBEAT));
	sendMsg.msgLen = sizeof(HEARTBEAT);
	
	CommunicationInterface_HMI::GetInstance()->Sending(&sendMsg);
	
}

/**************************************************************************//**
* 
* \brief   - Socket receiver task entry point
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver_HMI::Socket_HMI_Task(void)
{
	CommunicationInterface_HMI* Communication = CommunicationInterface_HMI::GetInstance();
	SocketReceiver_HMI* SocketReceiver = new(nothrow)SocketReceiver_HMI();
	CommunicationInterface_HMI::CLIENT_MESSAGE		ClientBuffer;
	MESSAGE				ProcessBuffer;
	if(nullptr != SocketReceiver)
	{
		while(SocketReceiver->bIsTaskRunStatus())
		{
			if(Communication->ErrorHandling() == OK)
			{
				if(Communication->Receiving(&ClientBuffer) == OK)
				{
					ProcessBuffer.msgID = ClientBuffer.msgID;
					memset(ProcessBuffer.Buffer, 0x00, sizeof(ProcessBuffer.Buffer));
					memcpy(ProcessBuffer.Buffer, ClientBuffer.Buffer, ClientBuffer.msgLen);
					SocketReceiver->ProcessTaskMessage(ProcessBuffer);
				}
			}
		}
		delete SocketReceiver;
	}
	else
	{
		LOGERR((char *)"HMISocket_T: -------Memory allocation failed-----------", 0, 0, 0);
	}
	
	SocketReceiver = nullptr;
	taskSuspend(taskIdSelf());
}
