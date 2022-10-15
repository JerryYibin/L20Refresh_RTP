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
    int GetLatestID(const string table, int* _id);
    int GetLatestID64(const string table, long long* _id);
    int Struct2JSON(const WeldStepValueSetting* _ptrArray, const unsigned int size, string& jsonStr);
    int JSON2Struct(const string jsonStr, WeldStepValueSetting* _ptrArray);
    int Array2Vector(const WeldStepValueSetting* _ptrArray, vector<WeldStepValueSetting>* _ptrvector);
    int Vector2Array(const vector<WeldStepValueSetting>* _ptrvector, WeldStepValueSetting* _ptrArray);
    
    int Vector2JSON(const vector<WELD_SIGNATURE>* _ptrVector, string& jsonStr);
    int JSON2Vector(const string jsonStr, vector<WELD_SIGNATURE>* _ptrVector);
    int Vector2String(const vector<WELD_SIGNATURE>* _ptrVector, string& str);
    int String2Vector(const string str, vector<WELD_SIGNATURE>* _ptrVector);
    int Vector2String(const vector<WeldStepValueSetting>* _ptrVector, string& str);
    int String2Vector(const string str, vector<WeldStepValueSetting>* _ptrVector);
    
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
	virtual int 	StoreAlarmLog(char* buffer)			override;

	virtual int 	QueryBlockWeldResult(char *) 		override;
	virtual void 	QueryWeldSignature(char *) 			override;
	virtual void 	QueryWeldRecipe(char *) 			override;
	virtual int 	QueryBlockAlarmLog(char *) 		  	override;
	virtual void 	QueryHeightCalibration(char *) 		override;
	virtual void 	QueryDbVersion(char *) 				override;
	virtual string 	QueryUserProfiles(char *) 			override;
	virtual string 	QueryPrivilegeConfiguration(char *) override;

	virtual void 	QueryWeldRecipeAll(char *) 			override;
	virtual int 	UpdateWeldRecipe(char *) 			override;
	virtual int 	UpdateHeightCalibration(char *) 	override;
	virtual int 	UpdateUserProfiles(char *) 			override;
	virtual int 	UpdatePrivilegeConfiguration(char *) override;

	virtual void 	DeleteOldest(const char *) 			override;

	//End of methods to fetch data from database needed for CSV reports generation.	
};
#endif /* DBACCESS_L20_DB_H_ */
