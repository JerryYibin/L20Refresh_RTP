/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains DBAccessP1BaseDB class implementation.
 	 Use this class to access GSX_P1_Base database file sample_p1_base.db.  
 
**********************************************************************************************************/

#include "DBAccess_p1_base_db.h"
#include "DBConfiguration.h"
#include "commons.h"

/******************************************************************************
* \brief - Constructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccessP1BaseDB::DBAccessP1BaseDB()
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
DBAccessP1BaseDB::~DBAccessP1BaseDB()
{

}

/******************************************************************************
* \brief - Connect to P1_Base Database.
*
* \param - None.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int DBAccessP1BaseDB::ConnectDB()
{
	return EstablishDataBaseConnection(P1_BASE_DB_FILE_PATH_EMMC);
}

/******************************************************************************
* \brief - GetWeldResultCSVParameters method reads the data from database 
* for passed row number needed for CSV WeldResult Report Generation.
*
* \param:
* nRowID - RowID to read.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - the read database values are returned as comma separated values.
*
******************************************************************************/
string DBAccessP1BaseDB::GetWeldResultCSVParameters(int nRowID,  int *pnStatus, int nRetryCounter)
{
	string strQuery = "SELECT " + strP1BaseWeldResultCSVParamString + " FROM " + TABLE_WELD_RESULT + " WHERE ROWID=" + to_string(nRowID) + ";";
	return ExecuteQuery(strQuery, pnStatus, nRetryCounter);	
}


/******************************************************************************
* \brief - Get WeldResult CSV Report Header.
*
* \param - None.
*
* \return - string strP1BaseWeldResultCSVParamString
*
******************************************************************************/
string DBAccessP1BaseDB::GetWeldResultCSVReportHeader()
{
	return strP1BaseWeldResultCSVParamString;
}

/******************************************************************************
* \brief - GetWeldSignatureCSVParameters method reads the data from database 
* for passed row number needed for CSV WeldSignature Report Generation.
*
* \param:
* nRowID - RowID to read.
* nBlobParamNumber - Number of parameters present in Graph Data blob for 1 set of data.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - the read database values are returned as comma separated values.
*
******************************************************************************/
string DBAccessP1BaseDB::GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int *pnStatus, int nRetryCounter)
{
	string strQuery = "SELECT " + strP1BaseWeldSignatureCSVParamString + " FROM " + TABLE_WELD_SIGNATURE + " WHERE ROWID=" + to_string(nRowID) + ";";
	nBlobParamNumber = BLOB_PARAMETERS_NUMBER_P1_BASE;
	
	return ExecuteQuery(strQuery, pnStatus, nRetryCounter);	
}

/******************************************************************************
* \brief - Get WeldSignature CSV Report Header 1.
*
* \param - None.
*
* \return - string strP1BaseWeldSignatureCSVReportHeader1
*
******************************************************************************/
string DBAccessP1BaseDB::GetWeldSignatureCSVReportHeader1()
{
	return strP1BaseWeldSignatureCSVReportHeader1;
}

/******************************************************************************
* \brief - Get WeldSignature CSV Report Header 2.
*
* \param - None.
*
* \return - string strP1BaseWeldSignatureCSVReportHeader2
*
******************************************************************************/
string DBAccessP1BaseDB::GetWeldSignatureCSVReportHeader2()
{
	return strP1BaseWeldSignatureCSVReportHeader2;
}









