/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains DBAccessP1BaseDB class implementation.
 	 Use this class to access L20 refresh database file sample_L20.db.  
 
**********************************************************************************************************/
#include "DBAccess_l20_db.h"
#include "commons.h"

/******************************************************************************
* \brief - Constructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccessL20DB::DBAccessL20DB() 
{
    strncpy(tableName[AlarmLog], "AlarmLog", TABLE_NAME_LEN);
    strncpy(tableName[WeldRecipeTable], "WeldRecipeTable", TABLE_NAME_LEN);
    strncpy(tableName[WeldResultTable], "WeldResultTable", TABLE_NAME_LEN);
    strncpy(tableName[WeldResultSignature], "WeldResultSignature", TABLE_NAME_LEN);
}

/******************************************************************************
* \brief - Destructor.
*
* \param - None.
*
* \return - None.
*
******************************************************************************/
DBAccessL20DB::~DBAccessL20DB() 
{

}

/******************************************************************************
* \brief - Connect to L20 Database.
*
* \param - None.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int DBAccessL20DB::ConnectDB()
{
	return EstablishDataBaseConnection(L20_DB_FILE_PATH_EMMC);
}
int DBAccessL20DB::dataInsert(DB_TABLE t, DB_DATA data)
{
    int result = 0;
    char str[600];
    switch(t)
        {
        case WeldRecipeTable:
            sprintf(str, INSERT_STRING WeldRecipeTable_String,
                tableName[t],
                data.data9.UserID,
                data.data9.IsValidate,
                data.data9.Amplitude,
                data.data9.Width,
                data.data9.WeldPressure,
                data.data9.TriggerPressure,
                data.data9.TimePlus,
                data.data9.TimeMinus,
                data.data9.PeakPowerPlus,
                data.data9.PeakPowerMinus,
                data.data9.TriggerHeightPlus,
                data.data9.TriggerHeightMinus,
                data.data9.WeldHeightPlus,
                data.data9.WeldHeightMinus,
                data.data9.WeldMode,
                data.data9.ModeValue,
                data.data9.PreBurst,
                data.data9.HoldTime,
                data.data9.SqueezeTime,
                data.data9.AfterBurstDelay,
                data.data9.AfterBurstDuration,
                data.data9.AfterBurstAmplitude,
                data.data9.WeldHeight,
                data.data9.MeasuredHeight,
                data.data9.StepWeldMode,
                data.data9.EnergyToStep,
                data.data9.TimeToStep,
                data.data9.PowerToStep,
                data.data9.RecipeName,
                data.data9.DateTime,
                data.data9.PresetPicPath);
            break;
        case WeldResultTable:
            sprintf(str, INSERT_STRING WeldResultTable_String,
                tableName[t],
                data.data13.DateTime,
                data.data13.RecipeID,
                data.data13.WeldEnergy,
                data.data13.TriggerPressure,
                data.data13.WeldPressure,
                data.data13.WeldAmplitude,
                data.data13.WeldTime,
                data.data13.WeldPeakPower,
                data.data13.TriggerHeight,
                data.data13.WeldHeight,
                data.data13.AlarmFlag,
                data.data13.SequenceID,
                data.data13.CycleCounter,
                data.data13.partID);
            break;
        case WeldResultSignature:
            sprintf(str, INSERT_STRING WeldResultSignature_String,
                tableName[t],
                data.data14.WeldResultID,
                data.data14.WeldGraph);
            break;
        default:
            printf("##dataInsert: unknow %d\n", t);
            result = -1;
            break;
        }
    if(result == 0)
        {
        result = ExecuteInsert((string)str);
        if(result != 0)
            printf("##ExecuteInsert: result %d - %s\n\n", result, str);
        }
    return result;
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
string DBAccessL20DB::GetWeldResultCSVParameters(int nRowID,  int *pnStatus, int nRetryCounter)
{
	return "";
}


/******************************************************************************
* \brief - Get WeldResult CSV Report Header.
*
* \param - None.
*
* \return - string strP1BaseWeldResultCSVParamString
*
******************************************************************************/
string DBAccessL20DB::GetWeldResultCSVReportHeader()
{
	return "";
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
string DBAccessL20DB::GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int *pnStatus, int nRetryCounter)
{
	return "";
}

/******************************************************************************
* \brief - Get WeldSignature CSV Report Header 1.
*
* \param - None.
*
* \return - string strP1BaseWeldSignatureCSVReportHeader1
*
******************************************************************************/
string DBAccessL20DB::GetWeldSignatureCSVReportHeader1()
{
	return "";
}

/******************************************************************************
* \brief - Get WeldSignature CSV Report Header 2.
*
* \param - None.
*
* \return - string strP1BaseWeldSignatureCSVReportHeader2
*
******************************************************************************/
string DBAccessL20DB::GetWeldSignatureCSVReportHeader2()
{
	return "";
}









