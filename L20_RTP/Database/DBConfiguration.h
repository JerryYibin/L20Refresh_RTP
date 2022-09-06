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

#define DB_RECORDS_STORAGE_WELD_RESULT_LIMIT	200000
#define DB_QUERY_SIZE							1024

//TABLE AND COLUMN NAMES IN DB
#define TABLE_WELD_RESULT		"WeldResultTable"
#define TABLE_WELD_RECIPE		"WeldRecipeTable"
#define TABLE_WELD_SIGNATURE	"WeldResultSignatureTable"
#define COLUMN_GRAPHDATA		"GraphData"
#define COLUMN_GOLDEN			"Golden"



#endif /* DBCONFIGURATION_H_ */
