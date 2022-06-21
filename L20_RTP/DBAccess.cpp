/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains DBAccess class implementation.
 
**********************************************************************************************************/

#include "DBAccess.h"
#include "DBConfiguration.h"

/******************************************************************************
* \brief - Constructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccess::DBAccess()
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
DBAccess::~DBAccess()
{

}


/******************************************************************************
* \brief - Read WeldSignature Graph Blob Data from database for passed ROWID. 
*
* \param:
* nRowID - ROWID to read.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - The read database values are returned as comma separated values.
*
******************************************************************************/
string DBAccess::GetWeldSignatureGraphBlob(int nRowID,  int *pnStatus, int nRetryCounter)
{
	string strQuery = "SELECT " + (string)COLUMN_GRAPHDATA + " FROM " + TABLE_WELD_SIGNATURE + " WHERE ROWID=" + to_string(nRowID) + ";";
	return ExecuteQuery(strQuery, pnStatus, nRetryCounter);	
}

/******************************************************************************
* \brief - Get Row Numbers which are marked as Golden Weld Signatures in database. 
*
* \param: 
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - vector containing RowIDs marked as golden signatures in database.
*
******************************************************************************/
vector<int> DBAccess::GetGoldenWeldSignatureRowNumbers(int *pnStatus, int nRetryCounter)
{
	size_t position = 0;	
	string strRowID = "";
	string strDelimiter = DB_VALUE_SEPARATOR;
	vector<int> vRowIDs;
	vRowIDs.clear();
	
	string strQuery = "SELECT ROWID FROM " + (string)TABLE_WELD_SIGNATURE + " WHERE " + (string)COLUMN_GOLDEN + "=1;";
	string strResult = ExecuteQuery(strQuery, pnStatus, nRetryCounter);
	
	//Convert/Parse the RowIDs string returned from database (e.g. 1,2,3) into vector of int ROWID.	
	if(strResult != "")
	{
		while ((position = strResult.find(strDelimiter)) != string::npos) 
		{
		    strRowID = strResult.substr(0, position);
		    
		    vRowIDs.push_back(atoi(strRowID.c_str()));

		    strResult.erase(0, position + strDelimiter.length());
		}
		vRowIDs.push_back(atoi(strResult.c_str()));
	}
	
	return vRowIDs;
}




