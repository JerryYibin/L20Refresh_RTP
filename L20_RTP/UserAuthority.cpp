/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "UserAuthority.h"
#include "UserPrivilege.h"
#include "ScreenIndexUI.h"
#include "Logger.h"
//Key is EntryScreenIndex, Value is premission Level;
map<int, int>* UserAuthority::_UserPrivilegesSC 			= nullptr;
//Key is PermissionLevel, Value is User instance;
map<int, User>* UserAuthority::_UserProfilesSC 				= nullptr;
vector<USER_PRIVILEGE>* UserAuthority::_UserPrivilegesUI 	= nullptr;
vector<USER_PROFILE>* UserAuthority::_UserProfilesUI 		= nullptr;
UserAuthority* UserAuthority::_UserAuthorityObj				= nullptr;
/**************************************************************************//**
* \brief   - Constructor - Initialize _UserPrivilegesSC, _UserProfilesSC, _UserPrivilegesUI and _UserProfilesUI.
* 			 The data should be from database in the future.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
UserAuthority::UserAuthority() 
{
	int i = 0;
	PREMISSION_LEVEL levels[] = {EXECUTIVE, TECHNICIAN};
	int ScreenIndex[] = {MENU_OPTIONS_SCREEN, RECIPE_LIBRARY_SCREEN, MAINTAIN_SCREEN, 
			SYSTEM_CONFIG_SCREEN, ADMINISTRATOR_SCREEN, DATA_LOG_SCREEN, 
			CONNECTIVITY_SCREEN, CALIBRATION_SCREEN, SYSTEM_INFORMATION_SCREEN};
	
	_UserPrivilegesSC 	= new map<int, int>();
	_UserProfilesSC		= new map<int, User>();
	_UserPrivilegesUI	= new vector<USER_PRIVILEGE>();
	_UserProfilesUI		= new vector<USER_PROFILE>();
	//Init _UserPrivilegesSC
	for(i = 0; i < (sizeof(ScreenIndex) / sizeof(int)); i++)
	{
		_UserPrivilegesSC->insert(pair<int, int>(ScreenIndex[i], 2));
	}
	//Init _UserProfilesSC
	for(i = 0; i< (sizeof(levels) / sizeof(int)); i++)
	{
		User tmpUser;
		tmpUser.m_Level = levels[i];
		tmpUser.m_Password = "000000";
		_UserProfilesSC->insert(pair<int, User>(levels[i], tmpUser));
	}
}

/**************************************************************************//**
* 
* \brief   - Destructor. destruct _UserPrivilegesSC, _UserProfilesSC, _UserPrivilegesUI and _UserProfilesUI
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
UserAuthority::~UserAuthority() 
{
	_UserPrivilegesSC->clear();
	delete _UserPrivilegesSC;

	_UserProfilesSC->clear();
	delete _UserProfilesSC;
	
	_UserPrivilegesUI->clear();
	delete _UserPrivilegesUI;
	
	_UserProfilesUI->clear();
	delete _UserProfilesUI;
	
	delete _UserAuthorityObj;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - UserAuthority* Object 
*
******************************************************************************/
UserAuthority*	UserAuthority::GetInstance()
{
	return (_UserAuthorityObj != nullptr) ? _UserAuthorityObj : (_UserAuthorityObj = new UserAuthority());
}

/**************************************************************************//**
* \brief   - Verify the password entered on the screen.
*
* \param   - const string passcode
* 			 const int entryScreenIndex
*
* \return  - int
*
******************************************************************************/
int UserAuthority::CheckScreenAccessAuthority(const string passcode, const int entryScreenIndex) const
{
	int iCheckStatus = PASSWORD_CHECK_FAIL;
	map<int, int>::iterator privilegesIter;
	map<int, User>::iterator userIter;
	privilegesIter = _UserPrivilegesSC->find(entryScreenIndex);
	if(privilegesIter != _UserPrivilegesSC->end() )
	{
		int iPrivilegesLevel = privilegesIter->second;
		for(map<int, User>::iterator userIter = _UserProfilesSC->begin(); userIter != _UserProfilesSC->end(); userIter++)
		{
			int iPermissionLevel = userIter->second.m_Level;
			string strPasswordTmp = userIter->second.m_Password;
			if(passcode == strPasswordTmp && iPermissionLevel <= iPrivilegesLevel)
			{
				iCheckStatus = PASSWORD_CHECK_SUCCESS;
			}
		}
	}

	return iCheckStatus;
}

/**************************************************************************//**
* \brief   - Update user page permissions.
*
* \param   - const vector<USER_PRIVILEGE>*
* 	
*
* \return  - int
*
******************************************************************************/
int	UserAuthority::UpdateUserPrivileges(const vector<USER_PRIVILEGE>* _PrivilegeList)
{
	for(auto iter = _PrivilegeList->begin(); iter != _PrivilegeList->end(); iter++)
	{
		auto mt = _UserPrivilegesSC->find(iter->EntryScreenIndex);
		if(mt != _UserPrivilegesSC->end())
		{
			mt->second = iter->PermissionLevel;
			LOG("Screen index = %d PermissionLevel = %d\n", iter->EntryScreenIndex, iter->PermissionLevel);
		}
	}
	return 0;
}

/**************************************************************************//**
* \brief   - Get user page permissions.
*
* \param   - const vector<USER_PRIVILEGE>*
* 	
*
* \return  - int 
*
******************************************************************************/
int UserAuthority::UpdateUserProfiles(const vector<USER_PROFILE>* _UserList)
{
	for(auto iter = _UserList->begin(); iter != _UserList->end(); iter++)
	{
		auto mt = _UserProfilesSC->find(iter->Level);
		if(mt != _UserProfilesSC->end())
		{
			mt->second.m_Level = iter->Level;
			mt->second.m_Password = string(iter->Password);
			LOG("level = %d password = %s\n", iter->Level, iter->Password);
		}
	}
	
	return 0;
}

/**************************************************************************//**
* \brief   - Update user name and password.
*
* \param   - const map<int, int>*
* 	
*
* \return  - int 
*
******************************************************************************/
int	UserAuthority::UpdateUserPrivileges	(const map<int, int>* _PrivilegeMap)
{
	_UserPrivilegesUI->clear();
	for(auto iter = _PrivilegeMap->begin(); iter != _PrivilegeMap->end(); iter++)
	{
		USER_PRIVILEGE tmpPrivilege;
		tmpPrivilege.EntryScreenIndex = iter->first;
		tmpPrivilege.PermissionLevel = iter->second;
		_UserPrivilegesUI->push_back(tmpPrivilege);
	}
	return 0;
}

/**************************************************************************//**
* \brief   - Get user name and password.
*
* \param   - const map<int, User>*
* 	
*
* \return  - int
*
******************************************************************************/
int UserAuthority::UpdateUserProfiles(const map<int, User>* _UserMap)
{
	_UserProfilesUI->clear();
	for(auto iter = _UserMap->begin(); iter != _UserMap->end(); iter++)
	{
		USER_PROFILE tmpUser;
		tmpUser.Level = (iter->second).m_Level;
		memcpy(tmpUser.Password, (iter->second).m_Password.c_str(), PASSWORD_LEN);
		tmpUser.Privileges = (iter->second).m_Privileges; 
		_UserProfilesUI->push_back(tmpUser);
	}
	return 0;
}

