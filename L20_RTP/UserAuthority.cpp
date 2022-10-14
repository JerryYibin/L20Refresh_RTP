/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "UserAuthority.h"
//Key is EntryScreenIndex, Value is passcode;
map<int, string>* UserAuthority::_UserPrivilegesSC 			= nullptr;
//Key is PermissionLevel, Value is passcode;
map<int, string>* UserAuthority::_UserProfilesSC 			= nullptr;
vector<USER_PRIVILEGE>* UserAuthority::_UserPrivilegesUI 	= nullptr;
vector<USER_PROFILE>* UserAuthority::_UserProfilesUI 		= nullptr;

UserAuthority::UserAuthority() 
{
	_UserPrivilegesSC 	= new map<int, string>();
	_UserProfilesSC		= new map<int, string>();
	_UserPrivilegesUI	= new vector<USER_PRIVILEGE>();
	_UserProfilesUI		= new vector<USER_PROFILE>();

}

UserAuthority::~UserAuthority() 
{
	delete _UserPrivilegesSC;
	delete _UserProfilesSC;
	delete _UserPrivilegesUI;
	delete _UserProfilesUI;
}

