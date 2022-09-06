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

const string strInsert = "insert into %s ";
const string strWeldResultTableFormat = 
		"(partID, DateTime, RecipeID, WeldEnergy, TriggerPressure, WeldPressure, "
		"WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, "
		"AlarmFlag, SequenceID, CycleCounter) "
		"values ('%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);";
const string strWeldRecipeTableFormat = 
		"(UserID, IsValidate, Amplitude, Width, WeldPressure, "
		"TriggerPressure, TimePlus, TimeMinus, PeakPowerPlus, PeakPowerMinus, "
		"TriggerHeightPlus, TriggerHeightMinus, WeldHeightPlus, WeldHeightMinus, WeldMode, "
		"ModeValue, PreBurst, HoldTime, SqueezeTime, AfterBurstDelay, "
		"AfterBurstDuration, AfterBurstAmplitude, WeldHeight, MeasuredHeight, StepWeldMode, "
		"EnergyToStep, TimeToStep, PowerToStep, RecipeName, DateTime, "
		"PresetPicPath) "
		"values (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, "
		"'%s','%s','%s','%s','%s','%s');";
const string strWeldSignatureFormat = 
		"(WeldResultID, WeldGraph) "
		"values (%d,'%s');";


class DBAccessL20DB: public DBAccess 
{
public:
	enum DB_TABLE
	{
	    DB_INVALID = -1,
	    AlarmLog = 0,
	    SystemConfiguration,
	    EventLog,
	    MaintenanceCounter,
	    UserProfiles,
	    PrivilegeConfiguration,  /* 5 */
	    PrivilegeLevelName,
	    Sequence,
	    SequencePreset,
	    WeldRecipeTable,
	    LastOperate,             /* 10 */
	    Communication,
	    GlobalSetting,
	    WeldResultTable,
	    WeldResultSignature,
	    HeightCalibration,       /* 15 */
	    MaintenanceLog,
	    TeachModeConfiguration,
	    DBVersion
	};
	
private:
    char *struct2Json(WeldStepValueSetting *, int);
    void json2Struct(const char *json, WeldStepValueSetting *);

    char *vector2Json(vector<WELD_SIGNATURE>);
    void json2Vector(const char *json, vector<WELD_SIGNATURE> &WeldSignatureVector);
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
	
	virtual int StoreWeldResult(char* buffer) override;
	virtual int StoreWeldSignature(char* buffer) override;
	virtual int StoreWeldRecipe(char* buffer) override;

	virtual void QueryWeldResult(char *) override;
	virtual void QueryWeldSignature(char *) override;
	virtual void QueryWeldRecipe(char *) override;

	virtual void DeleteOldest(const char *) override;
#ifdef UNITTEST_DATABASE
    virtual void ClearTable(const char *) override;
#endif

	//End of methods to fetch data from database needed for CSV reports generation.	
};

#endif /* DBACCESS_L20_DB_H_ */
