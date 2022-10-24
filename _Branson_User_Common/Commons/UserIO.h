/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define the system
information.

***************************************************************************/

#ifndef USER_IO_COMMON_H_
#define USER_IO_COMMON_H_
#include "Utils.h"
enum USER_IO_BIT
{
	RESET = 0, 	//bit 0
	ESTOP, 		//bit 1
	POWER_OFF,	//bit 2
	PB1,		//bit 3
	PB2,		//bit 4
	ACT_STATUS,	//bit 5
	LOST_24V,	//bit 6
	BITS_SIZE
};


#endif
