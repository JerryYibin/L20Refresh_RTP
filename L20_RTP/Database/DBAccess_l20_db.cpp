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
#include "../WeldResults.h"
#include "../Commons/recipedef.h"
#include "DBConfiguration.h"
extern "C"
{
	#include "hwif/drv/resource/vxbRtcLib.h"
}

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

/**************************************************************************//**
* \brief   - Writing WeldResult into DB
*      		
* \param   - char *buffer
*
* \return  - UINT8 -status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldResult(char* buffer)
{
	int nErrCode = SQLITE_ERROR;
	char insertQuery[DB_QUERY_SIZE] = {0x00};
	WELD_RESULT result;
	struct tm timeStamp;
	vxbRtcSet(&timeStamp);
	
	memcpy(&result, buffer, sizeof(WELD_RESULT));
	sprintf(insertQuery, string(strInsert + strWeldResultTableFormat).c_str(), 
			TABLE_WELD_RESULT,
			mktime((struct tm*)&timeStamp),
			result.RecipeNum,
			result.TotalEnergy,
			result.TriggerPressure,
			result.WeldPressure,
			result.Amplitude,
			result.WeldTime,
			result.PeakPower,
			result.PreHeight,
			result.PostHeight,
			result.ALARMS.ALARMflags,
			0,//sequence ID
			result.CycleCounter,
			result.PartID
			);
	nErrCode = SingleTransaction((string)insertQuery);
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing WeldSignature into DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldSignature(char* buffer)
{
	return OK;
}

/**************************************************************************//**
* \brief   - Writing Weld Recipe into DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldRecipe(char* buffer)
{
	int nErrCode = SQLITE_ERROR;
	char insertQuery[DB_QUERY_SIZE] = {0x00};
	WeldRecipeSC recipe;
	struct tm timeStamp;
	vxbRtcSet(&timeStamp);
	
	memcpy(&recipe, buffer, sizeof(WeldRecipeSC));
	sprintf(insertQuery, string(strInsert + strWeldRecipeTableFormat).c_str(),
			TABLE_WELD_RECIPE,
			0, //userID
			recipe.m_IsTeachMode,
			recipe.m_WeldParameter.m_Amplitude,
			recipe.m_WeldParameter.m_WidthSetting,
			recipe.m_WeldParameter.m_WPpressure,
			recipe.m_WeldParameter.m_TPpressure,
			recipe.m_QualityWindowSetting.m_TimeMax,
			recipe.m_QualityWindowSetting.m_TimeMin,
			recipe.m_QualityWindowSetting.m_PeakPowerMax,
			recipe.m_QualityWindowSetting.m_PeakPowerMin,
			recipe.m_QualityWindowSetting.m_PreHeightMax,
			recipe.m_QualityWindowSetting.m_PreHeightMin,
			recipe.m_QualityWindowSetting.m_HeightMax,
			recipe.m_QualityWindowSetting.m_HeightMin,
			recipe.m_AdvancedSetting.m_WeldMode,
			recipe.m_AdvancedSetting.m_PreBurst,
			recipe.m_AdvancedSetting.m_HoldTime,
			recipe.m_AdvancedSetting.m_SqueezeTime,
			recipe.m_AdvancedSetting.m_AfterBurstDelay,
			recipe.m_AdvancedSetting.m_AfterBurstTime,
			recipe.m_AdvancedSetting.m_AfterBurstAmplitude,
			recipe.m_AdvancedSetting.m_DisplayedHeightOffset,
			recipe.m_AdvancedSetting.m_MeasuredHeightOffset,
			recipe.m_AdvancedSetting.m_WeldStepMode,
			recipe.m_WeldParameter.m_EnergyStep,
			recipe.m_WeldParameter.m_TimeStep,
			recipe.m_WeldParameter.m_PowerStep,
			recipe.m_RecipeName,
			"2022-08-11 00:00:00",
			recipe.m_RecipePicPath
			);
	nErrCode = SingleTransaction((string)insertQuery);
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}




