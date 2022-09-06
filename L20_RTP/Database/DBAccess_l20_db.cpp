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
	WELD_RESULT *pResult = (WELD_RESULT *)buffer;
	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

	sprintf(insertQuery, string(strInsert + strWeldResultTableFormat).c_str(), 
			TABLE_WELD_RESULT,
			pResult->PartID,
			timeBuf,
			pResult->RecipeNum,
			pResult->TotalEnergy,
			pResult->TriggerPressure,
			pResult->WeldPressure,
			pResult->Amplitude,
			pResult->WeldTime,
			pResult->PeakPower,
			pResult->PreHeight,
			pResult->PostHeight,
			pResult->ALARMS.ALARMflags,
			0,//sequence ID
			pResult->CycleCounter
			);
	nErrCode = SingleTransaction((string)insertQuery);
#ifdef UNITTEST_DATABASE
    static int count=0;
    printf("#WeldResult(%d): result %d - %s\n\n", count++,nErrCode, insertQuery);
#endif
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
	int nErrCode = SQLITE_ERROR;
	char insertQuery[DB_QUERY_SIZE] = {0x00};
	int *WeldResultID = (int *)buffer;

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
    	sprintf(insertQuery, string(strInsert + strWeldSignatureFormat).c_str(), 
    			TABLE_WELD_SIGNATURE,
    			*WeldResultID,
    			json
    			);
        free(json);
    	nErrCode = SingleTransaction((string)insertQuery);
#ifdef UNITTEST_DATABASE
        static int count=0;
        printf("#WeldSignature(%d): result %d - %s\n\n", count++,nErrCode, insertQuery);
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
	int nErrCode = SQLITE_ERROR;
	char insertQuery[DB_QUERY_SIZE] = {0x00};
	WeldRecipeSC recipe;
	struct tm timeStamp;
    char timeBuf[20];
	vxbRtcGet(&timeStamp);
    strftime(timeBuf, 20, "%Y-%m-%d %H:%M:%S", &timeStamp);

	memcpy(&recipe, buffer, sizeof(WeldRecipeSC));

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
			0,//ModeValue
			recipe.m_AdvancedSetting.m_PreBurst,
			recipe.m_AdvancedSetting.m_HoldTime,
			recipe.m_AdvancedSetting.m_SqueezeTime,
			recipe.m_AdvancedSetting.m_AfterBurstDelay,
			recipe.m_AdvancedSetting.m_AfterBurstTime,
			recipe.m_AdvancedSetting.m_AfterBurstAmplitude,
			recipe.m_AdvancedSetting.m_DisplayedHeightOffset,
			recipe.m_AdvancedSetting.m_MeasuredHeightOffset,
			recipe.m_AdvancedSetting.m_WeldStepMode,
			jsonEnergy,
			jsonTime,
			jsonPower,
			"m_RecipeName",
			timeBuf,
			"m_RecipePicPath"
			);
    free(jsonEnergy);
    free(jsonTime);
    free(jsonPower);

	nErrCode = SingleTransaction((string)insertQuery);
#ifdef UNITTEST_DATABASE
    static int count=0;
    printf("#WeldRecipe(%d): result %d - %s\n\n", count++,nErrCode, insertQuery);
#endif
	if(nErrCode != 0)
		LOGERR((char*) "Database_T: Single Transaction Error. %d\n", nErrCode, 0, 0);
	return nErrCode;
}
void DBAccessL20DB::QueryWeldResult(char *buffer)
{
    char cmd[100];
	int *id = (int *)buffer;

    sprintf(cmd, "select * from %s where ID=%d;", TABLE_WELD_RESULT, *id);
    string str = ExecuteQuery(cmd);
    printf("QueryWeldResult: %s\n", str.c_str());
    return;
}
void DBAccessL20DB::QueryWeldSignature(char *buffer)
{
    char cmd[100];
	int *WeldResultID = (int *)buffer;
    vector<WELD_SIGNATURE> WeldSignVector;
    
    sprintf(cmd, "select WeldGraph from %s where WeldResultID=%d;", TABLE_WELD_SIGNATURE, *WeldResultID);
    string str = ExecuteQuery(cmd);
    json2Vector(str.c_str(), WeldSignVector);
    return;
}
void DBAccessL20DB::QueryWeldRecipe(char *buffer)
{
    char cmd[100];
	int *id = (int *)buffer;

    sprintf(cmd, "select * from %s where ID=%d;", TABLE_WELD_RECIPE, *id);
    string str = ExecuteQuery(cmd);
    printf("QueryWeldRecipe: %s\n", str.c_str());

    sprintf(cmd, "select EnergyToStep from %s where ID=%d;", TABLE_WELD_RECIPE, *id);
    str = ExecuteQuery(cmd);
    printf("EnergyToStep: %s\n", str.c_str());
    json2Struct(str.c_str(), NULL);

    sprintf(cmd, "select TimeToStep from %s where ID=%d;", TABLE_WELD_RECIPE, *id);
    str = ExecuteQuery(cmd);
    printf("TimeToStep: %s\n", str.c_str());
    json2Struct(str.c_str(), NULL);

    sprintf(cmd, "select PowerToStep from %s where ID=%d;", TABLE_WELD_RECIPE, *id);
    str = ExecuteQuery(cmd);
    printf("PowerToStep: %s\n", str.c_str());
    json2Struct(str.c_str(), NULL);
    return;
}
void DBAccessL20DB::DeleteOldest(const char *table)
{
    char cmd[200];
    sprintf(cmd, "delete from %s where ID in (select ID from %s order by ID asc limit 1);",
        table,table);
    SingleTransaction((string)cmd);
    return;
}
#ifdef UNITTEST_DATABASE
void DBAccessL20DB::ClearTable(const char *table)
{
    int sta;
    char cmd[200];

    sprintf(cmd, "delete from %s;", table);
    sta = SingleTransaction((string)cmd);
    printf("clear %s: %d\n", table, sta);

    sprintf(cmd, "UPDATE sqlite_sequence SET seq = 0 WHERE name='%s';", table);
    sta = SingleTransaction((string)cmd);
    printf("sqlite_sequence %s: %d\n", table, sta);
    return;
}
#endif
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

