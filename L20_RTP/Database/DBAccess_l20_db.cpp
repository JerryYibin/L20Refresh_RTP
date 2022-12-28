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
#include "../HeightEncoder.h"
#include "../UserAuthority.h"
#include "../SystemConfiguration.h"
#include "../User.h"
#include "../WeldResultsDefine.h"
#include "../WeldResults.h"
#include "../AlarmLog.h"
#include "../AlarmEvent.h"
#include "../Connectivity.h"
#include <jansson.h>
extern "C"
{
	#include "hwif/drv/resource/vxbRtcLib.h"
}
#undef RTC_MEASURE
#ifdef RTC_MEASURE
#include <timerDev.h>
#endif

#define min(x, y)	(((x) < (y)) ? (x) : (y))

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
    unsigned int TriggerPressure 	= 0;
    unsigned int WeldPressure 		= 0;
    unsigned int PreHeight 			= 0;
    unsigned int PostHeight 		= 0;
    int AlarmID 					= 0;
    int TimePlus					= 0;
    int TimeMinus					= 0;
    int PeakPowerPlus				= 0;
    int PeakPowerMinus				= 0;
    int TriggerHeightPlus			= 0;
    int TriggerHeightMinus			= 0;
    int WeldHeightPlus				= 0;
    int WeldHeightMinus				= 0;
    int WeldMode					= 0;
    int ModeValueSetting			= 0;
    WeldResults::_WeldResults->Get(WeldResults::TRIGGER_PRESSURE, &TriggerPressure);
    WeldResults::_WeldResults->Get(WeldResults::WELD_PRESSURE, &WeldPressure);
    WeldResults::_WeldResults->Get(WeldResults::PRE_HEIGHT, &PreHeight);
    WeldResults::_WeldResults->Get(WeldResults::POST_HEIGHT, &PostHeight);
    WeldResults::_WeldResults->Get(WeldResults::ALARM_ID, &AlarmID);
    
    
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MAX, &TimePlus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::TIME_MIN, &TimeMinus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MAX, &PeakPowerPlus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PEAK_POWER_MIN, &PeakPowerMinus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX, &TriggerHeightPlus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN, &TriggerHeightMinus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MAX, &WeldHeightPlus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::HEIGHT_MIN, &WeldHeightMinus);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::WELD_MODE, &WeldMode);
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_SETTING, &ModeValueSetting);

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
    strftime(timeBuf, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);

	string strStore =
        "insert into " + string(TABLE_WELD_RESULT) +
        " (PartID, DateTime, RecipeName, WeldEnergy, TriggerPressure, WeldPressure, " +
		"WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, " +
		"AlarmFlag, SequenceID, CycleCounter, TimePlus, TimeMinus, PeakPowerPlus, " + 
		"PeakPowerMinus, TriggerHeightPlus, TriggerHeightMinus, WeldHeightPlus, " + 
		"WeldHeightMinus, WeldMode, ModeValueSetting) " +
		"values ('"+
		WeldResults::_WeldResults->PartID+"','"+//PartID
		timeBuf+"','"+//DateTime
		Recipe::ActiveRecipeSC->m_RecipeName/*pData->RecipeNumber*/+"',"+//RecipeName
		std::to_string(WeldResults::_WeldResults->Energy)+","+//WeldEnergy
		std::to_string(TriggerPressure)+","+//TriggerPressure
		std::to_string(WeldPressure)+","+//WeldPressure
		std::to_string(WeldResults::_WeldResults->Amplitude)+","+//WeldAmplitude
		std::to_string(WeldResults::_WeldResults->WeldTime)+","+//WeldTime
		std::to_string(WeldResults::_WeldResults->PeakPower)+","+//WeldPeakPower
		std::to_string(PreHeight)+","+//TriggerHeight
		std::to_string(PostHeight)+","+//WeldHeight
		std::to_string(AlarmID)+","+//AlarmFlag
		std::to_string(0)+","+//SequenceID
		std::to_string(WeldResults::_WeldResults->CycleCounter)+","+//CycleCounter
		std::to_string(TimePlus)+","+//TimePlus
		std::to_string(TimeMinus)+","+
		std::to_string(PeakPowerPlus)+","+
		std::to_string(PeakPowerMinus)+","+
		std::to_string(TriggerHeightPlus)+","+
		std::to_string(TriggerHeightMinus)+","+
		std::to_string(WeldHeightPlus)+","+
		std::to_string(WeldHeightMinus)+","+
		std::to_string(WeldMode)+","+
		std::to_string(ModeValueSetting)+");";//ModeValueSetting

	int nErrCode = SingleTransaction(strStore);
    int id = ERROR;
    getLatestID(TABLE_WELD_RESULT, &id);
    if(id != ERROR)
    	WeldResults::_WeldResults->WeldResultID = id;

	if(nErrCode == 0)
	{
#if UNITTEST_DATABASE
        if((id%10000)==0)
		{
        	LOG("# store WeldResult to ID %u\n", id);
        	LOG("# %s\n", strStore.c_str());
		}
#endif
        if(id > DB_RECORDS_STORAGE_WELD_RESULT_LIMIT)
		{
            LOG("# DeleteOldest WeldResult ID %u\n", id);
        	DeleteOldest(TABLE_WELD_RESULT);
		}
	}
	else
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %u\n", nErrCode, id, 0);
	}
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing WeldSignature into DB.
* 			 The function shall much task the Message Queue mechanism to do weld signature saving, 
* 			 because the WeldResultID is the foreign key in WeldSignature table what means 
* 			 the WeldResult ID must be ready firstly in WeldResult Table. 
* 			 So the WeldResult save Message should be sent out before WeldSignature message.
* 			 So the WeldResultID should be updated in the function only.   
*
* \param   - char* buffer - No use
*
* \return  - int - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldSignature(char* buffer)
{
	string str;
    int id;
	int WeldResultID = WeldResults::_WeldResults->WeldResultID;
	int nErrCode = SQLITE_ERROR;

#if UNITTEST_DATABASE
    WeldResultID = *(int *)buffer;
#endif

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
	if(WeldResultSignature::_OrignalSignature->size()==0)
    {
        LOG("# push_back _OrignalSignature\n");
    	shared_ptr<WeldResultSignatureDefine> _Signature = WeldResultSignatureDefine::GetWeldSignature();
		_Signature->Set(HEIGHT, 1);
		_Signature->Set(FRQUENCY, 2);
		_Signature->Set(AMPLITUDE, 3);
		_Signature->Set(POWER, 4);
		WeldResultSignature::_OrignalSignature->push_back(_Signature);
    }
#endif
	str.clear();
	Vector2String(WeldResultSignature::_OrignalSignature, str);
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
        if((id%1000)==0)
            LOG("# store WeldSignature to ID %u\n", id);
#endif
        if(id > DB_RECORDS_STORAGE_WELD_SIGNAT_LIMIT)
		{
            LOG("# DeleteOldest WeldSignature ID %u\n", id);
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
#if UNITTEST_DATABASE
	if(Recipe::RecipeSC == nullptr)
		Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
    snprintf(Recipe::RecipeSC->m_RecipeName, RECIPE_LEN, "%s", buffer);
#endif
	string str = ExecuteQuery("select RecipeName from "+string(TABLE_WELD_RECIPE)+
                    " where RecipeName='"+Recipe::RecipeSC->m_RecipeName+"';");
	if(!str.empty())
	{
		LOGERR((char*) "Database_T: '%s' already exists in table WeldRecipe\n",
                (int)Recipe::RecipeSC->m_RecipeName, 0, 0);
    	return ERROR;
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
	unsigned int m_EnergySetting = 0;
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
	!Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_SETTING, &m_EnergySetting)||
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
		//Recipe::RecipeSC = nullptr;
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
        std::to_string(m_EnergySetting)+","+//ModeValue		//TODO It should be changed
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
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
		return ERROR;
	}

	// delete oldest record when record reaches 1000
    int id = ERROR;
    getLatestID(TABLE_WELD_RECIPE, &id);
#if UNITTEST_DATABASE
    LOG("# store WeldRecipe to ID(%u)\n", id);
#endif
	if(id >= DB_RECORDS_STORAGE_WELD_RECIPE_LIMIT)
	{
        DeleteOldest(TABLE_WELD_RECIPE);
	}
	return OK;
}

/**************************************************************************//**
* \brief   - Writing AlarmLog into DB
*
* \param   - char *buffer - Alarm Event object
*
* \return  - int - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreAlarmLog(char* buffer)
{
	string str;
	int id = 0;
	int nErrCode = SQLITE_ERROR;
	AlarmEvent event;
	memcpy((void*)&event, buffer, sizeof(AlarmEvent));
	if(event.m_AlarmActions.CYCLE_COUNTER == true)
		event.m_WeldResultID = WeldResults::_WeldResults->WeldResultID;

	str =
        "insert into " + string(TABLE_ALARM_LOG) +
        " (DateTime, AlarmType, RecipeID, WeldResultID, UserID, IsReset) " +
        "values ('"+
        event.m_strTimeStamp +"',"+//DateTime
        std::to_string(event.m_Type)+","+//AlarmType
        std::to_string(event.m_WeldRecipeID)+","+//RecipeID
        std::to_string(event.m_WeldResultID)+","+//WeldResultID
        std::to_string(-1)+","+//UserID
        std::to_string(1)+");";//IsReset
	nErrCode = SingleTransaction(str);

	getLatestID(TABLE_ALARM_LOG, &id);
	if(nErrCode == SQLITE_OK)
	{
#if UNITTEST_DATABASE
        if((id%10000)==0)
		{
            LOG("# store AlarmLog to ID(%u) with WeldResultID(%u)\n", id, event.m_WeldResultID);
		}
#endif
        if(id > DB_RECORDS_STORAGE_WELD_RESULT_LIMIT)
		{
            LOG("# DeleteOldest AlarmLog ID %u\n", id);
            DeleteOldest(TABLE_ALARM_LOG);
		}
	}
	else
    {
#if UNITTEST_DATABASE
        LOG("StoreAlarmLog:\n%s\n", str.c_str());
#endif
		LOGERR((char*) "Database_T: Single Transaction Error. %d after ID %u\n", nErrCode, id, 0);
    }
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Query latest Weld Result records from DB
*
* \param   - char* buffer - not used
*
* \return  - error code
*
******************************************************************************/
int DBAccessL20DB::QueryWeldResultLatestPage(char* buffer)
{
	sendDataNum = ONE_RESULT_PAGE_NUM;
	string strQuery = "select * from " + string(TABLE_WELD_RESULT)
			+ " order by ID DESC limit " + std::to_string(sendDataNum)
			+ ";";
	string str = ExecuteQuery(strQuery);
	if (str.empty()) {
		LOGERR((char*) "Weld Result is null\n", 0, 0, 0);
		return ERROR;
	}
#if UNITTEST_DATABASE
    LOG("QueryWeldResultLatestPage:\n%s\n", str.c_str());
#endif
	assignWeldResult(str);

	return OK;
}

int DBAccessL20DB::QueryWeldResultNextPage(char* buffer)
{
	string strQuery = "select * from " + string(TABLE_WELD_RESULT)
			+ " order by ID DESC limit " + std::to_string(sendDataNum)
			+ ", " + std::to_string(ONE_RESULT_PAGE_NUM) + ";";
	string str = ExecuteQuery(strQuery);
	if (str.empty()) {
		LOGERR((char*) "Weld Result is null\n", 0, 0, 0);
		return ERROR;
	}
#if UNITTEST_DATABASE
    LOG("QueryWeldResultNextPage:\n%s\n", str.c_str());
#endif
	assignWeldResult(str);

	sendDataNum += ONE_RESULT_PAGE_NUM;

	return OK;
}

/**************************************************************************//**
* \brief   - Decompose and store the data in the database into Vector
*
* \param   - buffer(data read from the database)
*
* \return  - null
*
******************************************************************************/
void DBAccessL20DB::assignWeldResult(const string& buffer)
{
	vector<string> tmpStr;
    Utility::StringToTokens(buffer, ',', tmpStr);
	for(int count = 0; count < tmpStr.size()/TABLE_RESULT_MEM; count++)
	{
		shared_ptr<WeldResults> ptr_WeldResults = WeldResults::GetWeldResults();
		ptr_WeldResults->WeldResultID   = atoi(tmpStr[count*TABLE_RESULT_MEM].c_str()); 
		strncpy(ptr_WeldResults->PartID, tmpStr[count*TABLE_RESULT_MEM+1].c_str(), BARCODE_DATA_SIZE);	//PartID
		strncpy(ptr_WeldResults->DateTime, tmpStr[count*TABLE_RESULT_MEM+2].c_str(), WELD_TIME_SIZE);  	//DateTime 	
	    strncpy(ptr_WeldResults->RecipeName, tmpStr[count*TABLE_RESULT_MEM+3].c_str(), USER_NAME_SIZE);	//RecipeName
	    
		int m_TriggerPressure 			= atoi(tmpStr[count*TABLE_RESULT_MEM+5].c_str());
		int m_WeldPressure 				= atoi(tmpStr[count*TABLE_RESULT_MEM+6].c_str());	
    	int m_TPpressureSetting 		= atoi(tmpStr[count*TABLE_RESULT_MEM+5].c_str());
    	int m_WPpressureSetting			= atoi(tmpStr[count*TABLE_RESULT_MEM+6].c_str());
    	unsigned int m_AmplitudeSetting = atoi(tmpStr[count*TABLE_RESULT_MEM+7].c_str());
		int m_PreHeight 				= atoi(tmpStr[count*TABLE_RESULT_MEM+10].c_str());
		int m_PostHeight 				= atoi(tmpStr[count*TABLE_RESULT_MEM+11].c_str());
		int m_AlarmFlag 				= atoi(tmpStr[count*TABLE_RESULT_MEM+12].c_str());
    	int m_TimeMax 					= atoi(tmpStr[count*TABLE_RESULT_MEM+15].c_str());
    	int m_TimeMin 					= atoi(tmpStr[count*TABLE_RESULT_MEM+16].c_str());
    	unsigned int m_PeakPowerMax		= atoi(tmpStr[count*TABLE_RESULT_MEM+17].c_str());
    	unsigned int m_PeakPowerMin		= atoi(tmpStr[count*TABLE_RESULT_MEM+18].c_str());
    	int m_PreHeightMax				= atoi(tmpStr[count*TABLE_RESULT_MEM+19].c_str());
    	int m_PreHeightMin				= atoi(tmpStr[count*TABLE_RESULT_MEM+20].c_str());
    	int m_HeightMax					= atoi(tmpStr[count*TABLE_RESULT_MEM+21].c_str());
    	int m_HeightMin 				= atoi(tmpStr[count*TABLE_RESULT_MEM+22].c_str());
    	ptr_WeldResults->WeldMode 		= atoi(tmpStr[count*TABLE_RESULT_MEM+23].c_str());
    	unsigned int m_EnergySetting 	= atoi(tmpStr[count*TABLE_RESULT_MEM+24].c_str());
	    
		ptr_WeldResults->Energy 		= atoi(tmpStr[count*TABLE_RESULT_MEM+4].c_str());		//WeldEnergy
		ptr_WeldResults->Amplitude 		= atoi(tmpStr[count*TABLE_RESULT_MEM+7].c_str());		//WeldAmplitude
		ptr_WeldResults->WeldTime 		= atoi(tmpStr[count*TABLE_RESULT_MEM+8].c_str());		//WeldTime
		ptr_WeldResults->PeakPower 		= atoi(tmpStr[count*TABLE_RESULT_MEM+9].c_str());		//WeldPeakPower
		ptr_WeldResults->CycleCounter 	= atoi(tmpStr[count*TABLE_RESULT_MEM+14].c_str());		//CycleCounter

		ptr_WeldResults->Set(WeldResults::PARALIST::TRIGGER_PRESSURE, 	&m_TriggerPressure);	//TriggerPressure
		ptr_WeldResults->Set(WeldResults::PARALIST::WELD_PRESSURE, 		&m_WeldPressure);		//WeldPressure
		ptr_WeldResults->Set(WeldResults::PARALIST::PRE_HEIGHT, 		&m_PreHeight);			//TriggerHeight
		ptr_WeldResults->Set(WeldResults::PARALIST::POST_HEIGHT, 		&m_PostHeight);			//WeldHeight
		ptr_WeldResults->Set(WeldResults::PARALIST::ALARM_ID, 			&m_AlarmFlag);			//AlarmID
    	ptr_WeldResults->Set(WeldResults::PARALIST::AMPLITUDE_SETTING, 	&m_AmplitudeSetting);	//AmplitudeSetting
    	ptr_WeldResults->Set(WeldResults::PARALIST::W_PRESSURE_SETTING, &m_WPpressureSetting);	//WPpressureSetting
    	ptr_WeldResults->Set(WeldResults::PARALIST::T_PRESSURE_SETTING, &m_TPpressureSetting);	//TPpressureSetting
    	ptr_WeldResults->Set(WeldResults::PARALIST::MAX_WELD_TIME, 		&m_TimeMax);			//TimeMax
    	ptr_WeldResults->Set(WeldResults::PARALIST::MIN_WELD_TIME, 		&m_TimeMin);			//TimeMin
    	ptr_WeldResults->Set(WeldResults::PARALIST::MAX_POWER, 			&m_PeakPowerMax);		//PeakPowerMax
    	ptr_WeldResults->Set(WeldResults::PARALIST::MIN_POWER, 			&m_PeakPowerMin);		//PeakPowerMin
    	ptr_WeldResults->Set(WeldResults::PARALIST::MAX_PRE_HEIGHT, 	&m_PreHeightMax);		//PreHeightMax
    	ptr_WeldResults->Set(WeldResults::PARALIST::MIN_PRE_HEIGHT, 	&m_PreHeightMin);		//PreHeightMin
    	ptr_WeldResults->Set(WeldResults::PARALIST::MAX_POST_HEIGHT, 	&m_HeightMax);			//HeightMax
    	ptr_WeldResults->Set(WeldResults::PARALIST::MIN_POST_HEIGHT, 	&m_HeightMin);			//HeightMin
    	ptr_WeldResults->Set(WeldResults::PARALIST::ENERGY_SETTING, 	&m_EnergySetting);  	//EnergySetting
		WeldResult::WeldResultVector.push_back(ptr_WeldResults);
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("assignWeldResult:\n%s\n", str.c_str());
#endif
	}
	tmpStr.clear();
}

/**************************************************************************//**
* \brief   - Query the latest records from table Weld Result(Out of use)
*
* \param   - char* buffer - ID
*
* \return  - int - count of records
*
******************************************************************************/
int DBAccessL20DB::QueryLatestWeldResult(char* buffer)
{
	vector<string> tmpStr;
	int errorCode = 0;
    string str = ExecuteQuery("select ID,CycleCounter from "+string(TABLE_WELD_RESULT)+
                " where ID = (select max(ID) from "+string(TABLE_WELD_RESULT)+
				") limit 1;", &errorCode);
    if(str.empty())
    {
    	CommonProperty::SystemInfo.psLifeCounter = 0;
    	WeldResults::_WeldResults->CycleCounter = 0;
    	if(errorCode != SQLITE_OK)
    		return ERROR;
    }
	else {
		Utility::StringToTokens(str, ',', tmpStr);
		CommonProperty::SystemInfo.psLifeCounter = atoi(tmpStr[1].c_str());
		WeldResults::_WeldResults->CycleCounter = atoi(tmpStr[2].c_str());
	}
#if UNITTEST_DATABASE
    LOG("QueryLatestWeldResult:\n%s\n\n", str.c_str());
#endif
    return OK;
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
int DBAccessL20DB::QueryWeldSignature(char* buffer)
{
    string strQuery =
        "select "+string(COLUMN_GRAPHDATA)+
        " from "+string(TABLE_WELD_SIGNATURE)+
        " where WeldResultID="+
        std::to_string(*(int* )buffer)+";";
    string str = ExecuteQuery(strQuery);
     if(str.empty())
        {
#if UNITTEST_DATABASE
        LOG("\tWeldResultID %d not found from WeldResultSignature\n", (*(int* )buffer));
#endif
        return ERROR;
        }

    WeldResultSignature::_OrignalSignature->clear();
    String2Vector(str, WeldResultSignature::_OrignalSignature);

#if UNITTEST_DATABASE
	shared_ptr<WeldResultSignatureDefine> _Signature;
	int iError = OK;
	int iResult = 0;
    LOG("_OrignalSignature size %d\n", WeldResultSignature::_OrignalSignature->size());
    for(int i=0;i<WeldResultSignature::_OrignalSignature->size();i++)
	{
		_Signature = WeldResultSignature::_OrignalSignature->at(i);
		for(int j = 0; j < WeldResultSignatureDefine::TOTAL; j++)
		{
			iResult = _Signature->Get(j, &iError);
			if(iError == OK)
    	        LOG("\t%d\n",iResult);
		}
	}
#endif

    return OK;
}

/**************************************************************************//**
* \brief   - Query Weld Recipe total number from DB
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
int DBAccessL20DB::QueryWeldRecipeTotalNumber(char* buffer)
{
    string strQuery =
        "select count(*) from "+ string(TABLE_WELD_RECIPE);
    string str = ExecuteQuery(strQuery);
    if(str.size() == 0)
    {
    	return ERROR;
    }
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("Query WeldRecipe Total Number:\n%s\n", str.c_str());
#endif
	return atoi(str.c_str());
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
int DBAccessL20DB::QueryWeldRecipeAll(char* buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+";";
    string str = ExecuteQuery(strQuery);

#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe all:\n%s\n", str.c_str());
#endif
    return OK;
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
	sendDataNum = FIRST_SEND_DATA_NUM;
	
    string strQuery =
        "select rowid,ID,RecipeName,DateTime from "+string(TABLE_WELD_RECIPE)+
        " order by DateTime DESC limit "+std::to_string(sendDataNum)+";";
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
        " order by DateTime DESC limit "+ std::to_string(sendDataNum) +", " + std::to_string(ONE_PAGE_NUM) +";";
    string str = ExecuteQuery(strQuery);
    
    if(!pushWeldRecipeLib(str))
    	return FALSE;

    sendDataNum += ONE_PAGE_NUM;
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("827 QueryWeldRecipe:\n%s\n", str.c_str());
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
* \return  - query result
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
int DBAccessL20DB::QueryWeldRecipe(char* buffer)
{
	int RecipeID;
	memcpy(&RecipeID, buffer, sizeof(unsigned int));

	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    string str = ExecuteQuery(strQuery);
    if(str.size()==0)
        return ERROR;
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());
#endif
	vector<string> data;
	if (!Utility::StringToTokens(str,',',data))
		return ERROR;

	unsigned int m_Amplitude 	= stoi(data.at(6));
	int m_WidthSetting			= stoi(data.at(7));
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
	unsigned int m_EnergySetting= stoi(data.at(19));
    int m_PreBurst				= stoi(data.at(20));
    int m_HoldTime				= stoi(data.at(21));
    int m_SqueezeTime			= stoi(data.at(22));
    int m_AfterBurstDelay		= stoi(data.at(23));
    int m_AfterBurstTime		= stoi(data.at(24));
    int m_AfterBurstAmplitude	= stoi(data.at(25));
    int m_DisplayedHeightOffset	= stoi(data.at(30));
    int m_MeasuredHeightOffset	= stoi(data.at(31));
	
	Recipe::RecipeSC->m_RecipeID = atoi(data[0].c_str());
	cout << "641 Recipe::RecipeSC->m_RecipeID " << Recipe::RecipeSC->m_RecipeID <<endl;
    memcpy(Recipe::RecipeSC->m_RecipeName, data[1].c_str(), min(RECIPE_LEN, data[1].size()));
#if UNITTEST_DATABASE
    LOG("m_RecipeName %s\n", Recipe::RecipeSC->m_RecipeName);
#endif
    Recipe::RecipeSC->Set(&m_Amplitude, WeldRecipeSC::PARALIST::AMPLITUDE);
    Recipe::RecipeSC->Set(&m_WidthSetting, WeldRecipeSC::PARALIST::WIDTH_SETTING);
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
    Recipe::RecipeSC->Set(&m_EnergySetting, WeldRecipeSC::PARALIST::ENERGY_SETTING);
    Recipe::RecipeSC->Set(&m_PreBurst, WeldRecipeSC::PARALIST::PRE_BURST);
    Recipe::RecipeSC->Set(&m_HoldTime, WeldRecipeSC::PARALIST::HOLD_TIME);
    Recipe::RecipeSC->Set(&m_SqueezeTime, WeldRecipeSC::PARALIST::SQUEEZE_TIME);
    Recipe::RecipeSC->Set(&m_AfterBurstDelay, WeldRecipeSC::PARALIST::AFTER_BURST_DELAY);
    Recipe::RecipeSC->Set(&m_AfterBurstTime, WeldRecipeSC::PARALIST::AFTER_BURST_TIME);
    Recipe::RecipeSC->Set(&m_AfterBurstAmplitude, WeldRecipeSC::PARALIST::AFTER_BURST_AMPLITUDE);
    Recipe::RecipeSC->Set(&m_DisplayedHeightOffset, WeldRecipeSC::PARALIST::DISPLAYED_HEIGHT_OFFSET);
    Recipe::RecipeSC->Set(&m_MeasuredHeightOffset, WeldRecipeSC::PARALIST::MEASURED_HEIGHT_OFFSET);

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting energy[STEP_MAX];
    vector<WeldStepValueSetting> vectorEnergy;
    if(OK == String2Vector(str, &vectorEnergy))
        {
        Vector2Array(&vectorEnergy, energy);
        Recipe::RecipeSC->Set(&energy, WeldRecipeSC::PARALIST::ENERGY_STEP);
#if UNITTEST_DATABASE
        LOG("EnergyToStep\n");
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
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting timeStep[STEP_MAX];
    vector<WeldStepValueSetting> vectorTime;
    if(OK == String2Vector(str, &vectorTime))
        {
        Vector2Array(&vectorTime, timeStep);
        Recipe::RecipeSC->Set(&timeStep, WeldRecipeSC::PARALIST::TIME_STEP);
#if UNITTEST_DATABASE
        LOG("TimeToStep\n");
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
        std::to_string(RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting power[STEP_MAX];
    vector<WeldStepValueSetting> vectorPower;
    if(OK == String2Vector(str, &vectorPower))
        {
        Vector2Array(&vectorPower, power);
        Recipe::RecipeSC->Set(&power, WeldRecipeSC::PARALIST::POWER_STEP);
#if UNITTEST_DATABASE
        LOG("PowerToStep\n");
        for(int i=0; i<STEP_MAX; i++)
            {
            LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
                power[i].m_Order, power[i].m_StepValue, power[i].m_AmplitudeValue);
            }
#endif
        }
    str = ExecuteQuery(
 			"select * from " + string(TABLE_ACTIVE_RECIPE)
 					+ " where RecipeID = "
 					+ std::to_string(RecipeID) + ";");

     if(!str.empty())
     {
     	string strStore = 
     			"select BatchSize from " + string(TABLE_ACTIVE_RECIPE) + 
     			" where RecipeID = " + std::to_string(RecipeID) + ";";
     	str = ExecuteQuery(strStore);
     	Recipe::RecipeSC->Set(&str, WeldRecipeSC::PARALIST::BATCH_SIZE);
     }
    
    return OK;
}

/**************************************************************************//**
* \brief   - Query latest Alarm log records from DB
*
* \param   - null
*
* \return  - error code
*
******************************************************************************/
int DBAccessL20DB::QueryAlarmLogLatestPage(char *buffer)
{
	sendDataNum = FIRST_SEND_DATA_NUM;
	string strQuery = "select * from " + string(TABLE_ALARM_LOG)
			+ " order by DateTime DESC limit " + std::to_string(sendDataNum)
			+ ";";
	string str = ExecuteQuery(strQuery);
	if (str.empty()) {
		return ERROR;
	}
	
	assignAlarmLog(str);

	return OK;
}

/**************************************************************************//**
* \brief   - Query next page Alarm Log records from DB
*
* \param   - null
*
* \return  - error code
*
******************************************************************************/
int DBAccessL20DB::QueryAlarmLogNextPage(char *buffer)
{
	string strQuery = "select * from " + string(TABLE_ALARM_LOG)
			+ " order by DateTime DESC limit " + std::to_string(sendDataNum)
			+ ", " + std::to_string(ONE_PAGE_NUM) + ";";
	
	string str = ExecuteQuery(strQuery);
	if (str.empty()) {
		return ERROR;
	}
	
	assignAlarmLog(str);

	sendDataNum += ONE_PAGE_NUM;
	return OK;
}

/**************************************************************************//**
* \brief   - Decompose and store the data in the database into Vector
*
* \param   - buffer(data read from the database)
*
* \return  - null
*
******************************************************************************/
void DBAccessL20DB::assignAlarmLog(const string& buffer)
{
	vector<string> tmpStr;
    Utility::StringToTokens(buffer, ',', tmpStr);
	for(int count = 0; count < tmpStr.size()/TABLE_ALARM_MEM; count++)
	{
		shared_ptr<AlarmLogDefine> ptr_Alarm = AlarmLogDefine::GetAlarmLog();
		char DateTime[DATA_TIME_LENGTH];
		strncpy(DateTime, tmpStr[count*TABLE_ALARM_MEM+1].c_str(), DATA_TIME_LENGTH);
		ptr_Alarm->Set(AlarmLogDefine::PARALIST::DATE_TIME, DateTime);//DateTime
		int AlarmType = atoi(tmpStr[count*TABLE_ALARM_MEM+2].c_str());
		ptr_Alarm->Set(AlarmLogDefine::PARALIST::ALARM_TYPE, &AlarmType);//AlarmType
		int RecipeID = atoi(tmpStr[count*TABLE_ALARM_MEM+3].c_str());
		
		char RecipeName[USER_NAME_SIZE];
	    string strQuery =
	        "select RecipeName from "+string(TABLE_WELD_RECIPE)+
	        " where ID="+
	        std::to_string(RecipeID)+";";
	    string str = ExecuteQuery(strQuery);
	    if(str.empty()){
	    	strncpy(RecipeName, "N/A", USER_NAME_SIZE);
	    }
	    else{
	    	strncpy(RecipeName, str.c_str(), USER_NAME_SIZE);
	    } 
	    ptr_Alarm->Set(AlarmLogDefine::PARALIST::RECIPE_NAME, RecipeName);//RecipeName
	    
	    int WeldResultID = atoi(tmpStr[count*TABLE_ALARM_MEM+4].c_str());
	    strQuery = "select CycleCounter from "+string(TABLE_WELD_RESULT)+
	    	        " where ID="+
	    	        std::to_string(WeldResultID)+";";
	    str = ExecuteQuery(strQuery);
	    if(!str.empty()){
	    	int WeldCounter = std::atoi(str.c_str());
	    	ptr_Alarm->Set(AlarmLogDefine::PARALIST::WELD_COUNT, &WeldCounter);//RecipeName
	    }
	    AlarmLog::AlarmVector.push_back(ptr_Alarm);
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("assignAlarmLog:\n%s\n", str.c_str());
#endif
	}
	tmpStr.clear();
}

/**************************************************************************//**
* \brief   - Query AlarmLog from DB
*
* \param   - char* buffer - ID
*
* \return  - int - count of records
*
******************************************************************************/
int DBAccessL20DB::QueryBlockAlarmLog(char* buffer)
{
    int count;
    string str = ExecuteQuery(
                "select * from "+string(TABLE_ALARM_LOG)+
                " where WeldResultID="+
                std::to_string(*(int*)buffer)+";");

#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryAlarmLog:\n%s\n", str.c_str());
#endif
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
int DBAccessL20DB::QueryHeightCalibration(char* buffer)
{
	int nErrCode = SQLITE_ERROR;
	int count;
    string str = ExecuteQuery(
                "select PSI, Count from "+string(TABLE_HI_CALIB)+";", &nErrCode);
    if(str.empty() == true)
    	return ERROR;
    if(nErrCode != SQLITE_OK)
    	return ERROR;

	vector<string> tmpStr;
    Utility::StringToTokens(str, ',', tmpStr);
    //The expected result shall be "1, 30000, 2, 30000, ..., 20, 32261"; The first is PSI, second the height raw count.
    for(count = 0; count < tmpStr.size() / 2; count++)
    {
    	HeightEncoder::HeightCalibratedMap[atoi(tmpStr[count * 2].c_str())].ZeroCount = atoi(tmpStr[count * 2 + 1].c_str());
#if UNITTEST_DATABASE
        LOG("PSI:%s ", tmpStr[count * 2].c_str());
        LOG("Count:%s\n", tmpStr[count * 2+1].c_str());
#endif
    }

    return count;
}

/**************************************************************************//**
* \brief   - Query DbVersion from DB
*
* \param   - char* buffer - not used
*
* \return  - query result
*
******************************************************************************/
//TODO Is it temporary code for test only, because there is not any return?
int DBAccessL20DB::QueryDbVersion(char* buffer)
{
    string str = ExecuteQuery(
                "select * from "+string(TABLE_DB_VERSION)+";"); 
    if(str.size()==0)
        return ERROR;

	vector<string> tmpStr;
    Utility::StringToTokens(str, DB_VALUE_SEPARATOR[0], tmpStr);

	sprintf(CommonProperty::SystemInfo.version_DB,"%s.%s.%s",
            tmpStr[0].c_str(),
            tmpStr[1].c_str(),
            tmpStr[2].c_str());
#if UNITTEST_DATABASE
    LOG("QueryDbVersion:%s\n", str.c_str());
    LOG("Major:%s\n", tmpStr[0].c_str());
    LOG("Minor:%s\n", tmpStr[1].c_str());
    LOG("Build:%s\n", tmpStr[2].c_str());
#endif
    return OK;
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
#if UNITTEST_DATABASE
    if(UserAuthority::_UserProfilesSC == nullptr)
        {
		UserAuthority::GetInstance();
        LOG("new _UserProfilesSC\n");
        }
#endif
    int count;
	vector<string> tmpStr;
	User tmpUser;
	int nErrCode = SQLITE_ERROR;
    string str = ExecuteQuery("select PermissionLevel, Password from "+string(TABLE_USER_PROFILE)+";", &nErrCode);
    if(str.empty() == true)
    	return ERROR;
    if(nErrCode != SQLITE_OK)
    	return ERROR;
    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/2; count++)
	{
		tmpUser.m_Level = (PREMISSION_LEVEL)atoi(tmpStr[count*2].c_str());
		tmpUser.m_Password = tmpStr[count*2+1];
		auto iter = UserAuthority::_UserProfilesSC->find((int)tmpUser.m_Level);	
	    if(iter != UserAuthority::_UserProfilesSC->end())
		{
			iter->second.m_Level = tmpUser.m_Level;
			iter->second.m_Password = tmpUser.m_Password;
#if UNITTEST_DATABASE
            LOG("\tm_Level(%d)\n", iter->second->m_Level);
            LOG("\tm_Password(%s)\n", iter->second->m_Password.c_str());
#endif
		}
	}
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
int DBAccessL20DB::QueryBlockPrivilegeConfigure(char* buffer)
{ 
    int count;
	vector<string> tmpStr;
	int nErrCode = SQLITE_ERROR;
    string str = ExecuteQuery("select ScreenIndex,PermissionLevel from "+string(TABLE_PRIVILEGE_CONFIG)+";", &nErrCode);
    if(str.empty() == true)
    	return 0;
    if(nErrCode != SQLITE_OK)
    	return 0;
    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/2; count++)
	{
		auto iter = UserAuthority::_UserPrivilegesSC->find(atoi(tmpStr[count*2].c_str()));
		if(iter != UserAuthority::_UserPrivilegesSC->end())
		{
			iter->second = atoi(tmpStr[count*2+1].c_str());
		}
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
    if(str.empty() == true)
    	return ERROR;
#if UNITTEST_DATABASE
    LOG("QueryBlockPowerSupply:\n%s\n\n", str.c_str());
    SystemConfiguration::PowerSupplyType.clear();
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size() / TABLE_PWR_SUP_MEM; count++)
	{
	    POWER_SUPPLY_TYPE tmpPwr;
        tmpPwr.Frequency = atoi(tmpStr[count * TABLE_PWR_SUP_MEM+1].c_str());//Frequency
        tmpPwr.Power = atoi(tmpStr[count * TABLE_PWR_SUP_MEM+2].c_str());//Power
		SystemConfiguration::PowerSupplyType.push_back(tmpPwr);

#if UNITTEST_DATABASE
        LOG("ID: %s\n", tmpStr[count * TABLE_PWR_SUP_MEM].c_str());
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
	if(str.empty() == true)
    	return ERROR;
#if UNITTEST_DATABASE
    LOG("QueryBlockTeachModeSetting:\n%s\n\n", str.c_str());
    SystemConfiguration::TeachModeSetting.clear();
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/TABLE_TEACH_MODE_MEM; count++)
	{
		TEACHMODE tmpTeach;
        tmpTeach.Teach_mode = (TEACHMODE_TYPE)atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+1].c_str());//TeachModeType
        tmpTeach.Time_Upper = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+2].c_str());//TimePLRG
        tmpTeach.Time_Lower = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+3].c_str());//TimeMSRG
        tmpTeach.Power_Upper = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+4].c_str());//PowerPLRG
        tmpTeach.Power_Lower = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+5].c_str());//PowerMSRG
        tmpTeach.PreHeight_Upper = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+6].c_str());//PreHeightPLRG
        tmpTeach.PreHeight_Lower = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+7].c_str());//PreHeightMSRG
        tmpTeach.PostHeight_Upper = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+8].c_str());//HeightPLRG
        tmpTeach.PostHeight_Lower = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+9].c_str());//HeightMSRG
        tmpTeach.Quantity = atoi(tmpStr[count*TABLE_TEACH_MODE_MEM+10].c_str());//Quantity
		SystemConfiguration::TeachModeSetting.push_back(tmpTeach);

#if UNITTEST_DATABASE
        LOG("ID: %s\n", tmpStr[count*TABLE_TEACH_MODE_MEM].c_str());
        LOG("TeachMode: %d\n", tmpTeach.Teach_mode);
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
int DBAccessL20DB::QuerySystemConfigure(char* buffer)
{
    int count;
    string str;
	vector<string> tmpStr;

    str = ExecuteQuery(string("select * from ")+string(TABLE_SYS_CONFIG)+";");
    if(str.empty() == true)
    	return ERROR;
#if UNITTEST_DATABASE
    LOG("QuerySystemConfigure:\n%s\n\n", str.c_str());
#endif

    Utility::StringToTokens(str, ',', tmpStr);
	SYSTEMCONFIGFORUI tmpSysCon;
    tmpSysCon.bFootPedalAbort      =                 atoi(tmpStr[0].c_str()); //FootPedalAbort
    tmpSysCon.bLockOnAlarm         =                 atoi(tmpStr[1].c_str()); //LockOnAlarm
    tmpSysCon.bHeightEncoder       =                 atoi(tmpStr[2].c_str()); //HeightEncoder
    tmpSysCon.bFirstScreen		   =                 atoi(tmpStr[3].c_str()); //FirstScreen
    tmpSysCon.Cooling              =        (COOLING)atoi(tmpStr[4].c_str()); //CoolingOption
    tmpSysCon.CoolingDuration      =                 atoi(tmpStr[5].c_str()); //CoolingDuration
    tmpSysCon.CoolingDelay         =                 atoi(tmpStr[6].c_str()); //CoolingDelay
    tmpSysCon.Language             =       (LANGUAGE)atoi(tmpStr[7].c_str()); //Language
    tmpSysCon.Amplitude_Unit       = (AMPLITUDE_UNIT)atoi(tmpStr[8].c_str()); //AmplitudeUnit
    tmpSysCon.Pressure_Unit        =  (PRESSURE_UNIT)atoi(tmpStr[9].c_str()); //PressureUnit
    tmpSysCon.Height_Unit          =    (HEIGHT_UNIT)atoi(tmpStr[10].c_str()); //HeightUnit
    tmpSysCon.MaxAmplitude         =                 atoi(tmpStr[11].c_str()); //MaxAmplitude
    tmpSysCon.TeachMode.Teach_mode = (TEACHMODE_TYPE)atoi(tmpStr[12].c_str()); //TeachModeID

	struct tm timeStamp;
	vxbRtcGet(&timeStamp);
    strftime(tmpSysCon.DateTime, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
#if 0 // not matched
    tmpSysCon.xxx = atoi(tmpStr[12].c_str()); //HomePositionCount
    tmpSysCon.xxx = atoi(tmpStr[15].c_str()); //TunePoint
#endif
    SystemConfiguration::Set(&tmpSysCon);
    return OK;
}

/**************************************************************************//**
* \brief   - Query the unique record from table ActiveRecipe
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
int DBAccessL20DB::QueryActiveRecipe(char* buffer)
{
    string str = ExecuteQuery(string("select * from ")+string(TABLE_ACTIVE_RECIPE)+";");
    if(str.size() == 0)
    {
    	LOGERR((char*) "ActiveRecipe is null\n", 0, 0, 0);
    	return ERROR;
    }
    vector<string> tmpStr;
    Utility::StringToTokens(str, DB_VALUE_SEPARATOR[0], tmpStr);

    WeldResults::_WeldResults->CycleCounter = atoi(tmpStr[0].c_str()); //TODO CycleNumber
	int BatchSize = atoi(tmpStr[1].c_str()); //BatchSize
	Recipe::ActiveRecipeSC->Set(&BatchSize, WeldRecipeSC::PARALIST::BATCH_SIZE);
	Recipe::ActiveRecipeSC->m_RecipeID = atoi(tmpStr[3].c_str()); //RecipeID

#if UNITTEST_DATABASE
    LOG("CycleCounter: %d\n", WeldResults::_WeldResults->CycleCounter);
    BatchSize=0;
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::BATCH_SIZE, &BatchSize);
    LOG("BatchSize: %d\n", BatchSize);
    LOG("RecipeID: %d\n", Recipe::ActiveRecipeSC->m_RecipeID);
#endif
    
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(Recipe::ActiveRecipeSC->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    if(str.size()==0)
        return ERROR;
#if UNITTEST_DATABASE
    if(str.size()>0)
        LOG("QueryWeldRecipe:\n%s\n", str.c_str());
#endif
	vector<string> data;
	if (!Utility::StringToTokens(str,',',data))
		return ERROR;

	unsigned int m_Amplitude 	= stoi(data.at(6));
	int m_WidthSetting			= stoi(data.at(7));
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
	unsigned int m_EnergySetting= stoi(data.at(19));
    int m_PreBurst				= stoi(data.at(20));
    int m_HoldTime				= stoi(data.at(21));
    int m_SqueezeTime			= stoi(data.at(22));
    int m_AfterBurstDelay		= stoi(data.at(23));
    int m_AfterBurstTime		= stoi(data.at(24));
    int m_AfterBurstAmplitude	= stoi(data.at(25));
    int m_DisplayedHeightOffset	= stoi(data.at(30));
    int m_MeasuredHeightOffset	= stoi(data.at(31));
	
	Recipe::ActiveRecipeSC->m_RecipeID = atoi(data[0].c_str());
    memcpy(Recipe::ActiveRecipeSC->m_RecipeName, data[1].c_str(), min(RECIPE_LEN, data[1].size()));
    Recipe::ActiveRecipeSC->Set(&m_Amplitude, WeldRecipeSC::PARALIST::AMPLITUDE);
    Recipe::ActiveRecipeSC->Set(&m_WidthSetting, WeldRecipeSC::PARALIST::WIDTH_SETTING);
    Recipe::ActiveRecipeSC->Set(&m_WPpressure, WeldRecipeSC::PARALIST::WP_PRESSURE);
    Recipe::ActiveRecipeSC->Set(&m_TPpressure, WeldRecipeSC::PARALIST::TP_PRESSURE);
    Recipe::ActiveRecipeSC->Set(&m_TimeMax, WeldRecipeSC::PARALIST::TIME_MAX);
    Recipe::ActiveRecipeSC->Set(&m_TimeMin, WeldRecipeSC::PARALIST::TIME_MIN);
    Recipe::ActiveRecipeSC->Set(&m_PeakPowerMax, WeldRecipeSC::PARALIST::PEAK_POWER_MAX);
    Recipe::ActiveRecipeSC->Set(&m_PeakPowerMin, WeldRecipeSC::PARALIST::PEAK_POWER_MIN);
    Recipe::ActiveRecipeSC->Set(&m_PreHeightMax, WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX);
    Recipe::ActiveRecipeSC->Set(&m_PreHeightMin, WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN);
    Recipe::ActiveRecipeSC->Set(&m_HeightMax, WeldRecipeSC::PARALIST::HEIGHT_MAX);
    Recipe::ActiveRecipeSC->Set(&m_HeightMin, WeldRecipeSC::PARALIST::HEIGHT_MIN);
    Recipe::ActiveRecipeSC->Set(&m_WeldMode, WeldRecipeSC::PARALIST::WELD_MODE);
    Recipe::ActiveRecipeSC->Set(&m_EnergySetting, WeldRecipeSC::PARALIST::ENERGY_SETTING);
    Recipe::ActiveRecipeSC->Set(&m_PreBurst, WeldRecipeSC::PARALIST::PRE_BURST);
    Recipe::ActiveRecipeSC->Set(&m_HoldTime, WeldRecipeSC::PARALIST::HOLD_TIME);
    Recipe::ActiveRecipeSC->Set(&m_SqueezeTime, WeldRecipeSC::PARALIST::SQUEEZE_TIME);
    Recipe::ActiveRecipeSC->Set(&m_AfterBurstDelay, WeldRecipeSC::PARALIST::AFTER_BURST_DELAY);
    Recipe::ActiveRecipeSC->Set(&m_AfterBurstTime, WeldRecipeSC::PARALIST::AFTER_BURST_TIME);

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(Recipe::ActiveRecipeSC->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting energy[STEP_MAX];
    vector<WeldStepValueSetting> vectorEnergy;
    if(OK == String2Vector(str, &vectorEnergy))
        {
        Vector2Array(&vectorEnergy, energy);
        Recipe::ActiveRecipeSC->Set(&energy, WeldRecipeSC::PARALIST::ENERGY_STEP);
#if UNITTEST_DATABASE
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
        std::to_string(Recipe::ActiveRecipeSC->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting timeStep[STEP_MAX];
    vector<WeldStepValueSetting> vectorTime;
    if(OK == String2Vector(str, &vectorTime))
        {
        Vector2Array(&vectorTime, timeStep);
        Recipe::ActiveRecipeSC->Set(&timeStep, WeldRecipeSC::PARALIST::TIME_STEP);
#if UNITTEST_DATABASE
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
        std::to_string(Recipe::ActiveRecipeSC->m_RecipeID)+";";
    str = ExecuteQuery(strQuery);
    WeldStepValueSetting power[STEP_MAX];
    vector<WeldStepValueSetting> vectorPower;
    if(OK == String2Vector(str, &vectorPower))
        {
        Vector2Array(&vectorPower, power);
        Recipe::ActiveRecipeSC->Set(&power, WeldRecipeSC::PARALIST::POWER_STEP);
#if UNITTEST_DATABASE
        LOG("\nPowerToStep\n");
        for(int i=0; i<STEP_MAX; i++)
            {
            LOG("\tOrder(%d): StepValue %d, AmplitudeValue %d\n",
                power[i].m_Order, power[i].m_StepValue, power[i].m_AmplitudeValue);
            }
#endif
        }
    return OK;
}

/**************************************************************************//**
* \brief   - Query the unique record from table Connectivity
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
int DBAccessL20DB::QueryConnectivity(char* buffer)
{
	vector<string> tmpStr;
    string str;

    str = ExecuteQuery(string("select * from ")+string(TABLE_CONNECTIVITY)+";");
    if(str.empty() == true)
    	return ERROR;

    Utility::StringToTokens(str, ',', tmpStr);
    Connectivity::EthernetConfig.EthernetType        = atoi(tmpStr[0].c_str()); /* EthernetType */
    Connectivity::EthernetConfig.TCP_RemoteSignature = atoi(tmpStr[1].c_str()); /* SignatureOption */
    Connectivity::EthernetConfig.TCP_ServerPort      = atoi(tmpStr[2].c_str()); /* ServerPort */
    memcpy(Connectivity::EthernetConfig.TCP_ServerIP, tmpStr[3].c_str(), min(IP_ADD_LEN, tmpStr[3].size())); /* DeviceIP */
    Connectivity::EthernetConfig.DIG_WeldResult      = atoi(tmpStr[4].c_str()); /* WeldResultOption */
    Connectivity::EthernetConfig.DIG_WeldRecipe      = atoi(tmpStr[5].c_str()); /* WeldRecipeOption */
    Connectivity::EthernetConfig.DIG_Signature       = atoi(tmpStr[6].c_str()); /* WeldSignatureOption */
    Connectivity::EthernetConfig.DIG_SysConfigure    = atoi(tmpStr[7].c_str()); /* SystemConfigureOption */
    Connectivity::EthernetConfig.DIG_MachineID       = atoi(tmpStr[8].c_str()); /* GatewayID */
#if UNITTEST_DATABASE
    LOG("QueryConnectivity: %s\n\n", str.c_str());
    LOG("TCP_ServerIP: %s\n\n", Connectivity::EthernetConfig.TCP_ServerIP);
#endif

    return OK;
}

/**************************************************************************//**
* \brief   - Query the unique record from table GatewayServer
*
* \param   - char* buffer - not used
*
* \return  - N/A
*
******************************************************************************/
int DBAccessL20DB::QueryGatewayMachine(char* buffer)
{
#if UNITTEST_DATABASE
    if(Connectivity::_DIGMachinesUI == nullptr)
        {
		Connectivity::GetInstance();
        LOG("new _DIGMachinesUI\n");
        }
    Connectivity::_DIGMachinesUI->clear();
#endif
	vector<string> tmpStr;
    string str;
    int count = 0;
    auto iter = Connectivity::_DIGMachinesSC->begin();
    str = ExecuteQuery(string("select * from ")+string(TABLE_GATEWAY_MACHINE)+";");
    if(str.empty() == true)
    	return ERROR;

    Utility::StringToTokens(str, ',', tmpStr);
	for(count = 0; count < tmpStr.size()/TABLE_GATEWAY_MACHINE_MEM; count++)
	{
        iter = Connectivity::_DIGMachinesSC->find(count);
        if(iter != Connectivity::_DIGMachinesSC->end())
        {
        	memcpy(iter->second.DIG_MachineName, tmpStr[count*TABLE_GATEWAY_MACHINE_MEM+1].c_str(), MACHINE_NAME_LEN); /* MachineName */
			iter->second.DIG_MachinePort = atoi(tmpStr[count*TABLE_GATEWAY_MACHINE_MEM+2].c_str()); //ServerPort
			memcpy(iter->second.DIG_MachineIP, tmpStr[count*TABLE_GATEWAY_MACHINE_MEM+3].c_str(), IP_ADD_LEN); /* ServerIP */
	#if UNITTEST_DATABASE
			LOG("ID: %s\n", tmpStr[count*TABLE_GATEWAY_MACHINE_MEM].c_str());
			LOG("DIG_MachineName: %s\n", machine.DIG_MachineName);
			LOG("DIG_MachinePort: %d\n", machine.DIG_MachinePort);
			LOG("DIG_MachineIP: %s\n", machine.DIG_MachineIP);
			LOG("\n");
	#endif
        }
	}
    return count;
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
    string strStore;
	int nErrCode = SQLITE_ERROR;

#if UNITTEST_DATABASE
	if(Recipe::RecipeSC == nullptr)
    	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
    LOG("# Recipe::RecipeSC->m_RecipeID %d\n", Recipe::RecipeSC->m_RecipeID);
    Recipe::RecipeSC->m_RecipeID = 1;
#endif
	
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
	int m_EnergySetting = 0;
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
	!Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::ENERGY_SETTING, &m_EnergySetting)||
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
    strftime(timeBuf, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);

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
        ", ModeValue=" 			+ std::to_string(m_EnergySetting)+//ModeValue
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
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
		return ERROR;
	}
    
    string str = ExecuteQuery(
			"select * from " + string(TABLE_ACTIVE_RECIPE)
					+ " where RecipeID = "
					+ std::to_string(Recipe::RecipeSC->m_RecipeID) + ";");
    if(!str.empty())
    {
    	unsigned int m_BatchSize = 0;
    	strStore = "";
    	Recipe::RecipeSC->Get(WeldRecipeSC::PARALIST::BATCH_SIZE, &m_BatchSize);
    	strStore = 
    			"UPDATE " + string(TABLE_ACTIVE_RECIPE) + 
    			" SET BatchSize = " + std::to_string(m_BatchSize) +
    			" where RecipeID = " + std::to_string(Recipe::RecipeSC->m_RecipeID) + ";";
    	nErrCode = SingleTransaction(strStore);
    	if(nErrCode != SQLITE_OK)
    	{
    		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
    		return ERROR;
    	}
    }

	return OK;
}

/**************************************************************************//**
* \brief   - Rename Specific Weld Recipe to DB
*
* \param   - char* buffer (not used)
*
* \return  - int - Error Code
*
******************************************************************************/
int DBAccessL20DB::RenameWeldRecipe(char* buffer)
{
    string strStore;
	int nErrCode = SQLITE_ERROR;

#if UNITTEST_DATABASE
	if(Recipe::RecipeSC == nullptr)
    	Recipe::RecipeSC = WeldRecipeSC::GetWeldRecipeSC();
    Recipe::RecipeSC->m_RecipeID = 1;
	LOG("# Recipe::RecipeSC->m_RecipeID %d\n", Recipe::RecipeSC->m_RecipeID);
#endif
	strStore = ExecuteQuery(
			"select RecipeName from " + string(TABLE_WELD_RECIPE) +
			                    " where RecipeName='"+Recipe::RecipeSC->m_RecipeName+"';");
	if(!strStore.empty())
	{
		LOGERR((char*) "Database_T: %s already exists in table WeldRecipe\n", (int)Recipe::RecipeSC->m_RecipeName, 0, 0);
    	return ERROR;
	}

	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y/%m/%d %H:%M:%S", &timeStamp);
	
	strStore = 
			"UPDATE " + string(TABLE_WELD_RECIPE) + 
			" SET RecipeName = '" + Recipe::RecipeSC->m_RecipeName +
			"' , DateTime = '" + timeBuf + 
			"' where ID = " + std::to_string(Recipe::RecipeSC->m_RecipeID) + ";";

	nErrCode = SingleTransaction(strStore);
#if UNITTEST_DATABASE
    LOG("# update WeldRecipe: result %d\n", nErrCode);
#endif
	//Recipe::RecipeSC = nullptr;
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
		return ERROR;
	}

	return OK;
}

/**************************************************************************//**
* \brief   - Update HeightCalibration to DB
*
* \param   - No used.
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateHeightCalibration(char* buffer)
{
	int nErrCode = ERROR;
    int PSI = 0;
    auto iter = HeightEncoder::HeightCalibratedMap.begin();
    for(iter = HeightEncoder::HeightCalibratedMap.begin(); iter != HeightEncoder::HeightCalibratedMap.end(); iter++)
    {
    	PSI = iter->first;
    	if(PSI != 0)
    	{
    		string strStore =
    	        "update " 	+ string(TABLE_HI_CALIB) +
    	        " set Count=" + std::to_string(HeightEncoder::HeightCalibratedMap[PSI].ZeroCount)+//Count
    	        " where PSI=" + std::to_string(PSI)+//PSI
    	        +";";

    		nErrCode = SingleTransaction(strStore);
    	#if UNITTEST_DATABASE
    	    LOG("# %s\n", strStore.c_str());
    	#endif
    		if(nErrCode != SQLITE_OK)
    		{
    			LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
    			break;
    		}
    	}
    }
    if(iter == HeightEncoder::HeightCalibratedMap.end())
    	nErrCode = OK;
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
    auto iter = UserAuthority::_UserProfilesSC->find(*(int* )buffer);
    if(iter == UserAuthority::_UserProfilesSC->end())
	{
	    return ERROR;
	}
    User _User = iter->second;
    //TODO, handle user passwords security in future.
	string strStore =
        "update " 	+ string(TABLE_USER_PROFILE) +
        " set Password='" + _User.m_Password.c_str()+//Password
        "' where PermissionLevel="+ std::to_string(_User.m_Level)+";";//PermissionLevel
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
		TEACHMODE tmpTeach = SystemConfiguration::TeachModeSetting.at(i);
    	string strStore =
            "update " 				+ string(TABLE_TEACH_MODE_SET) +
            " set TeachModeType=" 	+ std::to_string(tmpTeach.Teach_mode)+//TeachModeType
            ", TimePLRG=" 			+ std::to_string(tmpTeach.Time_Upper)+//TimePLRG
            ", TimeMSRG=" 			+ std::to_string(tmpTeach.Time_Lower)+//TimeMSRG
            ", PowerPLRG=" 			+ std::to_string(tmpTeach.Power_Upper)+//PowerPLRG
            ", PowerMSRG=" 			+ std::to_string(tmpTeach.Power_Lower)+//PowerMSRG
            ", PreHeightPLRG=" 		+ std::to_string(tmpTeach.PreHeight_Upper)+//PreHeightPLRG
            ", PreHeightMSRG=" 		+ std::to_string(tmpTeach.PreHeight_Lower)+//PreHeightMSRG
            ", HeightPLRG=" 		+ std::to_string(tmpTeach.PostHeight_Upper)+//HeightPLRG
            ", HeightMSRG=" 		+ std::to_string(tmpTeach.PostHeight_Lower)+//HeightMSRG
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
		", FirstScreen="		+ std::to_string(tmpSysCon.bFirstScreen)+
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
* \brief   - Update the unique record to ActiveRecipe
*
* \param   - char* buffer - RecipeID
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateActiveRecipe(char* buffer)
{
	int BatchSize;
	int RecipeID;
	memcpy(&RecipeID, buffer, sizeof(int));
	Recipe::ActiveRecipeSC->Get(WeldRecipeSC::PARALIST::BATCH_SIZE, &BatchSize);
	string strStore =
        "update " 			+ string(TABLE_ACTIVE_RECIPE) +
        " set CycleNumber=" + std::to_string(WeldResults::_WeldResults->CycleCounter)+
        ", BatchSize=" 		+ std::to_string(BatchSize)+
        ", RecipeID=" 		+ std::to_string(RecipeID)+
        ";";
	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != SQLITE_OK)
    {
		LOGERR((char*) "Database_T: UpdateActiveRecipe Error. %d\n", nErrCode, 0, 0);
    }
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Update the unique record to Connectivity
*
* \param   - char* buffer - not used
*
* \return  - int - Database status
*
******************************************************************************/
int DBAccessL20DB::UpdateConnectivity(char* buffer)
{
	string strStore =
        "update " 				+ string(TABLE_CONNECTIVITY) +
        " set EthernetType=" 	+ std::to_string(Connectivity::EthernetConfig.EthernetType)+
        ", SignatureOption=" 	+ std::to_string(Connectivity::EthernetConfig.TCP_RemoteSignature)+
        ", ServerPort=" 		+ std::to_string(Connectivity::EthernetConfig.TCP_ServerPort)+
        ", DeviceIP='" 			+ Connectivity::EthernetConfig.TCP_ServerIP+
        "', WeldResultOption=" 	+ std::to_string(Connectivity::EthernetConfig.DIG_WeldResult)+
        ", WeldRecipeOption=" 	+ std::to_string(Connectivity::EthernetConfig.DIG_WeldRecipe)+
        ", WeldSignatureOption=" 	+ std::to_string(Connectivity::EthernetConfig.DIG_Signature)+
        ", SystemConfigureOption=" 	+ std::to_string(Connectivity::EthernetConfig.DIG_SysConfigure)+
        ", GatewayID=" 			+ std::to_string(Connectivity::EthernetConfig.DIG_MachineID)+
        ";";
	int nErrCode = SingleTransaction(strStore);
	if(nErrCode != SQLITE_OK)
    {   
		LOGERR((char*) "Database_T: UpdateConnectivity Error. %d\n", nErrCode, 0, 0);
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
int DBAccessL20DB::DeleteOldest(const char* table)
{
    string strQuery =
        "delete from "+
        string(table)+" where ID in (select ID from "+
        string(table)+" order by ID asc limit 1);";
    SingleTransaction(strQuery);
    return OK;
}

/**************************************************************************//**
* \brief   - Delete Specific Weld Recipe to DB
*
* \param   - char *buffer - RecipeID
*
* \return  - int - ErrCode
*
******************************************************************************/
int DBAccessL20DB::DeleteSpecificRecipe(const char *buffer)
{
	int Index;
	memcpy(&Index, buffer, sizeof(int));
	
    string str = ExecuteQuery("select * from " + string(TABLE_ACTIVE_RECIPE) + " where RecipeID = " + std::to_string(Index) +";");
    if(str.size() != 0)
    {
    	LOGERR((char*) "Database_T: Not Delete ActiveRecipe.ID = %d\n", Index, 0, 0);
    	return ERROR;
    }
	
	string strQuery = "delete from "+string(TABLE_WELD_RECIPE)+" where ID = "+ std::to_string(Index) +";";
    int nErrCode= SingleTransaction(strQuery);
	if(nErrCode != 0)
	{
		LOGERR((char*) "Database_T: Delete Specific Recipe Error. %d\n", nErrCode, 0, 0);
		return ERROR;
	}
	return OK;
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
	{
	    if(_id == nullptr)
        {   
#if UNITTEST_DATABASE
            LOG("latest id of table %s is %u\n", table.c_str(), atoi(str.c_str()));
#endif
            errorCode = SQLITE_ERROR;
		}
        else
        	*_id = atoi(str.c_str());
	}
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
int DBAccessL20DB::Vector2String(const vector<shared_ptr<WeldResultSignatureDefine>>* _ptrVector, string & str)
{
	if (_ptrVector->size() == 0)
	{
		return ERROR;
	}
	shared_ptr<WeldResultSignatureDefine> _Signature;
	int j = 0;
	int iError = OK;
	int iResult = 0;
	for (int i = 0; i < _ptrVector->size(); i++)
	{
		_Signature = _ptrVector->at(i);
		for(int j = 0; j < WeldResultSignatureDefine::TOTAL; j++)
		{
			iResult = _Signature->Get(j, &iError);
			if(iError == OK)
				str += std::to_string(iResult) + ",";
		}
		str.pop_back(); // instead of "," using ";"
		str += ";";
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
int DBAccessL20DB::String2Vector(const string str, vector<shared_ptr<WeldResultSignatureDefine>>* _ptrVector)
{

	int iResult = ERROR;
	shared_ptr<WeldResultSignatureDefine> _Signature = WeldResultSignatureDefine::GetWeldSignature();

	if (str.empty() == true)
		return ERROR;
	if (_ptrVector == nullptr)
		return ERROR;
	vector<string> tmpStringlist;
	vector<string> tmpStringData;
	if (Utility::StringToTokens(str, ';', tmpStringlist) == 0)
		return ERROR;
	int vectorIndex;
	unsigned int data = 0;
	for (int i = 0; i < tmpStringlist.size(); i++)
	{
		vectorIndex = 0;
    	tmpStringData.clear();
		if (Utility::StringToTokens(tmpStringlist[i], ',', tmpStringData) > 0)
		{
			for (int j = 0; (j < WeldResultSignatureDefine::TOTAL) && (j<tmpStringData.size()); j++)
			{
				data = std::stoi(tmpStringData[vectorIndex]);
				if(_Signature->Set(j, data) == OK)
					vectorIndex++;
			}	
			_ptrVector->push_back(_Signature);
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
		if ((Utility::StringToTokens(tmpStringlist[i], ',', tmpStringData) > 0) &&
            (tmpStringData.size() == sizeof(WeldStepValueSetting)/sizeof(int)))
		{
			tmpStepSetting.m_Order = std::stoi(tmpStringData[Recipe::ORDER]);
			tmpStepSetting.m_StepValue = std::stoi(tmpStringData[Recipe::STEPVALUE]);
			tmpStepSetting.m_AmplitudeValue = std::stoi(tmpStringData[Recipe::AMPLITUDE]);
    		_ptrVector->push_back(tmpStepSetting);
		}
		else
		{
			return ERROR;
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

