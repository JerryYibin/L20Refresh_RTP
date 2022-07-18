/*
 * DBAccess_l20_db.h
 *
 *  Created on: Jun 20, 2022
 *      Author: JerryW.Wang
 */

#ifndef DBACCESS_L20_DB_H_
#define DBACCESS_L20_DB_H_

#include "DBAccess.h"

#define INSERT_STRING "insert into %s "
#define TABLE_NAME_LEN 30
typedef struct
	{
	int ID;
	int DateTime;
	int RecipeID;
	int WeldEnergy;
	int TriggerPressure;
	int WeldPressure;
	int WeldAmplitude;
	int WeldTime;
	int WeldPeakPower;
	int TriggerHeight;
	int WeldHeight;
	int AlarmFlag;
	int SequenceID;
	int CycleCounter;
	char partID[50];
	}WeldResultTable_Data;
#define WeldResultTable_String \
	"(CreatedTime,RecipeID,WeldEnergy,TriggerPressure,WeldPressure,WeldAmplitude,"\
	"WeldTime,WeldPeakPower,TriggerHeight,WeldHeight,AlarmFlag,SequenceID,CycleCounter,partID) "\
	"values (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s');"
typedef struct
	{
	int ID;
	int UserID;
	int IsValidate;
	int Amplitude;
	int Width;
	int WeldPressure;
	int TriggerPressure;
	int TimePlus;
	int TimeMinus;
	int PeakPowerPlus;
	int PeakPowerMinus;
	int TriggerHeightPlus;
	int TriggerHeightMinus;
	int WeldHeightPlus;
	int WeldHeightMinus;
	int WeldMode;
	int ModeValue;
	int PreBurst;
	int HoldTime;
	int SqueezeTime;
	int AfterBurstDelay;
	int AfterBurstDuration;
	int AfterBurstAmplitude;
	int WeldHeight;
	int MeasuredHeight;
	int StepWeldMode;
	char EnergyToStep[100];
	char TimeToStep[100];
	char PowerToStep[100];
	char RecipeName[100];
	char DateTime[20];
	char PresetPicPath[100];
	}WeldRecipeTable_Data;
typedef struct
	{
	int ID;
	int WeldResultID;
	char WeldGraph[200];
	}WeldResultSignature_Data;
typedef enum
    {
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
    DBVersion,
    DB_INVALID
    }DB_TABLE;
typedef enum
    {
    CMD_INSERT = 0,
    CMD_QUERY,
    CMD_UPDATE,
    CMD_INVALID
    }DB_ACCESS;
typedef union
	{
	WeldRecipeTable_Data data9;
	WeldResultTable_Data data13;
	WeldResultSignature_Data data14;
	}DB_DATA;
typedef struct
    {
    DB_TABLE db;
    DB_ACCESS cmd;
    DB_DATA data;
    }MSG_DATA;
typedef struct
    {
    UINT32 msgID;
    MSG_DATA msgData;
    }MESSAGE_DB;

class DBAccessL20DB: public DBAccess 
{
public:
	char tableName[DB_INVALID][TABLE_NAME_LEN];
	DBAccessL20DB();
	virtual ~DBAccessL20DB();
	
	int ConnectDB() override;

	int dataInsert(DB_TABLE t, DB_DATA data);
	string dataQuery(DB_TABLE t);

	//Methods to fetch data from database needed for CSV reports generation.		
	string GetWeldResultCSVParameters(int nRowID,  int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) override;
	string GetWeldResultCSVReportHeader() override;	
		
	string GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int *pnStatus = nullptr, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT) override;
	string GetWeldSignatureCSVReportHeader1() override;
	string GetWeldSignatureCSVReportHeader2() override;
	//End of methods to fetch data from database needed for CSV reports generation.	
};

#endif /* DBACCESS_L20_DB_H_ */
