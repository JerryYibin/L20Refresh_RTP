/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "UserAuthority.h"
//Key is EntryScreenIndex, Value is premission Level;
map<int, int>* UserAuthority::_UserPrivilegesSC 			= nullptr;
//Key is PermissionLevel, Value is passcode;
map<int, string>* UserAuthority::_UserProfilesSC 			= nullptr;
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
	_UserPrivilegesSC 	= new map<int, int>();
	_UserProfilesSC		= new map<int, string>();
	_UserPrivilegesUI	= new vector<USER_PRIVILEGE>();
	_UserProfilesUI		= new vector<USER_PROFILE>();
	

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

