/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains DBAccessP1BaseDB class implementation.
 	 Use this class to access L20 refresh database file sample_L20.db.  
 
**********************************************************************************************************/
#include <jansson.h>
#include "DBAccess_l20_db.h"
#include "commons.h"
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
* \param   - char *buffer - not used
*
* \return  - UINT8 -status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldResult(char* buffer)
{
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
		CommonProperty::WeldResult.PartID+"','"+//PartID
		timeBuf+"',"+//DateTime
		std::to_string(CommonProperty::WeldResult.RecipeNum)+","+//RecipeID
		std::to_string(CommonProperty::WeldResult.TotalEnergy)+","+//WeldEnergy
		std::to_string(CommonProperty::WeldResult.TriggerPressure)+","+//TriggerPressure
		std::to_string(CommonProperty::WeldResult.WeldPressure)+","+//WeldPressure
		std::to_string(CommonProperty::WeldResult.Amplitude)+","+//WeldAmplitude
		std::to_string(CommonProperty::WeldResult.WeldTime)+","+//WeldTime
		std::to_string(CommonProperty::WeldResult.PeakPower)+","+//WeldPeakPower
		std::to_string(CommonProperty::WeldResult.PreHeight)+","+//TriggerHeight
		std::to_string(CommonProperty::WeldResult.PostHeight)+","+//WeldHeight
		std::to_string(CommonProperty::WeldResult.ALARMS.ALARMflags)+","+//AlarmFlag
		std::to_string(0)+","+//SequenceID
		std::to_string(CommonProperty::WeldResult.CycleCounter)+");";//CycleCounter

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
    char *json = vector2Json(CommonProperty::WeldSignatureVector);
    if(json!=NULL)
        {
    	string strStore =
            "insert into " + string(TABLE_WELD_SIGNATURE) +
            " (WeldResultID, WeldGraph) " +
            "values ("+
            std::to_string(*(int *)buffer)+",'"+//WeldResultID
            json+"');";//WeldGraph

        free(json);
    	nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
        static int count=0;
        printf("#WeldSignature(num %d): result %d\n%s\n\n", count++,nErrCode, strStore.c_str());
#endif
        }
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Writing Weld Recipe into DB
*
* \param   - char *buffer - not used
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
int DBAccessL20DB::StoreWeldRecipe(char* buffer)
{
	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

#ifdef UNITTEST_DATABASE
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_Order = 1;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_StepValue = 2;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_AmplitudeValue = 3;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_Order = 4;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_StepValue = 5;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_AmplitudeValue = 6;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_Order = 7;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_StepValue = 8;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_AmplitudeValue = 9;

    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TimeStep[0].m_Order = 111;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_PowerStep[0].m_Order = 222;
#endif
    char *jsonEnergy = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep, STEP_MAX);
    char *jsonTime = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TimeStep, STEP_MAX);
    char *jsonPower = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_PowerStep, STEP_MAX);

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
        std::to_string(CommonProperty::ActiveRecipeSC.m_IsTeachMode)+","+//IsValidate
        std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude)+","+//Amplitude
        std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WidthSetting)+","+//Width
        std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WPpressure)+","+//WeldPressure
        std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure)+","+//TriggerPressure
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMax)+","+//TimePlus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMin)+","+//TimeMinus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMax)+","+//PeakPowerPlus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMin)+","+//PeakPowerMinus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PreHeightMax)+","+//TriggerHeightPlus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PreHeightMin)+","+//TriggerHeightMinus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_HeightMax)+","+//WeldHeightPlus
        std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_HeightMin)+","+//WeldHeightMinus
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_WeldMode)+","+//WeldMode
        std::to_string(0)+","+//ModeValue
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_PreBurst)+","+//PreBurst
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_HoldTime)+","+//HoldTime
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_SqueezeTime)+","+//SqueezeTime
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstDelay)+","+//AfterBurstDelay
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstTime)+","+//AfterBurstDuration
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstAmplitude)+","+//AfterBurstAmplitude
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_DisplayedHeightOffset)+","+//WeldHeight
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_MeasuredHeightOffset)+","+//MeasuredHeight
        std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_WeldStepMode)+",'"+//StepWeldMode
        jsonEnergy+"','"+//EnergyToStep
        jsonTime+"','"+//TimeToStep
        jsonPower+"','"+//PowerToStep
        CommonProperty::ActiveRecipeSC.m_RecipeName+"','"+//RecipeName
        timeBuf+"','"+//DateTime
        CommonProperty::ActiveRecipeSC.m_RecipePicPath+"');";//PresetPicPath

    free(jsonEnergy);
    free(jsonTime);
    free(jsonPower);

	int nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    static int count=0;
    printf("#WeldRecipe(num %d): result %d\n%s\n\n", count++,nErrCode, strStore.c_str());
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}
/**************************************************************************//**
*
* \param   - char *buffer - ID
*
******************************************************************************/
void DBAccessL20DB::QueryWeldResult(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RESULT)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);

    if(str.size()>0)
        printf("QueryWeldResult:\n%s\n", str.c_str());
    return;
}
/**************************************************************************//**
*
* \param   - char *buffer - WeldResultID
*
******************************************************************************/
void DBAccessL20DB::QueryWeldSignature(char *buffer)
{
    string strQuery =
        "select "+string(COLUMN_GRAPHDATA)+
        " from "+string(TABLE_WELD_SIGNATURE)+
        " where WeldResultID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);
    vector<WELD_SIGNATURE> WeldSignVector;
    json2Vector(str.c_str(), WeldSignVector);
    return;
}
/**************************************************************************//**
*
* \param   - char *buffer - not used
*
******************************************************************************/
void DBAccessL20DB::QueryWeldRecipeAll(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+";";
    string str = ExecuteQuery(strQuery);

    if(str.size()>0)
        printf("QueryWeldRecipe:\n%s\n", str.c_str());
    return;
}
/**************************************************************************//**
*
* \param   - char *buffer - ID
*
******************************************************************************/
void DBAccessL20DB::QueryWeldRecipe(char *buffer)
{
    string strQuery =
        "select * from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    string str = ExecuteQuery(strQuery);
    if(str.size()>0)
        printf("QueryWeldRecipe:\n%s\n", str.c_str());

    strQuery =
        "select EnergyToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    json2Struct(str.c_str(), NULL);

    strQuery =
        "select TimeToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    json2Struct(str.c_str(), NULL);

    strQuery =
        "select PowerToStep from "+string(TABLE_WELD_RECIPE)+
        " where ID="+
        std::to_string(*(int *)buffer)+";";
    str = ExecuteQuery(strQuery);
    json2Struct(str.c_str(), NULL);
    return;
}
/**************************************************************************//**
*
* \param   - char *buffer - ID
*
******************************************************************************/
int DBAccessL20DB::UpdateWeldRecipe(char *buffer)
{
	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

#ifdef UNITTEST_DATABASE
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_Order = 3;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_StepValue = 2;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[0].m_AmplitudeValue = 1;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_Order = 6;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_StepValue = 5;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[1].m_AmplitudeValue = 4;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_Order = 9;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_StepValue = 8;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep[4].m_AmplitudeValue = 7;

    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TimeStep[0].m_Order = 333;
    CommonProperty::ActiveRecipeSC.m_WeldParameter.m_PowerStep[0].m_Order = 444;
#endif
    char *jsonEnergy = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_EnergyStep, STEP_MAX);
    char *jsonTime = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TimeStep, STEP_MAX);
    char *jsonPower = struct2Json(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_PowerStep, STEP_MAX);

	string strStore =
        "update " + string(TABLE_WELD_RECIPE) +
        " set UserID="+std::to_string(0)+//userID
        ", IsValidate="+std::to_string(CommonProperty::ActiveRecipeSC.m_IsTeachMode)+//IsValidate
        ", Amplitude="+std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_Amplitude)+//Amplitude
        ", Width="+std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WidthSetting)+//Width
        ", WeldPressure="+std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_WPpressure)+//WeldPressure
        ", TriggerPressure="+std::to_string(CommonProperty::ActiveRecipeSC.m_WeldParameter.m_TPpressure)+//TriggerPressure
        ", TimePlus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMax)+//TimePlus
        ", TimeMinus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_TimeMin)+//TimeMinus
        ", PeakPowerPlus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMax)+//PeakPowerPlus
        ", PeakPowerMinus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PeakPowerMin)+//PeakPowerMinus
        ", TriggerHeightPlus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PreHeightMax)+//TriggerHeightPlus
        ", TriggerHeightMinus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_PreHeightMin)+//TriggerHeightMinus
        ", WeldHeightPlus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_HeightMax)+//WeldHeightPlus
        ", WeldHeightMinus="+std::to_string(CommonProperty::ActiveRecipeSC.m_QualityWindowSetting.m_HeightMin)+//WeldHeightMinus
        ", WeldMode="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_WeldMode)+//WeldMode
        ", ModeValue="+std::to_string(0)+//ModeValue
        ", PreBurst="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_PreBurst)+//PreBurst
        ", HoldTime="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_HoldTime)+//HoldTime
        ", SqueezeTime="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_SqueezeTime)+//SqueezeTime
        ", AfterBurstDelay="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstDelay)+//AfterBurstDelay
        ", AfterBurstDuration="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstTime)+//AfterBurstDuration
        ", AfterBurstAmplitude="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_AfterBurstAmplitude)+//AfterBurstAmplitude
        ", WeldHeight="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_DisplayedHeightOffset)+//WeldHeight
        ", MeasuredHeight="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_MeasuredHeightOffset)+//MeasuredHeight
        ", StepWeldMode="+std::to_string(CommonProperty::ActiveRecipeSC.m_AdvancedSetting.m_WeldStepMode)+//StepWeldMode
        ", EnergyToStep='"+jsonEnergy+//EnergyToStep
        "', TimeToStep='"+jsonTime+//TimeToStep
        "', PowerToStep='"+jsonPower+//PowerToStep
        "', RecipeName='"+CommonProperty::ActiveRecipeSC.m_RecipeName+//RecipeName
        "', DateTime='"+timeBuf+//DateTime
        "', PresetPicPath='"+CommonProperty::ActiveRecipeSC.m_RecipePicPath+//PresetPicPath
        "' where ID="+std::to_string(*(int *)buffer)+";";
    free(jsonEnergy);
    free(jsonTime);
    free(jsonPower);

	int nErrCode = SingleTransaction(strStore);
#ifdef UNITTEST_DATABASE
    static int count=0;
    printf("#WeldRecipe(len %d): result %d\n%s\n\n", strStore.size(),nErrCode, strStore.c_str());
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}
/**************************************************************************//**
*
* \param   - char *table - table name
*
******************************************************************************/
void DBAccessL20DB::DeleteOldest(const char *table)
{
    string strQuery =
        "delete from "+
        string(table)+" where ID in (select ID from "+
        string(table)+" order by ID asc limit 1);";
    SingleTransaction(strQuery);
    return;
}
char *DBAccessL20DB::struct2Json(WeldStepValueSetting *step, int num)
{
    char *result;
    if((step==NULL)||(num<=0)||(num>STEP_MAX))
        {
        return NULL;
        }

    json_t *array1 = json_array();
    json_t *array2 = json_array();
    json_t *array3 = json_array();
    json_t *all  = json_object();

    for(UINT32 i = 0; i < num; i++)
        {
        json_array_append_new(array1, json_integer(step[i].m_Order));
        json_array_append_new(array2, json_integer(step[i].m_StepValue));
        json_array_append_new(array3, json_integer(step[i].m_AmplitudeValue));
        }
    json_object_set_new(all, "0", array1);
    json_object_set_new(all, "1", array2);
    json_object_set_new(all, "2", array3);

    result = json_dumps(all, 0);

    json_delete(all);
    return result;
}
void DBAccessL20DB::json2Struct(const char *json, WeldStepValueSetting *)
{
    int i;
    void *each;
    json_t *all;
    json_t *array;
    json_t *mem;

    if(json == NULL)
        return;
    all = json_loads(json, 0, NULL);
    if(all == NULL)
        {
        printf("invalid json\n");
        return;
        }

    array = json_object_get(all, "0");
    printf("\tOrder: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    array = json_object_get(all, "1");
    printf("\tStepValue: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    array = json_object_get(all, "2");
    printf("\tAmplitudeValue: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    json_delete(all);
    return;
    }

char *DBAccessL20DB::vector2Json(vector<WELD_SIGNATURE> WeldSignatureVector)
{
    char *result;
    if(WeldSignatureVector.size()==0)
        {
        return NULL;
        }

    json_t *array1 = json_array();
    json_t *array2 = json_array();
    json_t *array3 = json_array();
    json_t *array4 = json_array();
    json_t *all  = json_object();

    for(UINT32 i = 0; i < WeldSignatureVector.size(); i++)
        {
        json_array_append_new(array1, json_integer(WeldSignatureVector[i].Frquency));
        json_array_append_new(array2, json_integer(WeldSignatureVector[i].Power));
        json_array_append_new(array3, json_integer(WeldSignatureVector[i].Height));
        json_array_append_new(array4, json_integer(WeldSignatureVector[i].Amplitude));
        }
    json_object_set_new(all, "0", array1);
    json_object_set_new(all, "1", array2);
    json_object_set_new(all, "2", array3);
    json_object_set_new(all, "3", array4);

    result = json_dumps(all, 0);

    json_delete(all);
    return result;
}
void DBAccessL20DB::json2Vector(const char *json, vector<WELD_SIGNATURE> &WeldSignVector)
{
    int i;
    void *each;
    json_t *all;
    json_t *array;
    json_t *mem;

    if(json == NULL)
        return;
    printf("QueryWeldSignature: %s\n", json);
    all = json_loads(json, 0, NULL);
    if(all == NULL)
        {
        printf("invalid json\n");
        return;
        }

    array = json_object_get(all, "0");
    printf("\tFrquency: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    array = json_object_get(all, "1");
    printf("\tPower: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    array = json_object_get(all, "2");
    printf("\tHeight: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");

    array = json_object_get(all, "3");
    printf("\tAmplitude: ");
    for(i=0; i<json_array_size(array); i++)
        {
        mem = json_array_get(array,i);
        printf("%d ", json_integer_value(mem));
        }
    printf("\n");
    json_delete(all);
    return;
    }

