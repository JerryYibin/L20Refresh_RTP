/*
 * SocketReceiverCAN.cpp
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#include "SocketReceiverCAN.h"
#include "CommunicationInterfaceCAN.h"
unsigned int SocketReceiver_CAN::m_ADHeightAvg[COUNTER] = {0};
unsigned int SocketReceiver_CAN::m_ADWidthAvg[COUNTER] = {0};
unsigned int SocketReceiver_CAN::m_ADPressureAvg[COUNTER] = {0};
unsigned int SocketReceiver_CAN::m_Counter = 0;
SocketReceiver_CAN::SocketReceiver_CAN() {
	// TODO Auto-generated constructor stub

}

SocketReceiver_CAN::~SocketReceiver_CAN() {
	// TODO Auto-generated destructor stub
}

void SocketReceiver_CAN::ProcessTaskMessage(MESSAGE& message)
{

}

unsigned int SocketReceiver_CAN::getAverageFilter(const unsigned int* pointer)
{
	unsigned int lowest = 0, highest = 0, data = 0;
	for(int i = 0; i < COUNTER; i++)
	{
		if(i == 0)
		{
			lowest = pointer[i];
			highest = pointer[i];
		}
		else if(lowest > pointer[i])
		{
			lowest = pointer[i];
		}
		else if(highest < pointer[i])
		{
			highest = pointer[i];
		}
		else
		{
			
		}
		data += pointer[i];
	}
	data -= (lowest + highest);
	return data/(COUNTER - 2);
}

unsigned short SocketReceiver_CAN::GetHeight()
{
	return getAverageFilter(m_ADHeightAvg);
}

unsigned short SocketReceiver_CAN::GetWidth()
{
	return getAverageFilter(m_ADWidthAvg);
}

unsigned short SocketReceiver_CAN::GetPressure()
{
	return getAverageFilter(m_ADPressureAvg);
}

void SocketReceiver_CAN::Socket_CAN_Task(void)
{
	CommunicationInterface_CAN* Communication = CommunicationInterface_CAN::GetInstance();
	SocketReceiver_CAN* SocketReceiver = new(nothrow)SocketReceiver_CAN();
	CommunicationInterface_CAN::RX_MESSAGE   RecvBuffer;
	UINT32		events;
	if(nullptr != SocketReceiver)
	{
		while(SocketReceiver->bIsTaskRunStatus())
		{
			if(Communication->ErrorHandling() == OK)
			{
				if(Communication->Receiving(&RecvBuffer) == OK)
				{
//					ProcessBuffer.msgID = ClientBuffer.msgID;
//					memset(ProcessBuffer.Buffer, 0x00, sizeof(ProcessBuffer.Buffer));
//					memcpy(ProcessBuffer.Buffer, ClientBuffer.Buffer, ClientBuffer.msgLen);
//					SocketReceiver->ProcessTaskMessage(ProcessBuffer);
					m_ADHeightAvg[m_Counter] = RecvBuffer.ADC_Height;
					LOG("Height: 0x%x\n", RecvBuffer.ADC_Height);
					m_ADWidthAvg[m_Counter] = RecvBuffer.ADC_Width;
					LOG("Width: 0x%x\n", RecvBuffer.ADC_Width);
					m_ADPressureAvg[m_Counter] = RecvBuffer.ADC_Pressure;
					LOG("Pressure: 0x%x\n", RecvBuffer.ADC_Pressure);
					m_Counter++;
					if(m_Counter == COUNTER)
						m_Counter = 0;
				}
				else
				{
					LOGERR((char *)"CAN Socket_T: -------Read failed-----------", 0, 0, 0);
				}

			}
		}
		delete SocketReceiver;
	}
	else
	{
		LOGERR((char *)"CAN Socket_T: -------Memory allocation failed-----------", 0, 0, 0);
	}
	
	SocketReceiver = nullptr;
	taskSuspend(taskIdSelf());
}
