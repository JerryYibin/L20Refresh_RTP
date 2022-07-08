/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef DBACCESS_COMMON_DB_H_
#define DBACCESS_COMMON_DB_H_

#include "DBAccess.h"

//Class DBAccessCommonDB. SQLiteDB is its parent class.
class DBAccessCommonDB : public SQLiteDB
{
public:	
	DBAccessCommonDB();
	~DBAccessCommonDB();
	
	int ConnectDB() override;		
};

#endif //DBACCESS_COMMON_DB_H_
