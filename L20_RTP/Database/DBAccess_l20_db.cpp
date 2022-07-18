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
    char str[400];
    switch(t)
        {
        case WeldRecipeTable:
            sprintf(str, INSERT_STRING,
                tableName[t]);
            printf("##dataInsert: %s\n", str);
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
            printf("##dataInsert: %s\n", str);
            result = ExecuteInsert((string)str);
            if(result != 0)
                printf("##dataInsert: result %d\n", result);
            break;
        case WeldResultSignature:
            sprintf(str, INSERT_STRING,
                tableName[t]);
            printf("##dataInsert: %s\n", str);
            break;
        default:
            printf("##dataInsert: unknow %d\n", t);
            break;
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









