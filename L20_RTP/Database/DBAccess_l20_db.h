/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 Use this class to access L20 refresh database file sample_L20.db.  
 
**********************************************************************************************************/
#ifndef DBACCESS_L20_DB_H_
#define DBACCESS_L20_DB_H_

#include <sstream>
#include "DBAccess.h"
#include "DBConfiguration.h"
#include "../Commons/RecipeUI.h"
#include "../CommonProperty.h"

#define	ONE_PAGE_NUM			10
#define FIRST_SEND_DATA_NUM		10

class DBAccessL20DB: public DBAccess 
{
private:
	int sendDataNum;
    int getLatestID(const string table, int* _id);
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
    int pushWeldRecipeLib(const string str);
    
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
	virtual int 	QueryWeldRecipe(char *) 			override;
	virtual int 	QueryBlockAlarmLog(char *) 		  	override;
	virtual void 	QueryHeightCalibration(char *) 		override;
	virtual int 	QueryDbVersion(char *) 				override;
	virtual int 	QueryBlockUserProfiles(char *) 		override;
	virtual int 	QueryBlockPrivilegeConfigure(char *) override;
	virtual int 	QueryBlockPowerSupply(char *)		override;
	virtual int 	QueryBlockTeachModeSetting(char *)	override;
	virtual void 	QuerySystemConfigure(char *)		override;
	virtual void 	QueryActiveRecipe(char *)			override;

	virtual void 	QueryWeldRecipeAll(char *) 			override;
	virtual int 	QueryWeldRecipeLatestPage() 		override;
	virtual int 	QueryWeldRecipeNextPage()			override;
	virtual int 	UpdateWeldRecipe(char *) 			override;
	virtual int 	UpdateHeightCalibration(char *) 	override;
	virtual int 	UpdateUserProfiles(char *) 			override;
	virtual int 	UpdatePrivilegeConfigure(char *)	override;
	virtual int 	UpdateBlockPowerSupply(char *) 		override;
	virtual int 	UpdateBlockTeachModeSetting(char *)	override;
	virtual int 	UpdateSystemConfigure(char *)		override;
	virtual int 	UpdateActiveRecipe(char *)			override;

	virtual void 	DeleteOldest(const char *) 			override;
	virtual int 	DeleteSpecificRecipe(const char *)	override;

	//End of methods to fetch data from database needed for CSV reports generation.	
};
#endif /* DBACCESS_L20_DB_H_ */
