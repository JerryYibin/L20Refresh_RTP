/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Get System Type Implementation

 **********************************************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
//#include "SystemType.h"
typedef enum
{
	GSXP1,
	GSXE1,
	L20,
	INVALID_SYSTEM
}EN_SYSTEM_TYPE;

int g_enSysType = INVALID_SYSTEM;
/******************************************************************************
* \brief - Get System Type e.g. GSXP1, GSXE1, L20.
*
* \param - none
*
* \return - SYSTEM_TYPE
*
******************************************************************************/
int GetSystemType()
{
	//TO_BE_DONE - Add code here that will identify System Type.
	g_enSysType = L20;

	return g_enSysType;
}

