/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The EmrsnDCan is the Ti' DCan communication operand code that has been encapsulated 
 by WindRiver and implemented by Socket.    
 
***************************************************************************/

#include <sockLib.h>
#include <stdio.h>
#include <string.h>
#include "EmrsnDCan.h"
extern "C"
{
	#include <socketCAN.h>
	#include <canDevLib.h>
}

/**************************************************************************//**
* \brief    - EmrsnSocket Class constructor
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
EmrsnDCan::EmrsnDCan() {
	int dwsock = -1;

}

/**************************************************************************//**
* 
* \brief   - EmrsnDCan class destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
EmrsnDCan::~EmrsnDCan() 
{
	
	Close();
}


int EmrsnDCan::Create()
{
	dwsock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(dwsock == ERROR)
		return ERROR;
	return OK;
}

int EmrsnDCan::Bind(int port)
{
	if(port < 1)
		return ERROR;
	struct sockaddr_can address;
	
	address.can_ifindex = port;
	address.can_family	= PF_CAN;
	if(bind(dwsock, (struct sockaddr*)&address, sizeof(sockaddr_can)) == ERROR)
		return ERROR;
	int loop = LOOPBACK;
	if(setsockopt(dwsock, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loop, sizeof(int)) == ERROR)
		return ERROR;
	return OK;
}

int EmrsnDCan::Send(const char* buff, const unsigned int bufflen)
{
	int nbytes;
	int len = 0;
	struct can_frame	frame;
	frame.can_id = 0x123;
	if(bufflen < MAX_BYTES)
		len = bufflen;
	else
		len = MAX_BYTES;
	frame.can_dlc = len;
	memset(frame.data, 0, MAX_BYTES);
	memcpy(frame.data, buff, len);
	if(len > 2)
		printf("#########################Len ################################: %d", len);
	nbytes = send(dwsock, &frame, sizeof(struct can_frame), 0);
	if(nbytes != sizeof(struct can_frame))
		return ERROR;
	return nbytes;
}

int EmrsnDCan::Read(char* buff, const unsigned int bufflen)
{
	int nbytes;
	struct can_frame	frame;
	frame.can_dlc = 64;
	frame.can_id  = 0x124;
	int len = 0;
	nbytes = recv(dwsock, &frame, sizeof(struct can_frame), 0);
	if(nbytes != sizeof(struct can_frame))
		return ERROR;
	if(!(frame.can_id & CAN_RTR_FLAG))
	{
		if(bufflen > frame.can_dlc)
			len = frame.can_dlc;
		else
			len = bufflen;
		memcpy(buff, frame.data, len);
	}
	return len;
}

int EmrsnDCan::Close()
{
	return close(dwsock);
}
