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
#include "../Recipe.h"
#include <jansson.h>
extern "C"
{
	#include "hwif/drv/resource/vxbRtcLib.h"
}
#undef RTC_MEASURE
#ifdef RTC_MEASURE
#include <timerDev.h>
#endif

extern long long atoll(const char *);

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
* \param   - char *buffer - not used
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
    LOG("vxbRtcGet took %u microseconds\n",
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
    long long id = ERROR;
    getLatestID64(TABLE_WELD_RESULT, &id);

	if(nErrCode == 0)
	{
#ifdef UNITTEST_DATABASE
        LOG("# store WeldResult to ID %llu\n", id);
        LOG("# %s\n", strStore.c_str());
#endif
        if(id > DB_RECORDS_STORAGE_WELD_RESULT_LIMIT)
		{
        	DeleteOldest(TABLE_WELD_RESULT);
		}
	}
	else
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %llu\n", nErrCode, id, 0);
	}
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
	string str;
	long long id = *(long long *)buffer;
	int nErrCode = SQLITE_ERROR;

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_SIGNATURE)+
            " where WeldResultID="+std::to_string(id)+";");
	if(str.empty()!=true)
	{
		LOGERR((char*) "Database_T: WeldResultID %llu already exists in table WeldResultSignature\n", id, 0, 0);
    	return nErrCode;
	}

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RESULT)+
            " where ID="+std::to_string(id)+";");
	if(str.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %llu doesn't exist in table WeldResult\n", id, 0, 0);
    	return nErrCode;
	}

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
	str.clear();
	Vector2String(&CommonProperty::WeldSignatureVector, str);
	if(str.empty() != true)
	{
    	string strStore =
            "insert into " + string(TABLE_WELD_SIGNATURE) +
            " (WeldResultID, WeldGraph) " +
            "values ("+
            std::to_string(id)+",'"+//WeldResultID
            str+"');";//WeldGraph

        str.shrink_to_fit();
    	nErrCode = SingleTransaction(strStore);
	}

	getLatestID64(TABLE_WELD_SIGNATURE, &id);

	if(nErrCode == 0)
	{
#ifdef UNITTEST_DATABASE
        LOG("# store WeldSignature to ID %llu\n", id);
#endif
        if(id > DB_RECORDS_STORAGE_WELD_SIGNAT_LIMIT)
		{
            DeleteOldest(TABLE_WELD_SIGNATURE);
		}
	}
	else
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %llu\n", nErrCode, id, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing Weld Recipe into DB
*
* \param   - char *buffer - WeldRecipeSC data
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldRecipe(char* buffer)
{
    int id = ERROR;
    getLatestID(TABLE_WELD_RECIPE, &id);
#ifdef UNITTEST_DATABASE
    LOG("# already %d WeldRecipe existing, try to insert new one\n", id);
#endif
    if(id >= DB_RECORDS_STORAGE_WELD_RECIPE_LIMIT)
	{
		LOGERR((char*) "Database_T: WeldRecipe reached the limit.\n", 0, 0, 0);
        return SQLITE_ERROR;
	}

    WeldRecipeSC *pData = (WeldRecipeSC *)buffer;

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    vector<WeldStepValueSetting> vectorEnergy, vectorTime, vectorPower;
    string strEnergy, strTime, strPower;
    Array2Vector(pData->m_WeldParameter.m_EnergyStep, &vectorEnergy);
    Array2Vector(pData->m_WeldParameter.m_TimeStep, &vectorTime);
    Array2Vector(pData->m_WeldParameter.m_PowerStep, &vectorPower);
    
    Vector2String(&vectorEnergy, strEnergy);
    Vector2String(&vectorTime, strTime);
    Vector2String(&vectorPower, strPower);

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
		strEnergy+"','"+//EnergyToStep
		strTime+"','"+//TimeToStep
		strPower+"','"+//PowerToStep
        pData->m_RecipeName+"','"+//RecipeName
        timeBuf+"','"+//DateTime
        pData->m_RecipePicPath+"');";//PresetPicPath
        strEnergy.shrink_to_fit();
        strTime.shrink_to_fit();
        strPower.shrink_to_fit();

	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing AlarmLog into DB
*
* \param   - char *buffer - WeldResultID
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreAlarmLog(char* buffer)
{
	string str;
	long long id = *(long long *)buffer;
	int nErrCode = SQLITE_ERROR;

	str = ExecuteQuery(
            "select * from "+string(TABLE_ALARM_LOG)+
            " where WeldResultID="+std::to_string(id)+";");
	if(str.empty()!=true)
	{
		LOGERR((char*) "Database_T: WeldResultID %llu already exists in table AlarmLog\n", id, 0, 0);
    	return nErrCode;
	}

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RESULT)+
            " where ID="+std::to_string(id)+";");
	if(str.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %llu doesn't exist in table WeldResult\n", id, 0, 0);
    	return nErrCode;
	}

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

	str =
        "insert into " + string(TABLE_ALARM_LOG) +
        " (DateTime, AlarmType, RecipeID, WeldResultID, UserID, IsReset) " +
        "values ('"+
        timeBuf+"',"+//DateTime
        std::to_string(1)+","+//AlarmType
        std::to_string(1)+","+//RecipeID
        std::to_string(id)+","+//WeldResultID
        std::to_string(1)+","+//UserID
        std::to_string(1)+");";//IsReset
	nErrCode = SingleTransaction(str);

	getLatestID64(TABLE_ALARM_LOG, &id);
	if(nErrCode == 0)
	{
#ifdef UNITTEST_DATABASE
        LOG("# store AlarmLog to ID %llu\n", id);
#endif
	}
	else
    {
        LOG("StoreAlarmLog:\n%s\n", str.c_str());
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %llu\n", nErrCode, id, 0);
    }
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Query the latest records from table Weld Result
*
* \param   - char *buffer - not used
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::QueryBlockWeldResult(char *buffer)
{
    int count;
    string str;
	vector<string> tmpStr;
	long long id = *(long long *)buffer;

    str = ExecuteQuery(
                string("select * from ")+string(TABLE_WELD_RESULT)+
                " where ID <= "+std::to_string(id)+" and ID > "+
                std::to_string(id-RESULT_FOR_UI_MAX)+
                " order by ID desc;");
#ifdef UNITTEST_DATABASE
    LOG("QueryBlockWeldResult:\n%s\n\n", str.c_str());
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/TABLE_RESULT_MEM; count++)
	    {
        strncpy(CommonProperty::WeldResultForUI[count].PartID,
            tmpStr[count*TABLE_RESULT_MEM+1].c_str(), BARCODE_DATA_SIZE);//PartID
        CommonProperty::WeldResultForUI[count].RecipeNum =
            atoi(tmpStr[count*TABLE_RESULT_MEM+3].c_str());//RecipeID
        CommonProperty::WeldResultForUI[count].TotalEnergy =
            atoi(tmpStr[count*TABLE_RESULT_MEM+4].c_str());//WeldEnergy
        CommonProperty::WeldResultForUI[count].TriggerPressure =
            atoi(tmpStr[count*TABLE_RESULT_MEM+5].c_str());//TriggerPressure
        CommonProperty::WeldResultForUI[count].WeldPressure =
            atoi(tmpStr[count*TABLE_RESULT_MEM+6].c_str());//WeldPressure
        CommonProperty::WeldResultForUI[count].Amplitude =
            atoi(tmpStr[count*TABLE_RESULT_MEM+7].c_str());//WeldAmplitude
        CommonProperty::WeldResultForUI[count].WeldTime =
            atoi(tmpStr[count*TABLE_RESULT_MEM+8].c_str());//WeldTime
        CommonProperty::WeldResultForUI[count].PeakPower =
            atoi(tmpStr[count*TABLE_RESULT_MEM+9].c_str());//WeldPeakPower
        CommonProperty::WeldResultForUI[count].PreHeight =
            atoi(tmpStr[count*TABLE_RESULT_MEM+10].c_str());//TriggerHeight
        CommonProperty::WeldResultForUI[count].PostHeight =
            atoi(tmpStr[count*TABLE_RESULT_MEM+11].c_str());//WeldHeight
        CommonProperty::WeldResultForUI[count].ALARMS.ALARMflags =
            atoi(tmpStr[count*TABLE_RESULT_MEM+12].c_str());//AlarmFlag
        CommonProperty::WeldResultForUI[count].CycleCounter =
            atoi(tmpStr[count*TABLE_RESULT_MEM+14].c_str());//CycleCounter
#ifdef UNITTEST_DATABASE
        LOG("ID: %s\n", tmpStr[count*TABLE_RESULT_MEM].c_str());
        LOG("PartID: %s\n", CommonProperty::WeldResultForUI[count].PartID);
        LOG("DateTime: %s\n", tmpStr[count*TABLE_RESULT_MEM+2].c_str());
        LOG("RecipeID: %d\n", CommonProperty::WeldResultForUI[count].RecipeNum);
        LOG("WeldTime: %d\n", CommonProperty::WeldResultForUI[count].WeldTime);
        LOG("\n");
#endif
	    }
    return count;
}

/**************************************************************************//**
* \brief   - Query Weld Signature from DB
*
* \param   - char *buffer WeldResult ID
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
        std::to_string(*(long long *)buffer)+";";
    string str = ExecuteQuery(strQuery);
    vector<WELD_SIGNATURE> WeldSignVector;
    String2Vector(str, &WeldSignVector);
#ifdef UNITTEST_DATABASE
    for(int i=0;i<WeldSignVector.size();i++)
	{
    	LOG("\tOrder(%d): Frquency %d, Power %d, Height %d, Amplitude %d\n",
            i,
            WeldSignVector[i].Frquency,
            WeldSignVector[i].Power,
            WeldSignVector[i].Height,
            WeldSignVector[i].Amplitude);
	}
#endif

    return;
}

/**************************************************************************//**
* \brief   - Query all Weld Recipe records from DB
*
* \param   - char *buffer - not used
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

#ifdef UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe all:\n%s\n", str.c_str());
#endif
    return;
}

/**************************************************************************//**
* \brief   - Query Specific Weld Recipe from DB
*
* \param   - char *buffer - WeldRecipe structure
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldRecipe(char *buffer)
{
	WeldRecipeSC *pRecipe = (WeldRecipeSC *)buffer;
	
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(pRecipe->m_RecipeID)+";";
    string str = ExecuteQuery(strQuery);
#ifdef UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());
#endif

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(pRecipe->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    if(str.empty()!=true)
    {
		WeldStepValueSetting energy[STEP_MAX];
		vector<WeldStepValueSetting> vectorEnergy;
		String2Vector(str, &vectorEnergy);
		Vector2Array(&vectorEnergy, energy);
#ifdef UNITTEST_DATABASE
		LOG("\nEnergyToStep\n");
		for(int i=0; i<STEP_MAX; i++)
		{
			LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
					energy[i].m_Order, energy[i].m_StepValue, energy[i].m_AmplitudeValue);
		}
#endif
	}

    strQuery =
        "select TimeToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(pRecipe->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    if(str.empty()!=true)
	{
		WeldStepValueSetting timeStep[STEP_MAX];
		vector<WeldStepValueSetting> vectorTime;
		String2Vector(str, &vectorTime);
		Vector2Array(&vectorTime, timeStep);
#ifdef UNITTEST_DATABASE
		LOG("\nTimeToStep\n");
		for(int i=0; i<STEP_MAX; i++)
        {
			LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
					timeStep[i].m_Order, timeStep[i].m_StepValue, timeStep[i].m_AmplitudeValue);
        }
#endif
	}

    strQuery =
        "select PowerToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(pRecipe->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    if(str.empty()!=true)
	{
		WeldStepValueSetting power[STEP_MAX];
		vector<WeldStepValueSetting> vectorPower;
		String2Vector(str, &vectorPower);
		Vector2Array(&vectorPower, power);
#ifdef UNITTEST_DATABASE
		LOG("\nPowerToStep\n");
		for(int i=0; i<STEP_MAX; i++)
        {
			LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
					power[i].m_Order, power[i].m_StepValue, power[i].m_AmplitudeValue);
        }
#endif
	}
    return;
}

/**************************************************************************//**
* \brief   - Query AlarmLog from DB
*
* \param   - char *buffer WeldResult ID
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryAlarmLog(char *buffer)
{
    string str = ExecuteQuery(
                "select * from "+string(TABLE_ALARM_LOG)+
                " where WeldResultID="+
                std::to_string(*(long long *)buffer)+";");

#ifdef UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryAlarmLog:\n%s\n", str.c_str());
#endif
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
    string strStore;
    WeldRecipeSC *pRecipe = (WeldRecipeSC *)buffer;
	int nErrCode = SQLITE_ERROR;

	strStore = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RECIPE)+
            " where ID="+std::to_string(pRecipe->m_RecipeID)+";");
	if(strStore.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %u doesn't exist in table WeldRecipe\n", pRecipe->m_RecipeID, 0, 0);
    	return nErrCode;
	}

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    string strEnergy, strTime, strPower;
    vector<WeldStepValueSetting> vectorEnergy, vectorTime, vectorPower;
    Array2Vector(pRecipe->m_WeldParameter.m_EnergyStep, &vectorEnergy);
    Array2Vector(pRecipe->m_WeldParameter.m_TimeStep, &vectorTime);
    Array2Vector(pRecipe->m_WeldParameter.m_PowerStep, &vectorPower);
    
    Vector2String(&vectorEnergy, strEnergy);
    Vector2String(&vectorTime, strTime);
    Vector2String(&vectorPower, strPower);

	strStore =
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
        ", EnergyToStep='" 			+ strEnergy+//EnergyToStep
        "', TimeToStep='" 			+ strTime+//TimeToStep
        "', PowerToStep='" 			+ strPower+//PowerToStep
        "', RecipeName='" 			+ pRecipe->m_RecipeName+//RecipeName
        "', DateTime='" 			+ timeBuf+//DateTime
        "', PresetPicPath='" 		+ pRecipe->m_RecipePicPath+//PresetPicPath
        "' where ID=" 				+ std::to_string(pRecipe->m_RecipeID)+";";
        strEnergy.shrink_to_fit();
        strTime.shrink_to_fit();
        strPower.shrink_to_fit();

	nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    LOG("# update WeldRecipe: result %d\n", nErrCode);
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Delete table, please confirm what's the purpose of the function.
*
* \param   - char *table - table name
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

/**************************************************************************//**
* \brief   - Latest ID getting for the specific table.
*
* \param   - table name, _id pointer
*
* \return  - If there is not any error happened while querying, it will return SQLITE_OK; 
* 			 else return ERROR or database status.
*
******************************************************************************/
int DBAccessL20DB::getLatestID(const string table, int* _id)
{
	int errorCode = SQLITE_OK;
	if(table.empty() == true)
		return ERROR;
	if(_id == nullptr)
		return ERROR;
    string strQuery =
        "select seq from sqlite_sequence where name='" +
        table +
        "';";
    string str = ExecuteQuery(strQuery, &errorCode);
    if(errorCode == SQLITE_OK)
    	*_id = atoi(str.c_str());
    return errorCode;
}


/**************************************************************************//**
* \brief   - The struct2Json shall be overload function for WeldStepValueSetting.
* 			 The format of JSON shall be 
* 			 {"Order1": [ModeValue1, Amplitude1], 
* 			  "Order2": [ModeValue2, Amplitude2],
* 			  "Order3": [ModeValue3, Amplitude3]
* 			 }                             
*
* \param   - WeldStepValueSetting, size and output JSONString
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int	DBAccessL20DB::Struct2JSON(const WeldStepValueSetting* _ptrArray, const unsigned int size, string& jsonStr)
{
    char *result;
    if((_ptrArray == nullptr) || (size == 0) || (size > STEP_MAX))
	{
        return ERROR;
	}

    json_t *jsonArray[size];
    for(int i = 0; i < size; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t *all  = json_object();

    for(UINT32 i = 0; i < size; i++)
	{
        json_array_append_new(jsonArray[i], json_integer(_ptrArray[i].m_StepValue));
        json_array_append_new(jsonArray[i],	json_integer(_ptrArray[i].m_AmplitudeValue));
        json_object_set_new(all, std::to_string(i).c_str(), jsonArray[i]);
	}
    
    result = json_dumps(all, 0);

    json_delete(all);
    jsonStr = result;
    return OK;
}

/**************************************************************************//**
* \brief   - The json2struct shall be overload function for WeldStepValueSetting.
* 			 The format of JSON shall be 
* 			 {"Order1": [ModeValue1, Amplitude1], 
* 			  "Order2": [ModeValue2, Amplitude2],
* 			  "Order3": [ModeValue3, Amplitude3]
* 			 }                             
*
* \param   - JSONString and WeldStepValueSetting
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::JSON2Struct(const string jsonStr, WeldStepValueSetting* _ptrArray)
{
    json_t *all;
    json_t *array;
    json_t *mem;
    int index = 0;

    if(_ptrArray == nullptr)
	{
        printf("invalid _ptrArray\n");
    	return ERROR;
	}

    all = json_loads(jsonStr.c_str(), 0, NULL);
    if(all == NULL)
    {
        printf("invalid json\n");
        return ERROR;
    }
    
    for(int i = 0; i < STEP_MAX; i++)
    {
    	array = json_object_get(all, std::to_string(i).c_str());
    	index = 0;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		_ptrArray[i].m_StepValue = json_integer_value(mem);
    	}
    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		_ptrArray[i].m_AmplitudeValue = json_integer_value(mem);
    	}
    }
    
    json_delete(all);
    return OK;
}

/**************************************************************************//**
* \brief   - The vector2Json is the function for the data converting from vector to JSON.
* 			 The format of JSON shall be 
* 			 {"Order1": [Frequency1, Power1, Height1, Amplitude1], 
* 			  "Order2": [Frequency2, Power2, Height2, Amplitude2],
* 			  "Order3": [Frequency3, Power3, Height3, Amplitude3]
* 			 }                             
*
* \param   - vector<WELD_SIGNATURE> pointer, and output JSONString
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::Vector2JSON(const vector<WELD_SIGNATURE>* _ptrVector, string& jsonStr)
{
    char *result;
    if((_ptrVector->size() == 0) || (_ptrVector->size() > HMI_SIGNA_POINT_MAX))
	{
        return ERROR;
	}
    
    json_t *jsonArray[SIGNATURE_DATA_TYPE::TOTAL] = {nullptr};
    for(int i = 0; i < SIGNATURE_DATA_TYPE::TOTAL; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t *all  = json_object();

    for(int i = 0; i < _ptrVector->size(); i++)
	{
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Frquency));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Power));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Height));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Amplitude));
        json_object_set_new(all, std::to_string(i).c_str(), jsonArray[i]);
	}

    result = json_dumps(all, 0);

    json_delete(all);
    jsonStr = result;
    return OK;
}

/**************************************************************************//**
* \brief   - The vector2String is the function for the data converting from vector to String with the specific format.
* 			 The format of String shall be
* 			 {"Frequency1, Power1, Height1, Amplitude1;
* 			  "Frequency2, Power2, Height2, Amplitude2;
* 			  "Frequency3, Power3, Height3, Amplitude3;
* 			 }
*
* \param   - vector<WELD_SIGNATURE> pointer, and output String
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::Vector2String(const vector<WELD_SIGNATURE>* _ptrVector, string & str)
{
	if ((_ptrVector->size() == 0) || (_ptrVector->size() > HMI_SIGNA_POINT_MAX))
	{
		return ERROR;
	}
	WELD_SIGNATURE tmpSignature;
	int j = 0;
	for (int i = 0; i < _ptrVector->size(); i++)
	{
		tmpSignature = _ptrVector->at(0);
		for(int j = 0; j < TOTAL; j++)
		{
			switch(j)
			{
			case FRQUENCY:
				str += std::to_string(tmpSignature.Frquency) + ",";
				break;
			case POWER:
				str += std::to_string(tmpSignature.Power) + ",";
				break;
			case HEIGHT:
				str += std::to_string(tmpSignature.Height) + ",";
				break;
			case AMPLITUDE:
				str += std::to_string(tmpSignature.Amplitude) + ";";
				break;
			default:
				break;
			}
		}
	}

	return OK;
}

/**************************************************************************//**
* \brief   - The json2Vector is the function for the data converting from JSON to vector.
* 			 The format of JSON shall be 
* 			 {"Order1": [Frequency1, Power1, Height1, Amplitude1], 
* 			  "Order2": [Frequency2, Power2, Height2, Amplitude2],
* 			  "Order3": [Frequency3, Power3, Height3, Amplitude3]
* 			 }                             
*
* \param   - input JSON string output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::JSON2Vector(const string jsonStr, vector<WELD_SIGNATURE>* _ptrVector)
{
    int size = 0, index = 0;
    json_t *all, *array, *mem;
    WELD_SIGNATURE signature;

    if(jsonStr.empty() == true)
        return ERROR;
    if(_ptrVector == nullptr)
    	return ERROR;

    all = json_loads(jsonStr.c_str(), 0, NULL);
    if(all == NULL)
	{
        printf("invalid json\n");
        return ERROR;
	}

    for(int i = 0; (array = json_object_get(all, std::to_string(i).c_str()))!=NULL; i++)
    {
    	index = 0;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Frquency = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Power = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Height = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Amplitude = json_integer_value(mem);
    	}
    	
    	_ptrVector->push_back(signature);
    }
    
    json_delete(all);
    return OK;
}

/**************************************************************************//**
* \brief   - The String2Vector is the function for the data converting from string with specific format to vector.
* 			 The format of string shall be 
* 			 {"Frequency1, Power1, Height1, Amplitude1; 
* 			  "Frequency2, Power2, Height2, Amplitude2;
* 			  "Frequency3, Power3, Height3, Amplitude3;
* 			 }                             
*
* \param   - input string output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::String2Vector(const string str, vector<WELD_SIGNATURE>* _ptrVector)
{
	int iResult = ERROR;
	WELD_SIGNATURE tmpSignature;

	if (str.empty() == true)
		return ERROR;
	if (_ptrVector == nullptr)
		return ERROR;
	vector<string> tmpStringlist;
	vector<string> tmpStringData;
	if (Utility::StringToTokens(str, ';', tmpStringlist) == 0)
		return ERROR;

	for (int i = 0; i < tmpStringlist.size(); i++)
	{
    	tmpStringData.clear();
		if (Utility::StringToTokens(tmpStringlist[i], ',', tmpStringData) > 0)
		{
			for (int j = 0; j < tmpStringData.size(); j++)
			{
				switch (j)
				{
				case FRQUENCY:
					tmpSignature.Frquency = std::stoi(tmpStringData[j]);
					break;
				case POWER:
					tmpSignature.Power = std::stoi(tmpStringData[j]);
					break;
				case HEIGHT:
					tmpSignature.Height = std::stoi(tmpStringData[j]);
					break;
				case AMPLITUDE:
					tmpSignature.Amplitude = std::stoi(tmpStringData[j]);
					break;
				default:
					break;
				}
			}
			_ptrVector->push_back(tmpSignature);
		}
		else
		{
			iResult = ERROR;
			break;
		}
	}

	return OK;
}

/**************************************************************************//**
* \brief   - The vector2String is the function for the data converting from vector to String with the specific format.
* 			 The format of String shall be
* 			 {"Order1, StepValue1, Amplitude1;
* 			  "Order2, StepValue2, Amplitude2;
* 			  "Order3, StepValue3, Amplitude3;
* 			 }
*
* \param   - vector<WeldStepValueSetting> pointer, and output String
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::Vector2String(const vector<WeldStepValueSetting>* _ptrVector, string& str)
{
	if ((_ptrVector->size() == 0) || (_ptrVector->size() > HMI_SIGNA_POINT_MAX))
	{
		return ERROR;
	}
	WeldStepValueSetting tmpStepSetting;
	int j = 0;
	for (int i = 0; i < _ptrVector->size(); i++)
	{
		tmpStepSetting = _ptrVector->at(0);
		for(int j = 0; j < TOTAL; j++)
		{
			switch(j)
			{
			case Recipe::ORDER:
				str += std::to_string(tmpStepSetting.m_Order) + ",";
				break;
			case Recipe::STEPVALUE:
				str += std::to_string(tmpStepSetting.m_StepValue) + ",";
				break;
			case AMPLITUDE:
				str += std::to_string(tmpStepSetting.m_AmplitudeValue) + ";";
				break;
			default:
				break;
			}
		}
	}
	return OK;
}

/**************************************************************************//**
* \brief   - The String2Vector is the function for the data converting from string with specific format to vector.
* 			 The format of string shall be 
* 			 {"Order1, StepValue1, Amplitude1;
* 			  "Order2, StepValue2, Amplitude2;
* 			  "Order3, StepValue3, Amplitude3;
* 			 }                            
*
* \param   - input string output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::String2Vector(const string str, vector<WeldStepValueSetting>* _ptrVector)
{
	int iResult = ERROR;
	WeldStepValueSetting tmpStepSetting;

	if (str.empty() == true)
		return ERROR;
	if (_ptrVector == nullptr)
		return ERROR;
	vector<string> tmpStringlist;
	vector<string> tmpStringData;
	if (Utility::StringToTokens(str, ';', tmpStringlist) == 0)
		return ERROR;

	for (int i = 0; i < tmpStringlist.size(); i++)
	{
    	tmpStringData.clear();
		if (Utility::StringToTokens(tmpStringlist[i], ',', tmpStringData) > 0)
		{
			for (int j = 0; j < tmpStringData.size(); j++)
			{
				switch (j)
				{
				case Recipe::ORDER:
					tmpStepSetting.m_Order = std::stoi(tmpStringData[j]);
					break;
				case Recipe::STEPVALUE:
					tmpStepSetting.m_StepValue = std::stoi(tmpStringData[j]);
					break;
				case Recipe::AMPLITUDE:
					tmpStepSetting.m_AmplitudeValue = std::stoi(tmpStringData[j]);
					break;
				default:
					break;
				}
			}
			_ptrVector->push_back(tmpStepSetting);
		}
		else
		{
			iResult = ERROR;
			break;
		}
	}

	return OK;
}

/**************************************************************************//**
* \brief   - The Arrary2Vector is the function for the data type converting from array to vector so
* 			 there is the common interface (vector to string) for database blob type. 
* 			 In case of the Recipe Step mode data type changed from struct array to vector in future,
* 			 there is not any change for database.                           
*
* \param   - input struct array output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::Array2Vector(const WeldStepValueSetting* _ptrArray, vector<WeldStepValueSetting>* _ptrvector)
{
	if((_ptrArray == nullptr) || (_ptrvector == nullptr))
		return ERROR;
	for(int i = 0; i < STEP_MAX; i++)
	{
		_ptrvector->push_back(*_ptrArray);
		_ptrArray++;
	}
	return OK;
}

/**************************************************************************//**
* \brief   - The Vector2Array is the function for the data type converting from vector to array so
* 			 there is the common interface (vector to string) for database blob type. 
* 			 In case of the Recipe Step mode data type changed from struct array to vector in future,
* 			 there is not any change for database.                           
*
* \param   - input struct array output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int DBAccessL20DB::Vector2Array(const vector<WeldStepValueSetting>* _ptrvector, WeldStepValueSetting* _ptrArray)
{
	int iResult = OK;
	if((_ptrArray == nullptr) || (_ptrvector == nullptr))
		return ERROR;
	for(int i = 0; i < _ptrvector->size(); i++)
	{
		if(i < STEP_MAX)
		{
			*_ptrArray = _ptrvector->at(i);
			_ptrArray++;
		}
		else
		{
			iResult = ERROR;
			break;
		}
	}
	if(_ptrvector->size() < STEP_MAX)
	{
		for(int i = _ptrvector->size(); i < STEP_MAX; i++)
		{
			_ptrArray[i].m_Order = -1;
			_ptrArray++;
		}
	}
	return iResult;
}

/**************************************************************************//**
* \brief   - Latest ID getting for the specific table.
*
* \param   - table name, _id pointer
*
* \return  - If there is not any error happened while querying, it will return SQLITE_OK; 
* 			 else return ERROR or database status.
*
******************************************************************************/
int DBAccessL20DB::getLatestID64(const string table, long long* _id)
{
	int errorCode = SQLITE_OK;
	if(table.empty() == true)
		return ERROR;
	if(_id == nullptr)
		return ERROR;
    string strQuery =
        "select seq from sqlite_sequence where name='" +
        table +
        "';";
    string str = ExecuteQuery(strQuery, &errorCode);
    if(errorCode == SQLITE_OK)
    	*_id = atoll(str.c_str());
    return errorCode;
}

