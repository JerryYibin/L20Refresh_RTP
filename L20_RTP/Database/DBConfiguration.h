/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

/*------------------------------ DESCRIPTION --------------------------------

     All database related configuration and setting will be stored here.
     
----------------------------------------------------------------------------*/

#ifndef DBCONFIGURATION_H_
#define DBCONFIGURATION_H_

#define DB_RECORDS_STORAGE_WELD_RECIPE_LIMIT	1000
#define DB_RECORDS_STORAGE_WELD_RESULT_LIMIT	1000000
#define DB_RECORDS_STORAGE_WELD_SIGNAT_LIMIT	5000

//TABLE AND COLUMN NAMES IN DB
#define TABLE_WELD_RESULT		"WeldResult"
#define TABLE_WELD_RECIPE		"WeldRecipe"
#define TABLE_WELD_SIGNATURE	"WeldResultSignature"
#define TABLE_ALARM_LOG     	"AlarmLog"
#define TABLE_HI_CALIB     		"HeightCalibration"
#define TABLE_DB_VERSION     	"DBVersion"
#define TABLE_USER_PROFILE     	"UserProfiles"
#define TABLE_PRIVILEGE_CONFIG  "PrivilegeConfiguration"
#define TABLE_PWR_SUPPLY     	"PowerSupply"
#define TABLE_TEACH_MODE_SET 	"TeachModeSetting"
#define TABLE_SYS_CONFIG     	"SystemConfigure"
#define COLUMN_GRAPHDATA		"WeldGraph"
#define COLUMN_GOLDEN			"Golden"



#endif /* DBCONFIGURATION_H_ */
