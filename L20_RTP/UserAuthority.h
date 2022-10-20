/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef USERAUTHORITY_H_
#define USERAUTHORITY_H_
#include "UserPrivilege.h"
#include <map>
#include <vector>
using namespace std;
class UserAuthority 
{
public:
	//Key is EntryScreenIndex, Value is Permission level;
	static map<int, int>* _UserPrivilegesSC;
	//Key is PermissionLevel, Value is passcode;
	static map<int, string>* _UserProfilesSC;
	
	static vector<USER_PRIVILEGE>* _UserPrivilegesUI;
	static vector<USER_PROFILE>* _UserProfilesUI;
public:
	UserAuthority();
	virtual ~UserAuthority();
	int CheckScreenAccessAuthority(const string passcode, const int entryScreenIndex);
	int	UpdateUserPrivileges(const vector<USER_PRIVILEGE>*);
	int UpdateUserProfiles(const vector<USER_PROFILE>*);
	int	UpdateUserPrivileges(const map<int, int>*);
	int UpdateUserProfiles(const map<int, string>*);
};

#endif /* USERAUTHORITY_H_ */
