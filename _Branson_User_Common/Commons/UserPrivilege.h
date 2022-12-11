/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------

User Authority definition is for L20 Refresh UI Controller only.
 
***************************************************************************/


#ifndef USERPRIVILEGE_H_
#define USERPRIVILEGE_H_
#define PASSWORD_LEN	20
#define USERNAME_LEN	20
struct USER_PRIVILEGE
{
	int		EntryScreenIndex;
	int		PermissionLevel;
	
};

enum PREMISSION_LEVEL
{
	EXECUTIVE = 0, //It should be Administrator for L20
	SUPERVISOR,
	TECHNICIAN,
	OPERATOR,	   //It should be Open for L20
};

enum PRIVILEGES
{
	//TODO Reserved for P1
};

struct USER_PROFILE
{
	PREMISSION_LEVEL		Level;
	char					Password[PASSWORD_LEN];
	char					UserName[USERNAME_LEN];
	PRIVILEGES				Privileges;	
};

enum PASSWORD_CHECK_STATUS
{
	PASSWORD_CHECK_FAIL = -1,
	PASSWORD_CHECK_SUCCESS
};

struct USER_PAGE_PASSWORD
{
	int  PageIndex;
	char Password[PASSWORD_LEN];
};

#endif /* USERPRIVILEGE_H_ */
