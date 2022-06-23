/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The EmrsnDCan is the Ti' DCan communication operand code that has been encapsulated 
 by WindRiver and implemented by Socket.    
 
***************************************************************************/

#ifndef EMRSNDCAN_H_
#define EMRSNDCAN_H_

#include "CommunicationImplementer.h"
#define LOOPBACK 	0
#define MAX_BYTES	8 
class EmrsnDCan: public CommunicationImplementer
{
private:
	int dwsock;
public:
	EmrsnDCan();
	virtual ~EmrsnDCan();
	
public:
	int Create();
	
	int Bind(int port);
	
	int Close();
	
	int Send(const char* buff, const unsigned int bufflen);
	
	int Read(char* buff, const unsigned int bufflen);
};

#endif /* EMRSNDCAN_H_ */
