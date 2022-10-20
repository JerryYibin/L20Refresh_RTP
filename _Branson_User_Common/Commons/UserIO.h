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

struct USER_IO_BIT
{
	unsigned int	Reset	: 1; //bit 0
	unsigned int	PB1		: 1; //bit 1
	unsigned int	PB2		: 1; //bit 2
};
union USER_IO
{
	unsigned int UserIOs;
	USER_IO_BIT  IOBits;
};

#endif
