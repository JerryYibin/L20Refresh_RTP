/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SQLITEDB_H_
#define SQLITEDB_H_

#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include "../Common.h"
//Values read from database are separated by comma
#define DB_VALUE_SEPARATOR	","

//records read from database are separated by semicolon
#define DB_RECORD_SEPARATOR	";"
#undef  DB_RECORD_SEPARATOR
#ifndef DB_RECORD_SEPARATOR
#endif

//Retry interval in case SQLITE_BUSY/SQLITE_LOCKED error returned.
#define SQLITE_BUSY_RETRY_INTERVAL	FIFTY_MS_DELAY
					
//Retry count in case SQLITE_BUSY/SQLITE_LOCKED error returned.
//Thus, default SQLITE_BUSY/SQLITE_LOCKED timeout becomes 4 retries of 50 milliseconds gap = 200 milliseconds. 
#define SQLITE_BUSY_DEFAULT_RETRY_COUNT 4

//Database access wrapper class.
//Abstract class.Use DBAccessP1BaseDB, DBAccessCommonDB classes to access the database files.
using namespace std;
class SQLiteDB 
{
public:
	SQLiteDB();
	virtual ~SQLiteDB();
	
	//Pure virtual function. Each child class (e.g. DBAccessCommonDB) must provide a ConnectDB() implementation based on the database file name that it will operate on.
	virtual int ConnectDB() = 0;
	int CloseDataBaseConnection();
	string ExecuteQuery(string strSqlStatement, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);	
	int SingleTransaction(string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int BulkTransaction(string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int Commit(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int Vacuum(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);	
	string GetDBColumnNames(string strTableName, int *pnStatus = nullptr);
	int GetDBColumnCount(string strTableName, int *pnStatus = nullptr);
	int GetNumberOfRows(string strTableName, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int GetMinRowID(string strTableName, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int GetMaxRowID(string strTableName, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int DeleteOneTableRow(string strTableName, int nRowID, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int DeleteMultipleTableRows(string strTableName, int nMinRowID, int nMaxRowID, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int DeleteAllTableRows(string strTableName, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
		
protected:	
	int EstablishDataBaseConnection(string strDBPath);
	
private:
	bool m_bIsDBOpen;
	sqlite3 *m_ptrDB;
	
	int executeDBQuery(std::string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int getMinMaxRowID(std::string strTableName, bool bIsMinRowIDRequest, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);			
	static int Callback(void *vPtrData, int argc, char **argv, char **azColName);
};

#endif /* SQLITEDB_H_ */
