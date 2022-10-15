/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef SYSTEMTYPE_H_
#define SYSTEMTYPE_H_

#define V_PTR_SIZE 4
typedef enum
{
	GSXP1,
	GSXE1,
	L20,
	INVALID_SYSTEM
}EN_SYSTEM_TYPE;

EN_SYSTEM_TYPE GetSystemType();

#endif
