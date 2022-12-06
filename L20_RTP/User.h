/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 User Object definition. 
 
**********************************************************************************************************/

#ifndef USER_H_
#define USER_H_
#include <iostream>
#include <string.h>
#include "UserPrivilege.h"
using namespace std;
class User 
{
public:
	string 				m_UserName;
	string 				m_Password;
	PREMISSION_LEVEL	m_Level;
	PRIVILEGES			m_Privileges;
	User();
	virtual ~User();
};

#endif /* USER_H_ */
