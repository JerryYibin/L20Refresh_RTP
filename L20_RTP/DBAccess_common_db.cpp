/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	It contains DBAccessCommonDB class implementation.
 	Use this class to access sample_common.db database file.   
 
**********************************************************************************************************/

#include "DBAccess_common_db.h"
#include "commons.h"

/******************************************************************************
* \brief - Constructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccessCommonDB::DBAccessCommonDB()
{

}

/******************************************************************************
* \brief - Destructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccessCommonDB::~DBAccessCommonDB()
{

}

/******************************************************************************
* \brief - Connect to Common Database.
*
* \param - None.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int DBAccessCommonDB::ConnectDB()
{
	return EstablishDataBaseConnection(COMMON_DB_FILE_PATH_EMMC);
}






