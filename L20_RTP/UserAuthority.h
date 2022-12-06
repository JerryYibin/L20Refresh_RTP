/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef USERAUTHORITY_H_
#define USERAUTHORITY_H_
#include <map>
#include <vector>
#include "User.h"
using namespace std;

class UserAuthority 
{
	
public:
	//Key is EntryScreenIndex, Value is Permission level;
	static map<int, int>* 			_UserPrivilegesSC;
	//Key is PermissionLevel, Value is User instance;
	static map<int, User*>*	 		_UserProfilesSC;
	
	static vector<USER_PRIVILEGE>* 	_UserPrivilegesUI;
	static vector<USER_PROFILE>* 	_UserProfilesUI;
public:
	static UserAuthority*	GetInstance					();
	virtual					 ~UserAuthority				();
	int 					CheckScreenAccessAuthority	(const string passcode, const int entryScreenIndex) const;
	int						UpdateUserPrivileges		(const vector<USER_PRIVILEGE>*);
	int 					UpdateUserProfiles			(const vector<USER_PROFILE>*);
	int						UpdateUserPrivileges		(const map<int, int>*);
	int 					UpdateUserProfiles			(const map<int, User*>*);
	
							UserAuthority				(const UserAuthority&) 	= delete;
							UserAuthority& operator=	(const UserAuthority&) 	= delete;
private:
							UserAuthority				();
private:
	static UserAuthority* 			_UserAuthorityObj;
	User					m_Admin;
	User					m_Techician;
	User					m_Open;
};

#endif /* USERAUTHORITY_H_ */
