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
#include "../Utility.h"
extern "C"
{
	#include "hwif/drv/resource/vxbRtcLib.h"
}
#undef RTC_MEASURE
#ifdef RTC_MEASURE
#include <timerDev.h>
#endif
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
    WELD_RESULT* pData = &CommonProperty::WeldResult;

	struct tm timeStamp;
    char timeBuf[20];
#ifdef RTC_MEASURE
	UINT32 m_startTime = sysTimestampLock();
#endif 
	vxbRtcGet(&timeStamp);
#ifdef RTC_MEASURE
	UINT32 m_endTime = sysTimestampLock();
    printf("vxbRtcGet took %u microseconds\n",
        (m_endTime-m_startTime)*1000000/sysTimestampFreq());
#endif
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

	string strStore =
        "insert into " + string(TABLE_WELD_RESULT) +
        " (PartID, DateTime, RecipeID, WeldEnergy, TriggerPressure, WeldPressure, " +
		"WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, " +
		"AlarmFlag, SequenceID, CycleCounter) " +
		"values ('"+
		pData->PartID+"','"+//PartID
		timeBuf+"',"+//DateTime
		std::to_string(pData->RecipeNum)+","+//RecipeID
		std::to_string(pData->TotalEnergy)+","+//WeldEnergy
		std::to_string(pData->TriggerPressure)+","+//TriggerPressure
		std::to_string(pData->WeldPressure)+","+//WeldPressure
		std::to_string(pData->Amplitude)+","+//WeldAmplitude
		std::to_string(pData->WeldTime)+","+//WeldTime
		std::to_string(pData->PeakPower)+","+//WeldPeakPower
		std::to_string(pData->PreHeight)+","+//TriggerHeight
		std::to_string(pData->PostHeight)+","+//WeldHeight
		std::to_string(pData->ALARMS.ALARMflags)+","+//AlarmFlag
		std::to_string(0)+","+//SequenceID
		std::to_string(pData->CycleCounter)+");";//CycleCounter

	int nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    static int count=0;
    printf("#WeldResult(num %d): result %d\n%s\n\n", count++,nErrCode, strStore.c_str());
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing WeldSignature into DB
*
* \param   - char *buffer - WeldResultID
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldSignature(char* buffer)
{
	int nErrCode = SQLITE_ERROR;

#ifdef UNITTEST_DATABASE
    if(CommonProperty::WeldSignatureVector.size()==0)
        {
    	WELD_SIGNATURE tmpWeldSignature;
    	tmpWeldSignature.Frquency = 1;
    	tmpWeldSignature.Power = 2;
    	tmpWeldSignature.Height = 3;
    	tmpWeldSignature.Amplitude = 4;
    	CommonProperty::WeldSignatureVector.push_back(tmpWeldSignature);

    	tmpWeldSignature.Frquency = 5;
    	tmpWeldSignature.Power = 6;
    	tmpWeldSignature.Height = 7;
    	tmpWeldSignature.Amplitude = 8;
    	CommonProperty::WeldSignatureVector.push_back(tmpWeldSignature);
        }
#endif
    string jsonStr;
    Utility::Vector2JSON(&CommonProperty::WeldSignatureVector, jsonStr);
    if(jsonStr.empty() != true)
    {
    	string strStore =
            "insert into " + string(TABLE_WELD_SIGNATURE) +
            " (WeldResultID, WeldGraph) " +
            "values ("+
            std::to_string(*(int *)buffer)+",'"+//WeldResultID
            jsonStr+"');";//WeldGraph

    	nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
        static int count=0;
        LOG("#WeldSignature(num %d): result %d\n%s\n\n", count++,nErrCode, strStore.c_str());
#endif
	}
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
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
    WeldRecipeSC *pData = (WeldRecipeSC *)buffer;

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    string jsonStrEnergy, jsonStrTime, jsonStrPower;
    Utility::Struct2JSON(pData->m_WeldParameter.m_EnergyStep, STEP_MAX, jsonStrEnergy);
    Utility::Struct2JSON(pData->m_WeldParameter.m_TimeStep, STEP_MAX, jsonStrTime);
    Utility::Struct2JSON(pData->m_WeldParameter.m_PowerStep, STEP_MAX, jsonStrPower);

	string strStore =
        "insert into " + string(TABLE_WELD_RECIPE) +
        " (UserID, IsValidate, Amplitude, Width, WeldPressure, " +
        "TriggerPressure, TimePlus, TimeMinus, PeakPowerPlus, PeakPowerMinus, " +
        "TriggerHeightPlus, TriggerHeightMinus, WeldHeightPlus, WeldHeightMinus, WeldMode, " +
        "ModeValue, PreBurst, HoldTime, SqueezeTime, AfterBurstDelay, " +
        "AfterBurstDuration, AfterBurstAmplitude, WeldHeight, MeasuredHeight, StepWeldMode, " +
        "EnergyToStep, TimeToStep, PowerToStep, RecipeName, DateTime, " +
        "PresetPicPath) " +
        "values (" +
        std::to_string(0)+","+//userID
        std::to_string(pData->m_IsTeachMode)+","+//IsValidate
        std::to_string(pData->m_WeldParameter.m_Amplitude)+","+//Amplitude
        std::to_string(pData->m_WeldParameter.m_WidthSetting)+","+//Width
        std::to_string(pData->m_WeldParameter.m_WPpressure)+","+//WeldPressure
        std::to_string(pData->m_WeldParameter.m_TPpressure)+","+//TriggerPressure
        std::to_string(pData->m_QualityWindowSetting.m_TimeMax)+","+//TimePlus
        std::to_string(pData->m_QualityWindowSetting.m_TimeMin)+","+//TimeMinus
        std::to_string(pData->m_QualityWindowSetting.m_PeakPowerMax)+","+//PeakPowerPlus
        std::to_string(pData->m_QualityWindowSetting.m_PeakPowerMin)+","+//PeakPowerMinus
        std::to_string(pData->m_QualityWindowSetting.m_PreHeightMax)+","+//TriggerHeightPlus
        std::to_string(pData->m_QualityWindowSetting.m_PreHeightMin)+","+//TriggerHeightMinus
        std::to_string(pData->m_QualityWindowSetting.m_HeightMax)+","+//WeldHeightPlus
        std::to_string(pData->m_QualityWindowSetting.m_HeightMin)+","+//WeldHeightMinus
        std::to_string(pData->m_AdvancedSetting.m_WeldMode)+","+//WeldMode
        std::to_string(0)+","+//ModeValue
        std::to_string(pData->m_AdvancedSetting.m_PreBurst)+","+//PreBurst
        std::to_string(pData->m_AdvancedSetting.m_HoldTime)+","+//HoldTime
        std::to_string(pData->m_AdvancedSetting.m_SqueezeTime)+","+//SqueezeTime
        std::to_string(pData->m_AdvancedSetting.m_AfterBurstDelay)+","+//AfterBurstDelay
        std::to_string(pData->m_AdvancedSetting.m_AfterBurstTime)+","+//AfterBurstDuration
        std::to_string(pData->m_AdvancedSetting.m_AfterBurstAmplitude)+","+//AfterBurstAmplitude
        std::to_string(pData->m_AdvancedSetting.m_DisplayedHeightOffset)+","+//WeldHeight
        std::to_string(pData->m_AdvancedSetting.m_MeasuredHeightOffset)+","+//MeasuredHeight
        std::to_string(pData->m_AdvancedSetting.m_WeldStepMode)+",'"+//StepWeldMode
		jsonStrEnergy+"','"+//EnergyToStep
		jsonStrTime+"','"+//TimeToStep
		jsonStrPower+"','"+//PowerToStep
        pData->m_RecipeName+"','"+//RecipeName
        timeBuf+"','"+//DateTime
        pData->m_RecipePicPath+"');";//PresetPicPath

	int nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    static int count=0;
    LOG("#WeldRecipe(num %d): result %d\n%s\n\n", count++,nErrCode, strStore.c_str());
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Query Weld Result from DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldResult(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RESULT)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);

    if(str.size()>0)
        LOG("QueryWeldResult:\n%s\n", str.c_str());
    return;
}

/**************************************************************************//**
* \brief   - Query Weld Signature from DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldSignature(char *buffer)
{
    string strQuery =
        "select "+string(COLUMN_GRAPHDATA)+
        " from "+string(TABLE_WELD_SIGNATURE)+
        " where WeldResultID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);
    vector<WELD_SIGNATURE> WeldSignVector;
    Utility::JSON2Vector(str, &WeldSignVector);
    return;
}

/**************************************************************************//**
* \brief   - Query all Weld Recipe records from DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldRecipeAll(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+";";
    string str = ExecuteQuery(strQuery);

    if(str.size()>0)
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());
    return;
}

/**************************************************************************//**
* \brief   - Query Specific Weld Recipe from DB
*
* \param   - char *buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldRecipe(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);
    if(str.size()>0)
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    Utility::JSON2Struct(str, NULL);

    strQuery =
        "select TimeToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    Utility::JSON2Struct(str, NULL);

    strQuery =
        "select PowerToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    Utility::JSON2Struct(str, NULL);
    return;
}

/**************************************************************************//**
* \brief   - Update Specific Weld Recipe to DB
*
* \param   - char *buffer (recipe structure)
*
* \return  - UINT8 - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateWeldRecipe(char *buffer)
{
    WeldRecipeSC *pRecipe = (WeldRecipeSC *)buffer;

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    string jsonStrEnergy, jsonStrTime, jsonStrPower;
    Utility::Struct2JSON(pRecipe->m_WeldParameter.m_EnergyStep, STEP_MAX, jsonStrEnergy);
    Utility::Struct2JSON(pRecipe->m_WeldParameter.m_TimeStep, STEP_MAX, jsonStrTime);
    Utility::Struct2JSON(pRecipe->m_WeldParameter.m_PowerStep, STEP_MAX, jsonStrPower);

	string strStore =
        "update " 				+ string(TABLE_WELD_RECIPE) +
        " set UserID=" 			+ std::to_string(0)+//userID
        ", IsValidate=" 		+ std::to_string(pRecipe->m_IsTeachMode)+//IsValidate
        ", Amplitude=" 			+ std::to_string(pRecipe->m_WeldParameter.m_Amplitude)+//Amplitude
        ", Width=" 				+ std::to_string(pRecipe->m_WeldParameter.m_WidthSetting)+//Width
        ", WeldPressure="	 	+ std::to_string(pRecipe->m_WeldParameter.m_WPpressure)+//WeldPressure
        ", TriggerPressure=" 	+ std::to_string(pRecipe->m_WeldParameter.m_TPpressure)+//TriggerPressure
        ", TimePlus=" 			+ std::to_string(pRecipe->m_QualityWindowSetting.m_TimeMax)+//TimePlus
        ", TimeMinus=" 			+ std::to_string(pRecipe->m_QualityWindowSetting.m_TimeMin)+//TimeMinus
        ", PeakPowerPlus=" 		+ std::to_string(pRecipe->m_QualityWindowSetting.m_PeakPowerMax)+//PeakPowerPlus
        ", PeakPowerMinus=" 	+ std::to_string(pRecipe->m_QualityWindowSetting.m_PeakPowerMin)+//PeakPowerMinus
        ", TriggerHeightPlus="	+ std::to_string(pRecipe->m_QualityWindowSetting.m_PreHeightMax)+//TriggerHeightPlus
        ", TriggerHeightMinus="	+ std::to_string(pRecipe->m_QualityWindowSetting.m_PreHeightMin)+//TriggerHeightMinus
        ", WeldHeightPlus=" 	+ std::to_string(pRecipe->m_QualityWindowSetting.m_HeightMax)+//WeldHeightPlus
        ", WeldHeightMinus=" 	+ std::to_string(pRecipe->m_QualityWindowSetting.m_HeightMin)+//WeldHeightMinus
        ", WeldMode=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_WeldMode)+//WeldMode
        ", ModeValue=" 			+ std::to_string(0)+//ModeValue
        ", PreBurst=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_PreBurst)+//PreBurst
        ", HoldTime=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_HoldTime)+//HoldTime
        ", SqueezeTime=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_SqueezeTime)+//SqueezeTime
        ", AfterBurstDelay=" 		+ std::to_string(pRecipe->m_AdvancedSetting.m_AfterBurstDelay)+//AfterBurstDelay
        ", AfterBurstDuration=" 	+ std::to_string(pRecipe->m_AdvancedSetting.m_AfterBurstTime)+//AfterBurstDuration
        ", AfterBurstAmplitude="	+ std::to_string(pRecipe->m_AdvancedSetting.m_AfterBurstAmplitude)+//AfterBurstAmplitude
        ", WeldHeight=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_DisplayedHeightOffset)+//WeldHeight
        ", MeasuredHeight=" 		+ std::to_string(pRecipe->m_AdvancedSetting.m_MeasuredHeightOffset)+//MeasuredHeight
        ", StepWeldMode=" 			+ std::to_string(pRecipe->m_AdvancedSetting.m_WeldStepMode)+//StepWeldMode
        ", EnergyToStep='" 			+ jsonStrEnergy+//EnergyToStep
        "', TimeToStep='" 			+ jsonStrTime+//TimeToStep
        "', PowerToStep='" 			+ jsonStrPower+//PowerToStep
        "', RecipeName='" 			+ pRecipe->m_RecipeName+//RecipeName
        "', DateTime='" 			+ timeBuf+//DateTime
        "', PresetPicPath='" 		+ pRecipe->m_RecipePicPath+//PresetPicPath
        "' where ID=" 				+ std::to_string(*(int *)buffer)+";";

	int nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    static int count=0;
    LOG("#WeldRecipe(len %d): result %d\n%s\n\n", strStore.size(),nErrCode, strStore.c_str());
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Delete table, please confirm what's the purpose of the function.
*
* \param   - table name
*
* \return  - none
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::DeleteOldest(const char *table)
{
    string strQuery =
        "delete from "+
        string(table)+" where ID in (select ID from "+
        string(table)+" order by ID asc limit 1);";
    SingleTransaction(strQuery);
    return;
}
