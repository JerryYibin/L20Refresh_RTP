/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef DBACCESS_H_
#define DBACCESS_H_

#include <vector>
#include "SQLiteDB.h"


//Abstract Class DBAccess. SQLiteDB is its parent class.
//DBAccessP1BaseDB is a derieved/child class of DBAccess parent class.
//DBAccessL20BaseDB is a derived / child class of DBAccess parent class.
class DBAccess: public SQLiteDB 
{
public:
	DBAccess();
	virtual ~DBAccess();
	
	//Pure virtual function. Each child class (e.g. DBAccessP1BaseDB) must provide a ConnectDB() implementation based on the database file name that it will operate on.
	virtual int		ConnectDB() = 0;
	
	//Methods to fetch data from database needed for CSV reports generation.		
	virtual string	GetWeldResultCSVParameters(int nRowID, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) = 0;
	virtual string	GetWeldResultCSVReportHeader() = 0;
	
	virtual string	GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) = 0;
	virtual string	GetWeldSignatureCSVReportHeader1() = 0;
	virtual string	GetWeldSignatureCSVReportHeader2() = 0;
	
	virtual int 	StoreWeldResult(char* buffer)		= 0;
	virtual int 	StoreWeldSignature(char* buffer)	= 0;
	virtual int 	StoreWeldRecipe(char* buffer)		= 0;
	virtual int 	StoreAlarmLog(char* buffer)			= 0;

	virtual int 	QueryBlockWeldResult(char *)		= 0;
	virtual void 	QueryWeldSignature(char *)			= 0;
	virtual int 	QueryWeldRecipe(char *)				= 0;
	virtual int 	QueryBlockAlarmLog(char *)		= 0;
	virtual void 	QueryHeightCalibration(char *)		= 0;
	virtual int 	QueryDbVersion(char *)				= 0;
	virtual int 	QueryBlockUserProfiles(char *)			= 0;
	virtual int 	QueryBlockPrivilegeConfigure(char *)	= 0;
	virtual int 	QueryBlockPowerSupply(char *)		= 0;
	virtual int 	QueryBlockTeachModeSetting(char *)	= 0;
	virtual void 	QuerySystemConfigure(char *)		= 0;
	virtual void 	QueryActiveRecipe(char *)		= 0;
	
	virtual void 	QueryWeldRecipeAll(char *)			= 0;
	virtual int 	QueryWeldRecipeLatestPage()			= 0;
	virtual int 	QueryWeldRecipeNextPage()			= 0;
	virtual int 	UpdateWeldRecipe(char *)			= 0;
	virtual int 	UpdateHeightCalibration(char *)	= 0;
	virtual int 	UpdateUserProfiles(char *)			= 0;
	virtual int 	UpdatePrivilegeConfigure(char *)	= 0;
	virtual int 	UpdateBlockPowerSupply(char *)		= 0;
	virtual int 	UpdateBlockTeachModeSetting(char *)	= 0;
	virtual int 	UpdateSystemConfigure(char *)		= 0;
	virtual int 	UpdateActiveRecipe(char *)		= 0;

	virtual void 	DeleteOldest(const char *)			= 0;
	virtual int		DeleteSpecificRecipe(const char *)	= 0;
	
	string GetWeldSignatureGraphBlob(int nRowID, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	vector <int> GetGoldenWeldSignatureRowNumbers(int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	//End of methods to fetch data from database needed for CSV reports generation.
};

#endif /* DBACCESS_H_ */
