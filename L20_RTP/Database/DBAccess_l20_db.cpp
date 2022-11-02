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
#include "../AlarmData.h"
#include "../HeightEncoder.h"
#include "../UserAuthority.h"
#include "../SystemConfiguration.h"
#include <jansson.h>
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
DBAccessL20DB::DBAccessL20DB() : sendDataNum(0)
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
string DBAccessL20DB::GetWeldResultCSVParameters(int nRowID,  int* pnStatus, int nRetryCounter)
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
string DBAccessL20DB::GetWeldSignatureCSVParameters(int nRowID, int &nBlobParamNumber, int* pnStatus, int nRetryCounter)
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
* \param   - char* buffer - not used
*
* \return  - int -status of query exec
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
    int WeldResultID;
    getLatestID(TABLE_WELD_RESULT, &WeldResultID);
	if(nErrCode == SQLITE_OK)
	{
#ifdef UNITTEST_DATABASE
        LOG("# store WeldResult to ID %u\n", WeldResultID);
#endif
        if(WeldResultID > DB_RECORDS_STORAGE_WELD_RESULT_LIMIT)
		{
        	DeleteOldest(TABLE_WELD_RESULT);
		}

#ifdef UNITTEST_DATABASE
        ALARM_DATA* pAlarm = &AlarmDataSC::AlarmData;
        pAlarm->AlarmType = 123;
        pAlarm->AlarmSubType = 456;
        pAlarm->WeldResultID = WeldResultID;
        pAlarm->WeldRecipeID = pData->RecipeNum;
        pAlarm->UserID = 789;
        LOG("\n#try to StoreAlarmLog with WeldResultID(%u) WeldRecipeID(%u)\n",
                WeldResultID, pData->RecipeNum);
#endif
        StoreAlarmLog(nullptr);

#ifdef UNITTEST_DATABASE
        LOG("\n#try to StoreWeldSignature with WeldResultID(%u)\n",
                WeldResultID);
#endif
        StoreWeldSignature((char* )&WeldResultID);
	}
	else
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %u\n",
                nErrCode, WeldResultID, 0);
	}
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing WeldSignature into DB
*
* \param   - char* buffer - WeldResultID
*
* \return  - int - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldSignature(char* buffer)
{
	string str;
    int id;
	int WeldResultID = *(int* )buffer;
	int nErrCode = SQLITE_ERROR;

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_SIGNATURE)+
            " where WeldResultID="+std::to_string(WeldResultID)+";");
	if(str.empty()!=true)
	{
		LOGERR((char*) "Database_T: WeldResultID %u already exists in table WeldResultSignature\n",
                WeldResultID, 0, 0);
    	return nErrCode;
	}

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RESULT)+
            " where ID="+std::to_string(WeldResultID)+";");
	if(str.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %u doesn't exist in table WeldResult\n",
                WeldResultID, 0, 0);
    	return nErrCode;
	}

#if UNITTEST_DATABASE
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
            std::to_string(WeldResultID)+",'"+//WeldResultID
            str+"');";//WeldGraph

        str.shrink_to_fit();
    	nErrCode = SingleTransaction(strStore);
	}

	getLatestID(TABLE_WELD_SIGNATURE, &id);

	if(nErrCode == SQLITE_OK)
	{
#if UNITTEST_DATABASE
        LOG("# store WeldSignature to ID %u\n", id);
#endif
        if(id > DB_RECORDS_STORAGE_WELD_SIGNAT_LIMIT)
		{
            DeleteOldest(TABLE_WELD_SIGNATURE);
		}
	}
	else
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %u\n", nErrCode, id, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing Weld Recipe into DB
*
* \param   - char* buffer - not used
*
* \return  - int - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldRecipe(char* buffer)
{
	cout << "305 StoreWeldRecipe" << endl;
    int id = ERROR;
    getLatestID(TABLE_WELD_RECIPE, &id);
#if UNITTEST_DATABASE
    LOG("# already %d WeldRecipe existing, try to insert new one\n", id);
#endif
    if(id >= DB_RECORDS_STORAGE_WELD_RECIPE_LIMIT)
	{
		LOGERR((char*) "Database_T: WeldRecipe reached the limit.\n", 0, 0, 0);
        return SQLITE_ERROR;
	}

    WeldStepValueSetting m_EnergyStep[STEP_MAX];
    WeldStepValueSetting m_TimeStep[STEP_MAX];
    WeldStepValueSetting m_PowerStep[STEP_MAX];
    int m_IsTeachMode = 0;
    unsigned int m_Amplitude = 0;
    int m_WidthSetting = 0;
    int m_WPpressure = 0;
    int m_TPpressure = 0;
    int m_TimeMax = 0;
    int m_TimeMin = 0;
    unsigned int m_PeakPowerMax = 0;
    unsigned int m_PeakPowerMin = 0;
    int m_PreHeightMax = 0;
	int m_PreHeightMin = 0;
	int m_HeightMax = 0;
	int m_HeightMin = 0;
	int m_WeldMode = 0;
    int m_PreBurst = 0;
    int m_HoldTime = 0;
    int m_SqueezeTime = 0;
    int m_AfterBurstDelay = 0;
    int m_AfterBurstTime = 0;
    int m_AfterBurstAmplitude = 0;
    int m_DisplayedHeightOffset = 0;
    int m_MeasuredHeightOffset = 0;
    int m_WeldStepMode = 0;
    char m_RecipePicPath[PIC_PATH_LEN];

#if UNITTEST_DATABASE
	if(Recipe::RecipeSC == nullptr)
    	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
#endif
    if(
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_STEP, m_EnergyStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_STEP, m_TimeStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::POWER_STEP, m_PowerStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::IS_TEACH_MODE, &m_IsTeachMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AMPLITUDE, &m_Amplitude)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WIDTH_SETTING, &m_WidthSetting)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WP_PRESSURE, &m_WPpressure)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &m_TPpressure)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MAX, &m_TimeMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &m_TimeMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MAX, &m_PeakPowerMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MIN, &m_PeakPowerMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX, &m_PreHeightMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN, &m_PreHeightMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MAX, &m_HeightMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MIN, &m_HeightMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WELD_MODE, &m_WeldMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_BURST, &m_PreBurst)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HOLD_TIME, &m_HoldTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::SQUEEZE_TIME, &m_SqueezeTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_DELAY, &m_AfterBurstDelay)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_TIME, &m_AfterBurstTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_AMPLITUDE, &m_AfterBurstAmplitude)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::DISPLAYED_HEIGHT_OFFSET, &m_DisplayedHeightOffset)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::MEASURED_HEIGHT_OFFSET, &m_MeasuredHeightOffset)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WELD_STEP_MODE, &m_WeldStepMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::RECIPE_PIC_PATH, m_RecipePicPath))
	{
		LOGERR((char*) "Get Error.\n", 0, 0, 0);
        return ERROR;
	}
    
	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);

    vector<WeldStepValueSetting> vectorEnergy, vectorTime, vectorPower;
    string strEnergy, strTime, strPower;
    Array2Vector(m_EnergyStep, &vectorEnergy);
    Array2Vector(m_TimeStep, &vectorTime);
    Array2Vector(m_PowerStep, &vectorPower);
    
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
        std::to_string(m_IsTeachMode)+","+//IsValidate
        std::to_string(m_Amplitude)+","+//Amplitude
        std::to_string(m_WidthSetting)+","+//Width
        std::to_string(m_WPpressure)+","+//WeldPressure
        std::to_string(m_TPpressure)+","+//TriggerPressure
        std::to_string(m_TimeMax)+","+//TimePlus
        std::to_string(m_TimeMin)+","+//TimeMinus
        std::to_string(m_PeakPowerMax)+","+//PeakPowerPlus
        std::to_string(m_PeakPowerMin)+","+//PeakPowerMinus
        std::to_string(m_PreHeightMax)+","+//TriggerHeightPlus
        std::to_string(m_PreHeightMin)+","+//TriggerHeightMinus
        std::to_string(m_HeightMax)+","+//WeldHeightPlus
        std::to_string(m_HeightMin)+","+//WeldHeightMinus
        std::to_string(m_WeldMode)+","+//WeldMode
        std::to_string(0)+","+//ModeValue
        std::to_string(m_PreBurst)+","+//PreBurst
        std::to_string(m_HoldTime)+","+//HoldTime
        std::to_string(m_SqueezeTime)+","+//SqueezeTime
        std::to_string(m_AfterBurstDelay)+","+//AfterBurstDelay
        std::to_string(m_AfterBurstTime)+","+//AfterBurstDuration
        std::to_string(m_AfterBurstAmplitude)+","+//AfterBurstAmplitude
        std::to_string(m_DisplayedHeightOffset)+","+//WeldHeight
        std::to_string(m_MeasuredHeightOffset)+","+//MeasuredHeight
        std::to_string(m_WeldStepMode)+",'"+//StepWeldMode
		strEnergy+"','"+//EnergyToStep
		strTime+"','"+//TimeToStep
		strPower+"','"+//PowerToStep
		Recipe::RecipeSC->m_RecipeName+"','"+//RecipeName
        timeBuf+"','"+//DateTime
        m_RecipePicPath+"');";//PresetPicPath
        strEnergy.shrink_to_fit();
        strTime.shrink_to_fit();
        strPower.shrink_to_fit();

	int nErrCode = SingleTransaction(strStore);
#if UNITTEST_DATABASE
    static int count=0;
    LOG("#WeldRecipe(num %d): result %d\n", count++, nErrCode);
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	Recipe::RecipeSC = nullptr;
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing AlarmLog into DB
*
* \param   - char* buffer - not used
*
* \return  - int - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreAlarmLog(char* buffer)
{
	int id;
	string str;
	int nErrCode = SQLITE_ERROR;
    ALARM_DATA* pData = &AlarmDataSC::AlarmData;

	str = ExecuteQuery(
            "select * from "+string(TABLE_ALARM_LOG)+
            " where WeldResultID="+std::to_string(pData->WeldResultID)+";");
	if(str.empty()!=true)
	{
		LOGERR((char*) "Database_T: WeldResultID %u already exists in table AlarmLog\n",
                pData->WeldResultID, 0, 0);
    	return nErrCode;
	}

	str = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RESULT)+
            " where ID="+std::to_string(pData->WeldResultID)+";");
	if(str.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %u doesn't exist in table WeldResult\n",
                pData->WeldResultID, 0, 0);
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
        std::to_string(pData->AlarmType)+","+//AlarmType
        std::to_string(pData->WeldRecipeID)+","+//RecipeID
        std::to_string(pData->WeldResultID)+","+//WeldResultID
        std::to_string(pData->UserID)+","+//UserID
        std::to_string(pData->AlarmSubType)+");";//is AlarmSubType for column IsReset?
	nErrCode = SingleTransaction(str);

	getLatestID(TABLE_ALARM_LOG, &id);
	if(nErrCode == SQLITE_OK)
	{
#ifdef UNITTEST_DATABASE
        LOG("# store AlarmLog to ID(%u) with WeldResultID(%u)\n", id, pData->WeldResultID);
#endif
        if(id > DB_RECORDS_STORAGE_WELD_RESULT_LIMIT)
		{
            DeleteOldest(TABLE_ALARM_LOG);
		}
	}
	else
    {
        LOG("StoreAlarmLog:\n%s\n", str.c_str());
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %u\n", nErrCode, id, 0);
    }
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Query the latest records from table Weld Result
*
* \param   - char* buffer - ID
*
* \return  - int - count of records
*
******************************************************************************/
int DBAccessL20DB::QueryBlockWeldResult(char* buffer)
{
    int count;
    string str;
	vector<string> tmpStr;
	int id = *(int* )buffer;

    str = ExecuteQuery(
                string("select * from ")+string(TABLE_WELD_RESULT)+
                " where ID <= "+std::to_string(id)+" and ID > "+
                std::to_string(id-RESULT_FOR_UI_MAX)+
                " order by ID desc;");
#if UNITTEST_DATABASE
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
#if UNITTEST_DATABASE
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
* \param   - char* buffer WeldResult ID
*
* \return  - N/A
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldSignature(char* buffer)
{
    string strQuery =
        "select "+string(COLUMN_GRAPHDATA)+
        " from "+string(TABLE_WELD_SIGNATURE)+
        " where WeldResultID="+
        std::to_string(*(int* )buffer)+";";
    string str = ExecuteQuery(strQuery);
    vector<WELD_SIGNATURE> WeldSignVector;
    String2Vector(str, &WeldSignVector);
#if UNITTEST_DATABASE
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
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldRecipeAll(char* buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+";";
    string str = ExecuteQuery(strQuery);

#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe all:\n%s\n", str.c_str());
#endif
    return;
}

/**************************************************************************//**
* \brief   - Query latest Weld Recipe records from DB
*
* \param   - null
*
* \return  - error code
*
******************************************************************************/
int DBAccessL20DB::QueryWeldRecipeLatestPage()
{
	cout << "637 QueryWeldRecipeLatestPage" << endl;
	sendDataNum = FIRST_SEND_DATA_NUM;
	
    string strQuery =
        "select rowid,ID,RecipeName,DateTime from "+string(TABLE_WELD_RECIPE)+
        " order by rowid DESC limit "+std::to_string(sendDataNum)+";";
    string str = ExecuteQuery(strQuery);
    
    if(!pushWeldRecipeLib(str))
    	return FALSE;

#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("649 QueryWeldRecipe:\n%s\n", str.c_str());
#endif
    return TRUE;
}

/**************************************************************************//**
* \brief   - Query next page Weld Recipe records from DB
*
* \param   - null
*
* \return  - error code
*
******************************************************************************/
int DBAccessL20DB::QueryWeldRecipeNextPage()
{
    string strQuery =
        "select rowid,ID,RecipeName,DateTime from "+string(TABLE_WELD_RECIPE)+
        " order by rowid DESC limit "+ std::to_string(sendDataNum) +", " + std::to_string(ONE_PAGE_NUM) +";";
    string str = ExecuteQuery(strQuery);
    
    if(!pushWeldRecipeLib(str))
    	return FALSE;

    sendDataNum += ONE_PAGE_NUM;
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("448 QueryWeldRecipe:\n%s\n", str.c_str());
#endif
    return TRUE;
}

/**************************************************************************//**
* \brief   - Split and pack data into vectors
*
* \param   - const string str
*
* \return  - null
*
******************************************************************************/
int DBAccessL20DB::pushWeldRecipeLib(const string str)
{
	vector<string> data;
	WeldRecipeLibForUI WLib{};
	if (!Utility::StringToTokens(str,',',data))
		return FALSE;

	for (int i = 0; i < data.size(); ++i)
	{
		if (i % 4 == 1)
			WLib.m_RecipeID = atoi(data[i].c_str());
		else if (i % 4 == 2)
			memcpy(&WLib.m_RecipeName, data[i].c_str(), RECIPE_LEN);
		else if (i % 4 == 3)
		{
			memcpy(&WLib.m_DateTime, data[i].c_str(), RECIPE_LEN);
			Recipe::WeldRecipeLibraryForUI.push_back(WLib);
		}
	}
	return TRUE;
}

/**************************************************************************//**
* \brief   - Query Specific Weld Recipe from DB
*
* \param   - char* buffer - RecipeID
*
* \return  - N/A
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryWeldRecipe(char* buffer)
{
	int RecipeID;
	memcpy(&RecipeID, buffer, sizeof(unsigned int));

	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    string str = ExecuteQuery(strQuery);
    if(str.size()==0)
        return;
#ifdef UNITTEST_DATABASE
    else
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());
#endif

	vector<string> data;
	string tmp{};
	stringstream input(str);
	while (getline(input, tmp, ','))
		data.push_back(tmp);

	unsigned int m_Amplitude 	= stoi(data.at(6));
	int m_WPpressure 			= stoi(data.at(8));
	int m_TPpressure			= stoi(data.at(9));
	int m_TimeMax				= stoi(data.at(10));
	int m_TimeMin				= stoi(data.at(11));
	unsigned int m_PeakPowerMax	= stoi(data.at(12));
	unsigned int m_PeakPowerMin	= stoi(data.at(13));
	int m_PreHeightMax			= stoi(data.at(14));
	int m_PreHeightMin			= stoi(data.at(15));
	int m_HeightMax				= stoi(data.at(16));
	int m_HeightMin				= stoi(data.at(17));
	int m_WeldMode				= stoi(data.at(18));
	
	Recipe::RecipeSC->m_RecipeID = atoi(data[0].c_str());
	cout << "641 Recipe::RecipeSC->m_RecipeID " << Recipe::RecipeSC->m_RecipeID <<endl;
    strcpy(Recipe::RecipeSC->m_RecipeName, data[1].c_str());
    Recipe::RecipeSC->Set(&m_Amplitude, WeldRecipeSC::PARALIST::AMPLITUDE);
    Recipe::RecipeSC->Set(&m_WPpressure, WeldRecipeSC::PARALIST::WP_PRESSURE);
    Recipe::RecipeSC->Set(&m_TPpressure, WeldRecipeSC::PARALIST::TP_PRESSURE);
    Recipe::RecipeSC->Set(&m_TimeMax, WeldRecipeSC::PARALIST::TIME_MAX);
    Recipe::RecipeSC->Set(&m_TimeMin, WeldRecipeSC::PARALIST::TIME_MIN);
    Recipe::RecipeSC->Set(&m_PeakPowerMax, WeldRecipeSC::PARALIST::PEAK_POWER_MAX);
    Recipe::RecipeSC->Set(&m_PeakPowerMin, WeldRecipeSC::PARALIST::PEAK_POWER_MIN);
    Recipe::RecipeSC->Set(&m_PreHeightMax, WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX);
    Recipe::RecipeSC->Set(&m_PreHeightMin, WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN);
    Recipe::RecipeSC->Set(&m_HeightMax, WeldRecipeSC::PARALIST::HEIGHT_MAX);
    Recipe::RecipeSC->Set(&m_HeightMin, WeldRecipeSC::PARALIST::HEIGHT_MIN);
    Recipe::RecipeSC->Set(&m_WeldMode, WeldRecipeSC::PARALIST::WELD_MODE);

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);

    WeldStepValueSetting energy[STEP_MAX];
    vector<WeldStepValueSetting> vectorEnergy;
    String2Vector(str, &vectorEnergy);
    Vector2Array(&vectorEnergy, energy);
#if UNITTEST_DATABASE
    LOG("\nEnergyToStep\n");
    for(int i=0; i<STEP_MAX; i++)
        {
        LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
            energy[i].m_Order, energy[i].m_StepValue, energy[i].m_AmplitudeValue);
        }
#endif

    strQuery =
        "select TimeToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);

    WeldStepValueSetting timeStep[STEP_MAX];
    vector<WeldStepValueSetting> vectorTime;
    String2Vector(str, &vectorTime);
    Vector2Array(&vectorTime, timeStep);
#if UNITTEST_DATABASE
    LOG("\nTimeToStep\n");
    for(int i=0; i<STEP_MAX; i++)
        {
        LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
            timeStep[i].m_Order, timeStep[i].m_StepValue, timeStep[i].m_AmplitudeValue);
        }
#endif

    strQuery =
        "select PowerToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);

    WeldStepValueSetting power[STEP_MAX];
    vector<WeldStepValueSetting> vectorPower;
    String2Vector(str, &vectorPower);
    Vector2Array(&vectorPower, power);
#if UNITTEST_DATABASE
    LOG("\nPowerToStep\n");
    for(int i=0; i<STEP_MAX; i++)
        {
        LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
            power[i].m_Order, power[i].m_StepValue, power[i].m_AmplitudeValue);
        }
#endif
    return;
}

/**************************************************************************//**
* \brief   - Query AlarmLog from DB
*
* \param   - char* buffer - ID
*
* \return  - int - count of records
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
int DBAccessL20DB::QueryBlockAlarmLog(char* buffer)
{
    int count;
	vector<string> tmpStr;
    int id = *(int* )buffer;

    string str = ExecuteQuery(
                "select * from "+string(TABLE_ALARM_LOG)+
                " where ID <= "+std::to_string(id)+" and ID > "+
                std::to_string(id-ALARM_LOG_MAX)+
                " order by ID desc;");
    Utility::StringToTokens(str, ',', tmpStr);

	for(count = 0; count < tmpStr.size()/TABLE_ALARMLOG_MEM; count++)
        {
        UI_ALARM_LOG tmpLog;

        bzero(tmpLog.DateTime, 20);
        strncpy(tmpLog.DateTime, tmpStr[count*TABLE_ALARMLOG_MEM+1].c_str(), 20);//DateTime
        tmpLog.AlarmType = atoi(tmpStr[count*TABLE_ALARMLOG_MEM+2].c_str());//AlarmType

        str = ExecuteQuery(
                    "select RecipeName from "+string(TABLE_WELD_RECIPE)+
                    " where ID="+ tmpStr[count*TABLE_ALARMLOG_MEM+3] +";");//RecipeID
        bzero(tmpLog.RecipeName, RECIPE_LEN);
        strncpy(tmpLog.RecipeName, str.c_str(), RECIPE_LEN);

        tmpLog.WeldCount = atoi(tmpStr[count*TABLE_ALARMLOG_MEM+4].c_str());//WeldResultID

        for(int i=0; i<AlarmDataSC::AlarmLog.size(); i++)
            {
            if(AlarmDataSC::AlarmLog[i].WeldCount == tmpLog.WeldCount)
                {
#ifdef UNITTEST_DATABASE
                LOG("erase:%u\n", tmpLog.WeldCount);
#endif
                AlarmDataSC::AlarmLog.erase(AlarmDataSC::AlarmLog.begin()+i);
                break;
                }
            }
		AlarmDataSC::AlarmLog.push_back(tmpLog);

#ifdef UNITTEST_DATABASE
        LOG("ID:%s\n", tmpStr[count*TABLE_ALARMLOG_MEM].c_str());
        LOG("WeldCount:%u\n", tmpLog.WeldCount);
        LOG("RecipeId:%d\n", atoi(tmpStr[count*TABLE_ALARMLOG_MEM+3].c_str()));
        LOG("RecipeName:%s\n", tmpLog.RecipeName);
        LOG("DateTime:%s\n", tmpLog.DateTime);
        LOG("AlarmType:%d\n\n", tmpLog.AlarmType);
#endif
        }
    return count;
}

/**************************************************************************//**
* \brief   - Query HeightCalibration from DB
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryHeightCalibration(char* buffer)
{
    string str = ExecuteQuery(
                "select * from "+string(TABLE_HI_CALIB)+";");

	vector<string> tmpStr;
    Utility::StringToTokens(str, ',', tmpStr);

    HeightEncoder::HeightCalibratedMap[atoi(tmpStr[0].c_str())].ZeroCount = atoi(tmpStr[1].c_str());

#ifdef UNITTEST_DATABASE
    LOG("HeightCalibration:%s\n", str.c_str());
    LOG("PSI:%s\n", tmpStr[0].c_str());
    LOG("Count:%s\n", tmpStr[1].c_str());
#endif
    return;
}

/**************************************************************************//**
* \brief   - Query DbVersion from DB
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::QueryDbVersion(char* buffer)
{
    string str = ExecuteQuery(
                "select * from "+string(TABLE_DB_VERSION)+";");

	vector<string> tmpStr;
    Utility::StringToTokens(str, ',', tmpStr);

	sprintf(CommonProperty::SystemInfo.version_DB,"%s.%s.%s",
            tmpStr[0].c_str(),
            tmpStr[1].c_str(),
            tmpStr[2].c_str());
#ifdef UNITTEST_DATABASE
    LOG("QueryDbVersion:%s\n", str.c_str());
    LOG("Major:%s\n", tmpStr[0].c_str());
    LOG("Minor:%s\n", tmpStr[1].c_str());
    LOG("Build:%s\n", tmpStr[2].c_str());
#endif
    return;
}

/**************************************************************************//**
* \brief   - Query PermissionLevel and Password from DB
*
* \param   - char* buffer - not used
*
* \return  - Password count
*
******************************************************************************/
int DBAccessL20DB::QueryBlockUserProfiles(char* buffer)
{
    int PermissionLevel;
    string str = ExecuteQuery("select PermissionLevel, Password from "+string(TABLE_USER_PROFILE)+";");
    int count;
	vector<string> tmpStr;
	UserAuthority::_UserProfilesSC.clear();
    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/2; count++)
	{
        UserAuthority::_UserProfilesSC->insert(
             pair<int, string>
            (atoi(tmpStr[count*2].c_str()),tmpStr[count*2+1]));
	}
#if UNITTEST_DATABASE
    map<int,string>::iterator st;
    for(st=UserAuthority::_UserProfilesSC->begin();st!=UserAuthority::_UserProfilesSC->end();st++)
    {
        LOG("PermissionLevel(%d) Password(%s)\n", st->first, st->second.c_str());
    }
#endif
    return count;
}

/**************************************************************************//**
* \brief   - Query ScreenIndex and PermissionLevel from DB
*
* \param   - char* buffer - not used
*
* \return  - ScreenIndex count
*
******************************************************************************/
int DBAccessL20DB::QueryBlockPrivilegeConfiguration(char* buffer)
{
	UserAuthority::_UserPrivilegesSC->clear();
    int ScreenIndex;
    string str = ExecuteQuery("select ScreenIndex,PermissionLevel from "+string(TABLE_PRIVILEGE_CONFIG)+";");
    int count;
	vector<string> tmpStr;
    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/2; count++)
	{
        UserAuthority::_UserPrivilegesSC->insert(
             pair<int, int>
            (atoi(tmpStr[count*2].c_str()),atoi(tmpStr[count*2+1].c_str())));
	}
#if UNITTEST_DATABASE
    map<int,int>::iterator st;
    for(st=UserAuthority::_UserPrivilegesSC->begin();st!=UserAuthority::_UserPrivilegesSC->end();st++)
    {
        LOG("ScreenIndex(%d) PermissionLevel(%d)\n", st->first, st->second);
    }
#endif
    return count;
}

/**************************************************************************//**
* \brief   - Query all records from table PowerSupply
*
* \param   - char* buffer - not used
*
* \return  - int - count of records
*
******************************************************************************/
int DBAccessL20DB::QueryBlockPowerSupply(char* buffer)
{
    int count;
    string str;
	vector<string> tmpStr;

    str = ExecuteQuery(string("select * from ")+string(TABLE_PWR_SUPPLY)+";");
#if UNITTEST_DATABASE
    LOG("QueryBlockPowerSupply:\n%s\n\n", str.c_str());
    SystemConfiguration::PowerSupplyType.clear();
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/TABLE_RESULT_MEM; count++)
	    {
	    POWER_SUPPLY_TYPE tmpPwr;
        tmpPwr.Frequency = atoi(tmpStr[count*TABLE_RESULT_MEM+1].c_str());//Frequency
        tmpPwr.Power = atoi(tmpStr[count*TABLE_RESULT_MEM+2].c_str());//Power
		SystemConfiguration::PowerSupplyType.push_back(tmpPwr);

#if UNITTEST_DATABASE
        LOG("ID: %s\n", tmpStr[count*TABLE_RESULT_MEM].c_str());
        LOG("Frequency: %d\n", tmpPwr.Frequency);
        LOG("Power: %d\n", tmpPwr.Power);
        LOG("\n");
#endif
	    }
    return count;
}

/**************************************************************************//**
* \brief   - Query all records from table TeachModeSetting
*
* \param   - char* buffer - not used
*
* \return  - int - count of records
*
******************************************************************************/
int DBAccessL20DB::QueryBlockTeachModeSetting(char* buffer)
{
    int count;
    string str;
	vector<string> tmpStr;

    str = ExecuteQuery(string("select * from ")+string(TABLE_TEACH_MODE_SET)+";");
#if UNITTEST_DATABASE
    LOG("QueryBlockTeachModeSetting:\n%s\n\n", str.c_str());
    SystemConfiguration::TeachModeSetting.clear();
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/TABLE_TEACH_MODE_MEM; count++)
	    {
	    TEACH_MODE_SETTING tmpTeach;
        tmpTeach.TeachMode = (TEACHMODE_TYPE)atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+1].c_str());//TeachModeType
        tmpTeach.TimeRangePL = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+2].c_str());//TimePLRG
        tmpTeach.TimeRangeMS = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+3].c_str());//TimeMSRG
        tmpTeach.PowerRangePL = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+4].c_str());//PowerPLRG
        tmpTeach.PowerRangeMS = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+5].c_str());//PowerMSRG
        tmpTeach.PreHeightRangePL = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+6].c_str());//PreHeightPLRG
        tmpTeach.PreHeightRangeMS = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+7].c_str());//PreHeightMSRG
        tmpTeach.PostHeightRangePL = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+8].c_str());//HeightPLRG
        tmpTeach.PostHeightRangeMS = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+9].c_str());//HeightMSRG
        tmpTeach.Quantity = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+10].c_str());//Quantity
		SystemConfiguration::TeachModeSetting.push_back(tmpTeach);

#if UNITTEST_DATABASE
        LOG("ID: %s\n", tmpStr[count*TABLE_TEACH_MODE_MEM].c_str());
        LOG("TeachMode: %d\n", tmpTeach.TeachMode);
        LOG("TimeRangePL: %d\n", tmpTeach.TimeRangePL);
        LOG("TimeRangeMS: %d\n", tmpTeach.TimeRangeMS);
        LOG("PowerRangePL: %d\n", tmpTeach.PowerRangePL);
        LOG("PowerRangeMS: %d\n", tmpTeach.PowerRangeMS);
        LOG("PreHeightRangePL: %d\n", tmpTeach.PreHeightRangePL);
        LOG("PreHeightRangeMS: %d\n", tmpTeach.PreHeightRangeMS);
        LOG("PostHeightRangePL: %d\n", tmpTeach.PostHeightRangePL);
        LOG("PostHeightRangeMS: %d\n", tmpTeach.PostHeightRangeMS);
        LOG("Quantity: %d\n", tmpTeach.Quantity);
        LOG("\n");
#endif
	    }
    return count;
}

/**************************************************************************//**
* \brief   - Query the unique record from table SystemConfigure
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
void DBAccessL20DB::QuerySystemConfigure(char* buffer)
{
    int count;
    string str;
	vector<string> tmpStr;

    str = ExecuteQuery(string("select * from ")+string(TABLE_SYS_CONFIG)+";");
#if UNITTEST_DATABASE
    LOG("QuerySystemConfigure:\n%s\n\n", str.c_str());
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	SYSTEMCONFIGFORUI tmpSysCon;
    tmpSysCon.bFootPedalAbort      =                 atoi(tmpStr[0].c_str()); //FootPedalAbort
    tmpSysCon.bLockOnAlarm         =                 atoi(tmpStr[1].c_str()); //LockOnAlarm
    tmpSysCon.bHeightEncoder       =                 atoi(tmpStr[2].c_str()); //HeightEncoder
    tmpSysCon.Cooling              =        (COOLING)atoi(tmpStr[3].c_str()); //CoolingOption
    tmpSysCon.CoolingDuration      =                 atoi(tmpStr[4].c_str()); //CoolingDuration
    tmpSysCon.CoolingDelay         =                 atoi(tmpStr[5].c_str()); //CoolingDelay
    tmpSysCon.Language             =       (LANGUAGE)atoi(tmpStr[6].c_str()); //Language
    tmpSysCon.Amplitude_Unit       = (AMPLITUDE_UNIT)atoi(tmpStr[7].c_str()); //AmplitudeUnit
    tmpSysCon.Pressure_Unit        =  (PRESSURE_UNIT)atoi(tmpStr[8].c_str()); //PressureUnit
    tmpSysCon.Height_Unit          =    (HEIGHT_UNIT)atoi(tmpStr[9].c_str()); //HeightUnit
    tmpSysCon.MaxAmplitude         =                 atoi(tmpStr[10].c_str()); //MaxAmplitude
    tmpSysCon.TeachMode.Teach_mode = (TEACHMODE_TYPE)atoi(tmpStr[11].c_str()); //TeachModeID

	struct tm timeStamp;
	vxbRtcGet(&timeStamp);
    strftime(tmpSysCon.DateTime, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    SystemConfiguration::Set(&tmpSysCon);
    return;
}

/**************************************************************************//**
* \brief   - Update Specific Weld Recipe to DB
*
* \param   - char* buffer - not used
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateWeldRecipe(char *buffer)
{
	cout << "857 UpdateWeldRecipe" << endl;
    string strStore;
	int nErrCode = SQLITE_ERROR;

#if UNITTEST_DATABASE
	if(Recipe::RecipeSC == nullptr)
    	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC().get();
    LOG("# Recipe::RecipeSC->m_RecipeID %d\n", Recipe::RecipeSC->m_RecipeID);
    Recipe::RecipeSC->m_RecipeID = 1;
#endif
	strStore = ExecuteQuery(
            "select * from "+string(TABLE_WELD_RECIPE)+
            " where ID="+std::to_string(Recipe::RecipeSC->m_RecipeID)+";");
	if(strStore.empty()==true)
	{
		LOGERR((char*) "Database_T: ID %u doesn't exist in table WeldRecipe\n", Recipe::RecipeSC->m_RecipeID, 0, 0);
    	return nErrCode;
	}

	///////////////////////////////////////////////////////////////////////
	
    WeldStepValueSetting m_EnergyStep[STEP_MAX];
    WeldStepValueSetting m_TimeStep[STEP_MAX];
    WeldStepValueSetting m_PowerStep[STEP_MAX];
    int m_IsTeachMode = 0;
    unsigned int m_Amplitude = 0;
    int m_WidthSetting = 0;
    int m_WPressure = 0;
    int m_TPressure = 0;
    int m_TimeMax = 0;
    int m_TimeMin = 0;
    unsigned int m_PeakPowerMax = 0;
    unsigned int m_PeakPowerMin = 0;
    int m_PreHeightMax = 0;
	int m_PreHeightMin = 0;
	int m_HeightMax = 0;
	int m_HeightMin = 0;
	int m_WeldMode = 0;
    int m_PreBurst = 0;
    int m_HoldTime = 0;
    int m_SqueezeTime = 0;
    int m_AfterBurstDelay = 0;
    int m_AfterBurstTime = 0;
    int m_AfterBurstAmplitude = 0;
    int m_DisplayedHeightOffset = 0;
    int m_MeasuredHeightOffset = 0;
    int m_WeldStepMode = 0;
    char m_RecipePicPath[PIC_PATH_LEN];
    
    if(
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_STEP, m_EnergyStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_STEP, m_TimeStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::POWER_STEP, m_PowerStep)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::IS_TEACH_MODE, &m_IsTeachMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AMPLITUDE, &m_Amplitude)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WIDTH_SETTING, &m_WidthSetting)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WP_PRESSURE, &m_WPressure)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TP_PRESSURE, &m_TPressure)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MAX, &m_TimeMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &m_TimeMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MAX, &m_PeakPowerMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MIN, &m_PeakPowerMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX, &m_PreHeightMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN, &m_PreHeightMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MAX, &m_HeightMax)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MIN, &m_HeightMin)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WELD_MODE, &m_WeldMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::PRE_BURST, &m_PreBurst)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::HOLD_TIME, &m_HoldTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::SQUEEZE_TIME, &m_SqueezeTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_DELAY, &m_AfterBurstDelay)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_TIME, &m_AfterBurstTime)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::AFTER_BURST_AMPLITUDE, &m_AfterBurstAmplitude)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::DISPLAYED_HEIGHT_OFFSET, &m_DisplayedHeightOffset)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::MEASURED_HEIGHT_OFFSET, &m_MeasuredHeightOffset)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::WELD_STEP_MODE, &m_WeldStepMode)||
    !Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::RECIPE_PIC_PATH, m_RecipePicPath))
	{
		LOGERR((char*) "Get Error.\n", 0, 0, 0);
        return ERROR;
	}

	///////////////////////////////////////////////////////////////////////

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

    string strEnergy, strTime, strPower;
    vector<WeldStepValueSetting> vectorEnergy, vectorTime, vectorPower;
    Array2Vector(m_EnergyStep, &vectorEnergy);
    Array2Vector(m_TimeStep, &vectorTime);
    Array2Vector(m_PowerStep, &vectorPower);

    Vector2String(&vectorEnergy, strEnergy);
    Vector2String(&vectorTime, strTime);
    Vector2String(&vectorPower, strPower);

	strStore =
        "update " 				+ string(TABLE_WELD_RECIPE) +
        " set UserID=" 			+ std::to_string(0)+//userID
        ", IsValidate=" 		+ std::to_string(m_IsTeachMode)+//IsValidate
        ", Amplitude=" 			+ std::to_string(m_Amplitude)+//Amplitude
        ", Width=" 				+ std::to_string(m_WidthSetting)+//Width
        ", WeldPressure="	 	+ std::to_string(m_WPressure)+//WeldPressure
        ", TriggerPressure=" 	+ std::to_string(m_TPressure)+//TriggerPressure
        ", TimePlus=" 			+ std::to_string(m_TimeMax)+//TimePlus
        ", TimeMinus=" 			+ std::to_string(m_TimeMin)+//TimeMinus
        ", PeakPowerPlus=" 		+ std::to_string(m_PeakPowerMax)+//PeakPowerPlus
        ", PeakPowerMinus=" 	+ std::to_string(m_PeakPowerMin)+//PeakPowerMinus
        ", TriggerHeightPlus="	+ std::to_string(m_PreHeightMax)+//TriggerHeightPlus
        ", TriggerHeightMinus="	+ std::to_string(m_PreHeightMin)+//TriggerHeightMinus
        ", WeldHeightPlus=" 	+ std::to_string(m_HeightMax)+//WeldHeightPlus
        ", WeldHeightMinus=" 	+ std::to_string(m_HeightMin)+//WeldHeightMinus
        ", WeldMode=" 			+ std::to_string(m_WeldMode)+//WeldMode
        ", ModeValue=" 			+ std::to_string(0)+//ModeValue
        ", PreBurst=" 			+ std::to_string(m_PreBurst)+//PreBurst
        ", HoldTime=" 			+ std::to_string(m_HoldTime)+//HoldTime
        ", SqueezeTime=" 			+ std::to_string(m_SqueezeTime)+//SqueezeTime
        ", AfterBurstDelay=" 		+ std::to_string(m_AfterBurstDelay)+//AfterBurstDelay
        ", AfterBurstDuration=" 	+ std::to_string(m_AfterBurstTime)+//AfterBurstDuration
        ", AfterBurstAmplitude="	+ std::to_string(m_AfterBurstAmplitude)+//AfterBurstAmplitude
        ", WeldHeight=" 			+ std::to_string(m_DisplayedHeightOffset)+//WeldHeight
        ", MeasuredHeight=" 		+ std::to_string(m_MeasuredHeightOffset)+//MeasuredHeight
        ", StepWeldMode=" 			+ std::to_string(m_WeldStepMode)+//StepWeldMode
        ", EnergyToStep='" 			+ strEnergy+//EnergyToStep
        "', TimeToStep='" 			+ strTime+//TimeToStep
        "', PowerToStep='" 			+ strPower+//PowerToStep
        "', RecipeName='" 			+ Recipe::RecipeSC->m_RecipeName+//RecipeName
        "', DateTime='" 			+ timeBuf+//DateTime
        "', PresetPicPath='" 		+ m_RecipePicPath+//PresetPicPath
        "' where ID=" 				+ std::to_string(Recipe::RecipeSC->m_RecipeID)+";";
        strEnergy.shrink_to_fit();
        strTime.shrink_to_fit();
        strPower.shrink_to_fit();

	nErrCode = SingleTransaction(strStore);
#if UNITTEST_DATABASE
    LOG("# update WeldRecipe: result %d\n", nErrCode);
#endif
	if(nErrCode != SQLITE_OK)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	Recipe::RecipeSC = nullptr;
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Update HeightCalibration to DB
*
* \param   - char* buffer - PSI
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateHeightCalibration(char* buffer)
{
	int nErrCode = SQLITE_ERROR;
    int PSI = *(int* )buffer;

    if(1 != HeightEncoder::HeightCalibratedMap.count(PSI))
        {
#ifdef UNITTEST_DATABASE
        HeightEncoder::HeightCalibratedMap[PSI].ZeroCount = 1234;
        LOG("# set PSI(%d) with ZeroCount(%d)\n", PSI, 1234);
#else
	    return nErrCode;
#endif
        }

	string strStore =
        "update " 	+ string(TABLE_HI_CALIB) +
        " set PSI=" + std::to_string(PSI)+//PSI
        ", Count=" 	+ std::to_string(HeightEncoder::HeightCalibratedMap[PSI].ZeroCount)+//Count
        +";";

	nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    LOG("# %s\n", strStore.c_str());
#endif
	if(nErrCode != SQLITE_OK)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Update UserProfiles to DB
*
* \param   - char* buffer - PermissionLevel
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateUserProfiles(char* buffer)
{
    map<int,string>::iterator st = UserAuthority::_UserProfilesSC->find(*(int* )buffer);
    if(st == UserAuthority::_UserProfilesSC->end())
	{
	    return SQLITE_ERROR;
	}

	string strStore =
        "update " 	+ string(TABLE_USER_PROFILE) +
        " set Password='" + st->second+//Password
        "' where PermissionLevel="+ std::to_string(st->first)+";";//PermissionLevel
	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != SQLITE_OK)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Update PrivilegeConfiguration to DB
*
* \param   - char* buffer - ScreenIndex
*
* \return  - int - PermissionLevel, or negative Database status if failed
*
******************************************************************************/
int DBAccessL20DB::UpdatePrivilegeConfigure(char* buffer)
{
    map<int,int>::iterator st = UserAuthority::_UserPrivilegesSC->find(*(int* )buffer);
    if(st == UserAuthority::_UserPrivilegesSC->end())
	{
	    return SQLITE_ERROR;
	}

    string strStore = 
            "update " 	+ string(TABLE_PRIVILEGE_CONFIG) +
            " set PermissionLevel=" + std::to_string(st->second)+//PermissionLevel
            " where ScreenIndex="+std::to_string(st->first)+";";
	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != SQLITE_OK)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Update all records to PowerSupply
*
* \param   - char* buffer - not used
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateBlockPowerSupply(char* buffer)
{
	for (int i = 0; i < SystemConfiguration::PowerSupplyType.size(); i++)
	{
		POWER_SUPPLY_TYPE tmpPwr = SystemConfiguration::PowerSupplyType.at(i);
    	string strStore =
            "update " 			+ string(TABLE_PWR_SUPPLY) +
            " set Frequency=" 	+ std::to_string(tmpPwr.Frequency)+//Frequency
            ", Power=" 			+ std::to_string(tmpPwr.Power)+//Power
            " where ID=" 		+ std::to_string(i)+";";
    	int nErrCode = SingleTransaction(strStore);
    	if(nErrCode != SQLITE_OK)
        {   
    		LOGERR((char*) "Database_T: UpdateBlockPowerSupply Error. %d\n", nErrCode, 0, 0);
        	return nErrCode;
        }
	}
	return SQLITE_OK;
}

/**************************************************************************//**
* \brief   - Update all records to TeachModeSetting
*
* \param   - char* buffer - not used
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateBlockTeachModeSetting(char* buffer)
{
	for (int i = 0; i < SystemConfiguration::TeachModeSetting.size(); i++)
	{
		TEACH_MODE_SETTING tmpTeach = SystemConfiguration::TeachModeSetting.at(i);
    	string strStore =
            "update " 				+ string(TABLE_TEACH_MODE_SET) +
            " set TeachModeType=" 	+ std::to_string(tmpTeach.TeachMode)+//TeachModeType
            ", TimePLRG=" 			+ std::to_string(tmpTeach.TimeRangePL)+//TimePLRG
            ", TimeMSRG=" 			+ std::to_string(tmpTeach.TimeRangeMS)+//TimeMSRG
            ", PowerPLRG=" 			+ std::to_string(tmpTeach.PowerRangePL)+//PowerPLRG
            ", PowerMSRG=" 			+ std::to_string(tmpTeach.PowerRangeMS)+//PowerMSRG
            ", PreHeightPLRG=" 		+ std::to_string(tmpTeach.PreHeightRangePL)+//PreHeightPLRG
            ", PreHeightMSRG=" 		+ std::to_string(tmpTeach.PreHeightRangeMS)+//PreHeightMSRG
            ", HeightPLRG=" 		+ std::to_string(tmpTeach.PostHeightRangePL)+//HeightPLRG
            ", HeightMSRG=" 		+ std::to_string(tmpTeach.PostHeightRangeMS)+//HeightMSRG
            ", Quantity=" 			+ std::to_string(tmpTeach.Quantity)+//Quantity
            " where ID=" 			+ std::to_string(i)+";";
    	int nErrCode = SingleTransaction(strStore);
    	if(nErrCode != SQLITE_OK)
        {   
    		LOGERR((char*) "Database_T: UpdateBlockTeachModeSetting Error. %d\n", nErrCode, 0, 0);
        	return nErrCode;
        }
	}
	return SQLITE_OK;
}

/**************************************************************************//**
* \brief   - Update the unique record to SystemConfigure
*
* \param   - char* buffer - not used
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateSystemConfigure(char* buffer)
{
	SYSTEMCONFIGFORUI tmpSysCon;
    SystemConfiguration::Get(&tmpSysCon);

	string strStore =
        "update " 				+ string(TABLE_SYS_CONFIG) +
        " set FootPedalAbort=" 	+ std::to_string(tmpSysCon.bFootPedalAbort)+
        ", LockOnAlarm=" 		+ std::to_string(tmpSysCon.bLockOnAlarm)+
        ", HeightEncoder=" 		+ std::to_string(tmpSysCon.bHeightEncoder)+
        ", CoolingOption=" 		+ std::to_string(tmpSysCon.Cooling)+
        ", CoolingDuration=" 	+ std::to_string(tmpSysCon.CoolingDuration)+
        ", CoolingDelay=" 		+ std::to_string(tmpSysCon.CoolingDelay)+
        ", Language=" 			+ std::to_string(tmpSysCon.Language)+
        ", AmplitudeUnit=" 		+ std::to_string(tmpSysCon.Amplitude_Unit)+
        ", PressureUnit=" 		+ std::to_string(tmpSysCon.Pressure_Unit)+
        ", HeightUnit=" 		+ std::to_string(tmpSysCon.Height_Unit)+
        ", MaxAmplitude=" 		+ std::to_string(tmpSysCon.MaxAmplitude)+
        ", TeachModeID=" 		+ std::to_string(tmpSysCon.TeachMode.Teach_mode)+
        //", HomePositionCount=" 	+ std::to_string(0)+
        ";";
	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != SQLITE_OK)
    {   
		LOGERR((char*) "Database_T: UpdateSystemConfigure Error. %d\n", nErrCode, 0, 0);
    }
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Delete table, please confirm what's the purpose of the function.
*
* \param   - char* table - table name
*
* \return  - none
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
void DBAccessL20DB::DeleteOldest(const char* table)
{
    string strQuery =
        "delete from "+
        string(table)+" where ID in (select ID from "+
        string(table)+" order by ID asc limit 1);";
    SingleTransaction(strQuery);
    return;
}

/**************************************************************************//**
* \brief   - Delete Specific Weld Recipe to DB
*
* \param   - char *buffer - Recipe ID
*
* \return  - int - ErrCode
*
******************************************************************************/
int DBAccessL20DB::DeleteSpecificRecipe(const char *buffer)
{
	int Index;
	memcpy(&Index, buffer, sizeof(int));
	cout << "1035 DeleteSpecificRecipe " << Index << endl;
	string strQuery = "delete from "+string(TABLE_WELD_RECIPE)+" where ID = "+ std::to_string(Index) +";";
    int nErrCode= SingleTransaction(strQuery);
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Delete Specific Recipe Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
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
    char* result;
    if((_ptrArray == nullptr) || (size == 0) || (size > STEP_MAX))
	{
        return ERROR;
	}

    json_t* jsonArray[size];
    for(int i = 0; i < size; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t* all  = json_object();

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
    json_t* all;
    json_t* array;
    json_t* mem;
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
    char* result;
    if((_ptrVector->size() == 0) || (_ptrVector->size() > HMI_SIGNA_POINT_MAX))
	{
        return ERROR;
	}
    
    json_t* jsonArray[SIGNATURE_DATA_TYPE::TOTAL] = {nullptr};
    for(int i = 0; i < SIGNATURE_DATA_TYPE::TOTAL; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t* all  = json_object();

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
    json_t* all, *array, *mem;
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
		tmpStepSetting = _ptrVector->at(i);
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

