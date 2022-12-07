/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the SQLite Data base related functions   
 
**********************************************************************************************************/


#include "SQLiteDB.h"
#include "../Logger.h"
/******************************************************************************
* \brief - Constructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
SQLiteDB::SQLiteDB()
{
	m_ptrDB = nullptr;
	m_bIsDBOpen = false;
}

SQLiteDB::~SQLiteDB() {
	// TODO Auto-generated destructor stub
}

/******************************************************************************
* \brief - Callback function called from ExecuteQuery method.
*
* \param:
* vPtrData - Stores result in vPtrData as comma separated string values.
* argc - The number of columns in the result.
* argv - An array of pointers to strings obtained as if from sqlite3_column_text(), one for each column.
* azColName - An array of pointers to strings where each entry represents the name of corresponding result column as obtained from sqlite3_column_name().
*  
* \return - 0. Stores result in vPtrData as comma separated string values.
*
******************************************************************************/
int SQLiteDB::Callback(void *vPtrData, int argc, char **argv, char **azColName)
{
	string *strData = static_cast<string*>(vPtrData);
	for(int nIndex = 0; nIndex < argc; nIndex++)
	{
		strData->append(argv[nIndex]);
		strData->append(DB_VALUE_SEPARATOR);
	}
#ifdef DB_RECORD_SEPARATOR
	strData->replace(strData->size() - 1, 1, DB_RECORD_SEPARATOR);
#else
	strData->replace(strData->size() - 1, 1, DB_VALUE_SEPARATOR);
#endif
	return 0;
}

/******************************************************************************
* \brief - Establish/Open the SQLite database connection.
*
* \param:
* strDBPath - Path of Database File to be opened.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::EstablishDataBaseConnection(string strDBPath)
{
	int nErrCode = SQLITE_ERROR;
	string strQuery = "";
		
	nErrCode = sqlite3_open_v2(strDBPath.c_str(),&m_ptrDB, SQLITE_OPEN_READWRITE, nullptr);
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"EstablishDataBaseConnection - ERROR in opening database, nErrCode: %d",nErrCode,0,0);	
	}
	else
	{
		m_bIsDBOpen = true;
		//setting DB to NO auto vacuum mode
		strQuery = "PRAGMA auto_vacuum=none;";
		nErrCode = executeDBQuery(strQuery);
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"EstablishDataBaseConnection - ERROR in setting up database connection auto_vacuum to none, nErrCode: %d",nErrCode,0,0);
			m_bIsDBOpen = false;
		}
		else
		{
			//Set the enforcement of foreign key constraints.
			strQuery = "PRAGMA foreign_keys=ON;";
			nErrCode = executeDBQuery(strQuery);	
			if(nErrCode != SQLITE_OK)
			{
				LOGERR((char *)"EstablishDataBaseConnection - ERROR in setting up database connection foreign_keys to ON, nErrCode: %d",nErrCode,0,0);	
				m_bIsDBOpen = false;			
			}
			else
			{	
				//Set WAL Mode.			 
				strQuery = "PRAGMA journal_mode=WAL;";
				nErrCode = executeDBQuery(strQuery);
				if(nErrCode != SQLITE_OK)
				{
					LOGERR((char *)"EstablishDataBaseConnection - ERROR in setting up database connection journal_mode to WAL, nErrCode: %d",nErrCode,0,0);	
					m_bIsDBOpen = false;			
				}
			}
		}
	}
			
	return nErrCode;
}

/******************************************************************************
* 
* \brief - closes the SqLite Database.
*
* \param - None.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::CloseDataBaseConnection()
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_ptrDB != nullptr)
	{
		nErrCode = sqlite3_close(m_ptrDB);
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"CloseDataBaseConnection - ERROR in DataBase Connection close, nErrCode: %d",nErrCode,0,0);
		}
	}
		
	m_ptrDB = nullptr;
	m_bIsDBOpen = false;
	
	return nErrCode;
}

/******************************************************************************
* 
* \brief - Calls the sqlite3_exec function to execute specific statement.
* sqlite3_exec() called from ExecuteQuery() has a Callback function involved.
* ExecuteQuery() is declared public.
* \param:
* strSqlStatement - SQLite query/statement to be executed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.

* \return - Returns the result of execution of query. In case of database
* read operation (Select queries), the read database values are 
* returned as comma separated values.
*
******************************************************************************/
string SQLiteDB::ExecuteQuery(string strSqlStatement, int *pnStatus, int nRetryCounter)
{
	string strOutBuffer = "";
	int nErrCode = SQLITE_ERROR;

	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"ExecuteQuery - ERROR, Connection not opened", 0, 0, 0);
	}
	else
	{	//Callback function returns the comma separated string values in "strOutBuffer".
		do
		{
			nErrCode = sqlite3_exec(m_ptrDB, strSqlStatement.c_str(), Callback, &strOutBuffer, nullptr);
			switch (nErrCode)
			{
			case SQLITE_OK:
				break;

			case SQLITE_BUSY:
			case SQLITE_LOCKED:
				LOGERR((char* )"ExecuteQuery - SQLITE_BUSY/SQLITE_LOCKED, nErrCode: %d",nErrCode, 0, 0);
				if (nRetryCounter > 0)
				{
					taskDelay(SQLITE_BUSY_RETRY_INTERVAL);
				}
				break;

			default:
				LOGERR((char* )"ExecuteQuery - DEFAULT", 0, 0, 0);
				break;
			}
			nRetryCounter--;		
		}while( ( nRetryCounter > 0 ) && ( nErrCode == SQLITE_BUSY || nErrCode == SQLITE_LOCKED) );
		
		if(!strOutBuffer.empty())
		{
			//Erase the last DB_VALUE_SEPARATOR (,) from the returned comma separated string.
			strOutBuffer.erase(strOutBuffer.size() - 1);
		}		
	}

	if(pnStatus != nullptr)
	{
		*pnStatus = nErrCode;	
	}

	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"ExecuteQuery - FAILED, nErrCode: %d",nErrCode,0,0);
	}

	return strOutBuffer;
}

/******************************************************************************
* 
* \brief - Calls sqlite3 API for writing data into DB.
*
* \param:
* strSqlStatement - SQLite Query.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.

* \return  - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::SingleTransaction(string strSqlStatement, int nRetryCounter)
{
	return executeDBQuery(strSqlStatement, nRetryCounter);
}

/******************************************************************************
* 
* \brief - Bind the operation with transaction for writing bulk data.
*
* \param:
* strSqlStatement - SQLite Query.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::BulkTransaction(string strSqlStatement, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"BulkTransaction - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		nErrCode = executeDBQuery("BEGIN TRANSACTION;", nRetryCounter);
		if(nErrCode == SQLITE_OK)
		{
			nErrCode = executeDBQuery(strSqlStatement, 0);
			
			if(nErrCode == SQLITE_OK)
			{
				nErrCode = executeDBQuery("END TRANSACTION;", 0); //END TRANSACTION is an alias for COMMIT
				if(nErrCode != SQLITE_OK)
				{
					LOGERR((char *)"BulkTransaction - ERROR in END TRANSACTION, nErrCode: %d",nErrCode,0,0);	
				}
			}
			else
			{
				LOGERR((char *)"BulkTransaction - ERROR in executeDBQuery, Rolling Back, nErrCode: %d",nErrCode,0,0);
				if (executeDBQuery("ROLLBACK TRANSACTION;", 0) != SQLITE_OK)
				{
					LOGERR((char *)"BulkTransaction - ERROR in ROLLBACK TRANSACTION", 0,0,0);		
				}	
			}
		}
		else
		{
			LOGERR((char *)"BulkTransaction - ERROR, BEGIN TRANSACTION, nErrCode:%d",nErrCode,0,0);
		}
	}
	
	return nErrCode;
}

/******************************************************************************
* 
* \brief - Calls the SQLite commit via executeDBQuery.
*
* \param:
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::Commit(int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	string strQuery = "COMMIT;";
			
	nErrCode = executeDBQuery(strQuery, nRetryCounter);	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"COMMIT - ERROR: %d",nErrCode,0,0);
 	}
 	
	return nErrCode;	
}

/******************************************************************************
 * \brief - This function performs Database Vacuum.
 *
 * \param:
 * nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
 *
 *  return - SQLITE_OK (0) on success, else error code.
 *
 ******************************************************************************/
int SQLiteDB::Vacuum(int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	string strQuery = "VACUUM;";		
	nErrCode = executeDBQuery(strQuery, nRetryCounter);	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"Vacuum - ERROR: %d",nErrCode,0,0);
 	}
	else
	{
		//Commit(); //TO_BE_DONE / TO_BE_DISCUSSED - I think this call to Commit() is not needed. So commenting it out.
		//LOGDBG((char *)"Vacuum - Successfully Done DB VACUUM",0,0,0);
	}
	
	return nErrCode;
}

/******************************************************************************
* \brief - Returns the Table's Column Names
*
* \param:
* strTableName - Table Name whose column names are needed.
* pnStatus - Query execution status/result.
*
* \return - String containing Table's Column Names separated by comma.
*
******************************************************************************/
string SQLiteDB::GetDBColumnNames(string strTableName, int *pnStatus)
{
	int nErrCode = SQLITE_ERROR, ncolumnCount = 0;
	string strColumnHeaderBuf = "";
	sqlite3_stmt* ptrSqlStmt = nullptr;
	string strSqlStatement = "SELECT * FROM " + strTableName + " WHERE 0=1;";
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"GetDBColumnName - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		nErrCode = sqlite3_prepare_v2(m_ptrDB, strSqlStatement.c_str(), -1, &ptrSqlStmt, 0);
		
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"GetDBColumnName - ERROR in preparing SQLite Statement, nErrCode:%d",nErrCode,0,0);
		}
		else
		{
			ncolumnCount = sqlite3_column_count(ptrSqlStmt);
	
			for(int ncoloumnIndex = 0; ncoloumnIndex < ncolumnCount; ncoloumnIndex++)
			{
				strColumnHeaderBuf += sqlite3_column_name(ptrSqlStmt, ncoloumnIndex);
				
				if(ncoloumnIndex < ( ncolumnCount - 1) )
				{
					strColumnHeaderBuf.append(DB_VALUE_SEPARATOR);			
				}
			}
		}
			
		nErrCode = sqlite3_finalize(ptrSqlStmt);
		
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"GetColumnCount - ERROR in Finalize SQLite Statement, nErrCode:%d",nErrCode,0,0);
		}
	}
	
	if(pnStatus != nullptr)
	{
		*pnStatus = nErrCode;	
	}
	
	return strColumnHeaderBuf;
}

/******************************************************************************
* \brief - Returns the Number of Columns for passed table name
*
* \param:
* strTableName - Table Name whose column count is needed.
* pnStatus - Query execution status/result.
*
* \return - Number of Columns.
*
******************************************************************************/
int SQLiteDB::GetDBColumnCount(string strTableName, int *pnStatus)
{
	int nErrCode = SQLITE_ERROR, nColumnCount = 0;
	sqlite3_stmt* ptrSqlStmt = nullptr;
	string strSqlStatement = "SELECT * FROM " + strTableName + " WHERE 0=1;";
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"GetDBColumnCount - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		nErrCode = sqlite3_prepare_v2(m_ptrDB, strSqlStatement.c_str(), -1, &ptrSqlStmt, 0);
		
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"GetColumnCount - ERROR in Preparing SQLite Statement, nErrCode:%d",nErrCode,0,0);
		}
		else
		{
			nColumnCount = sqlite3_column_count(ptrSqlStmt);
		}
		
		nErrCode = sqlite3_finalize(ptrSqlStmt);
		
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"GetColumnCount - ERROR in Finalize SQLite Statement, nErrCode:%d",nErrCode,0,0);
		}
	}
	
	if(pnStatus != nullptr)
	{
		*pnStatus = nErrCode;	
	}

	return nColumnCount;
}

/******************************************************************************
* \brief - Returns the Number of Rows present in a table for passed table name
*
* \param:
* strTableName - Table Name whose row count is needed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - Number of Rows. -1 is returned in case of error.
*
******************************************************************************/
int SQLiteDB::GetNumberOfRows(string strTableName, int *pnStatus, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR, nRowCount = -1;

	string strQuery = "SELECT COUNT(1) FROM " + strTableName + ";";
	string strResult = "";
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"GetNumberOfRows - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		strResult = ExecuteQuery(strQuery, &nErrCode, nRetryCounter);	
		nRowCount = atoi(strResult.c_str());

		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"GetNumberOfRows - ERROR: %d",nErrCode,0,0);
	 	}
	}
	
	if(pnStatus != nullptr)
	{
		*pnStatus = nErrCode;	
	}
	
	return nRowCount;
}

/******************************************************************************
* \brief - Returns the Minimum RowID value in a table for passed table name.
*
* \param:
* strTableName - Table Name whose Minimum RowID is needed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - Minimum RowID value. -1 is returned in case of error.
*
******************************************************************************/
int SQLiteDB::GetMinRowID(string strTableName, int *pnStatus, int nRetryCounter)
{	
	return getMinMaxRowID(strTableName, true, pnStatus, nRetryCounter);
}

/******************************************************************************
* \brief - Returns the Maximum RowID value in a table for passed table name.
*
* \param:
* strTableName - Table Name whose Maximum RowID is needed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - Maximum RowID value. -1 is returned in case of error.
*
******************************************************************************/
int SQLiteDB::GetMaxRowID(std::string strTableName, int *pnStatus, int nRetryCounter)
{
	return getMinMaxRowID(strTableName, false, pnStatus, nRetryCounter);
}

/******************************************************************************
* \brief - Delete the passed RowID from database table.
*
* \param:
* strTableName - Table Name from where row needs to be deleted.
* nRowID - RowID to be deleted.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::DeleteOneTableRow(string strTableName, int nRowID, int nRetryCounter)
{
	string strQuery = "DELETE FROM " + strTableName + " WHERE ROWID=" + to_string(nRowID) + ";";
	
	return	executeDBQuery(strQuery, nRetryCounter);			
}

/******************************************************************************
* \brief - Delete the passed RowIDs range from database table.
*
* \param:
* strTableName - Table Name from where rows need to be deleted.
* nMinRowID - Min RowID to be deleted.
* nMaxRowID - Max RowID to be deleted.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::DeleteMultipleTableRows(string strTableName, int nMinRowID, int nMaxRowID, int nRetryCounter)
{
	string strQuery = "DELETE FROM " + strTableName + " WHERE ROWID BETWEEN " + std::to_string(nMinRowID) + " AND " + std::to_string(nMaxRowID) +";";
	
	return	executeDBQuery(strQuery, nRetryCounter);			
}

/******************************************************************************
* \brief - Delete all rows from database table.
*
* \param:
* strTableName - Table Name from where all rows need to be deleted.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::DeleteAllTableRows(string strTableName, int nRetryCounter)
{
    int nErrCode;
	string strQuery = "DELETE FROM " + strTableName + ";";
    nErrCode = executeDBQuery(strQuery, nRetryCounter);
    if(nErrCode == SQLITE_OK)
    {
    	string strQuery = "UPDATE sqlite_sequence SET seq = 0 WHERE name='" + strTableName + "';";
    	nErrCode = executeDBQuery(strQuery, nRetryCounter);
    }
	return nErrCode;
}

/******************************************************************************
* \brief - sqlite3_exec wrapper with retry mechanism for SQLITE_BUSY/SQLITE_LOCKED error.
*sqlite3_exec() called from executeDBQuery() has NO Callback function involved.
*executeDBQuery() is declared private. 
* \param:
* strSqlStatement - Query to be executed.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::executeDBQuery(string strSqlStatement, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"executeDBQuery - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		do
		{
			nErrCode = sqlite3_exec(m_ptrDB, strSqlStatement.c_str(), nullptr, nullptr, nullptr);
			switch(nErrCode)
			{
				case SQLITE_OK:
				break;
				
				case SQLITE_BUSY:
				case SQLITE_LOCKED:
				//LOGERR((char *)"executeDBQuery - SQLITE_BUSY/SQLITE_LOCKED, nErrCode: %d ",nErrCode,0,0);
				if(nRetryCounter > 0)
				{
					taskDelay(SQLITE_BUSY_RETRY_INTERVAL);
				}
				break;	
						
				default:
				break;
			}
			nRetryCounter--;		
		}while( ( nRetryCounter > 0 ) && ( nErrCode == SQLITE_BUSY || nErrCode == SQLITE_LOCKED) );;
	}
	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"executeDBQuery - FAILED, nErrCode: %d",nErrCode,0,0);
	}
#if 0 //submit manually
	else
	{
		nErrCode = sqlite3_wal_checkpoint(m_ptrDB, nullptr);
    	if(nErrCode != SQLITE_OK)
    	{
    		LOGERR((char *)"sqlite3_wal_checkpoint - FAILED, nErrCode: %d",nErrCode,0,0);
    	}
	}
#endif
	return nErrCode;
}

/******************************************************************************
* \brief - Returns the Minimum/Maximum RowID value in a table for passed 
* table name.
*
* \param:
* strTableName - Table Name whose Minimum RowID is needed.
* bIsMinRowIDRequest - Set to true if Min RowID needed else set to false
* if Max RowID needed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - Minimum/Maximum RowID based on value of "bIsMinRowIDRequest".
* -1 is returned in case of error.
*
******************************************************************************/
int SQLiteDB::getMinMaxRowID(string strTableName, bool bIsMinRowIDRequest, int *pnStatus, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR, nRowID = -1;
	string strResult = "", strQuery = "";
		
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR((char *)"getMinMaxRowID - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		if(bIsMinRowIDRequest == true)
		{
			strQuery = "SELECT MIN(ROWID) FROM " + strTableName + ";";
		}
		else
		{
			strQuery = "SELECT MAX(ROWID) FROM " + strTableName + ";";
		}
		
		strResult = ExecuteQuery(strQuery, &nErrCode, nRetryCounter);	
		nRowID = atoi(strResult.c_str());
		if(nErrCode != SQLITE_OK)
		{
			LOGERR((char *)"getMinMaxRowID - ERROR: %d",nErrCode,0,0);
	 	}
	}
	
	if(pnStatus != nullptr)
	{
		*pnStatus = nErrCode;	
	}
	
	return nRowID;
}
