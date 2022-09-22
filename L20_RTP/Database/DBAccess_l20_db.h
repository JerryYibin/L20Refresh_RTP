/*
 * DBAccess_l20_db.h
 *
 *  Created on: Jun 20, 2022
 *      Author: JerryW.Wang
 */

#ifndef DBACCESS_L20_DB_H_
#define DBACCESS_L20_DB_H_

#include "DBAccess.h"
#include "DBConfiguration.h"
#include "../CommonProperty.h"

class DBAccessL20DB: public DBAccess 
{
private:
    int GetLatestID(const char *);
public:
	DBAccessL20DB();
	virtual ~DBAccessL20DB();
	
	int ConnectDB() override;
	
	//Methods to fetch data from database needed for CSV reports generation.		
	string GetWeldResultCSVParameters(int nRowID,  int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) override;
	string GetWeldResultCSVReportHeader() override;	
		
	string GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) override;
	string GetWeldSignatureCSVReportHeader1() override;
	string GetWeldSignatureCSVReportHeader2() override;
	
	virtual int 	StoreWeldResult(char* buffer) 		override;
	virtual int 	StoreWeldSignature(char* buffer)	override;
	virtual int 	StoreWeldRecipe(char* buffer) 		override;

	virtual void	QueryWeldResult(char *) 			override;
	virtual void 	QueryWeldSignature(char *) 			override;
	virtual void 	QueryWeldRecipe(char *) 			override;
	virtual void 	QueryWeldRecipeAll(char *) 			override;
	virtual int 	UpdateWeldRecipe(int, char *) 			override;

	virtual void 	DeleteOldest(const char *) 			override;

	//End of methods to fetch data from database needed for CSV reports generation.	
};
#endif /* DBACCESS_L20_DB_H_ */
