/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "UserAuthority.h"
#include "UserPrivilege.h"
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
	int PermissionLevel[] = {0, 2, 3};
	int ScreenIndex[] = {1, 3, 9 , 13, 21, 24, 28, 33, 34, 37, 39, 40, 41};
	
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
	for(i = 0; i< (sizeof(PermissionLevel) / sizeof(int)); i++)
	{
		User userTmp;
		userTmp.m_Level = (PREMISSION_LEVEL)PermissionLevel[i];
		userTmp.m_Password = "000000";
		_UserProfilesSC->insert(pair<int, User>(PermissionLevel[i], userTmp));
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
int	UserAuthority::UpdateUserPrivileges(const vector<USER_PRIVILEGE>*)
{
	for(vector<USER_PRIVILEGE>::iterator iter = _UserPrivilegesUI->begin(); iter != _UserPrivilegesUI->end(); iter++)
	{
		auto mt = _UserPrivilegesSC->find(iter->EntryScreenIndex);
		if(mt != _UserPrivilegesSC->end())
		{
			mt->second = iter->PermissionLevel;
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
int UserAuthority::UpdateUserProfiles(const vector<USER_PROFILE>*)
{
	for(vector<USER_PROFILE>::iterator iter = _UserProfilesUI->begin(); iter != _UserProfilesUI->end(); iter++)
	{
		auto mt = _UserProfilesSC->find(iter->Level);
		if(mt != _UserProfilesSC->end())
		{
			mt->second.m_Level = iter->Level;
			mt->second.m_Password = string(iter->Password);
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
int	UserAuthority::UpdateUserPrivileges	(const map<int, int>*)
{
	_UserPrivilegesUI->clear();

	for(map<int, int>::iterator iter = _UserPrivilegesSC->begin(); iter != _UserPrivilegesSC->end(); iter++)
	{
		USER_PRIVILEGE userPrivilegeTmp;
		userPrivilegeTmp.EntryScreenIndex = iter->first;
		userPrivilegeTmp.PermissionLevel = iter->second;
		_UserPrivilegesUI->push_back(userPrivilegeTmp);
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
int UserAuthority::UpdateUserProfiles(const map<int, User>*)
{
	_UserProfilesUI->clear();
	
	for(map<int, User>::iterator iter = _UserProfilesSC->begin(); iter != _UserProfilesSC->end(); iter++)
	{
		USER_PROFILE userProfileTmp;
		userProfileTmp.Level = (iter->second).m_Level;
		memcpy(userProfileTmp.Password, (iter->second).m_Password.c_str(), sizeof(char) * (iter->second).m_Password.size());
		userProfileTmp.Privileges = (iter->second).m_Privileges; 
		_UserProfilesUI->push_back(userProfileTmp);
	}
	
	return 0;
}

