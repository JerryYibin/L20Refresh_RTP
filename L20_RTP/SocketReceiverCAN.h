/*
 * SocketReceiverCAN.h
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#ifndef SOCKETRECEIVERCAN_H_
#define SOCKETRECEIVERCAN_H_

#include "SCTask.h"
#define COUNTER	10
class SocketReceiver_CAN: public SCTask
{
public:
	SocketReceiver_CAN();
	virtual ~SocketReceiver_CAN();
	
	static  void 	Socket_CAN_Task			(void);
	static unsigned short GetHeight			();
	static unsigned short GetWidth			();
	static unsigned short GetPressure		();
protected:
	virtual void	ProcessTaskMessage		(MESSAGE& message) override;
private:
	enum ANALOG_CHANNEL
	{
		HEIGHT_OUT		= 0,
		WIDTH_OUT,
		PRESSURE_OUT
	};
	static unsigned int 	m_ADHeightAvg		[COUNTER];
	static unsigned int 	m_ADWidthAvg		[COUNTER];
	static unsigned int 	m_ADPressureAvg		[COUNTER];
	static unsigned int		m_Counter;
	static unsigned int		getAverageFilter(const unsigned int* pointer);
};

#endif /* SOCKETRECEIVERCAN_H_ */
