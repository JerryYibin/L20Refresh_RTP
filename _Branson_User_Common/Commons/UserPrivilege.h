/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef USERPRIVILEGE_H_
#define USERPRIVILEGE_H_
#define PASSCODE_LEN	6
struct USER_PRIVILEGE
{
	char 	Passcode[PASSCODE_LEN];
	int		EntryScreenIndex;
};

struct USER_PROFILE
{
	int		PermissionLevel;
	char	Passcode[PASSCODE_LEN];
};

#endif /* USERPRIVILEGE_H_ */
