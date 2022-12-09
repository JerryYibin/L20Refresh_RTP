/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef EXTERNALDATADIG_H_
#define EXTERNALDATADIG_H_
#include "ExternalData.h"
#include "Common.h"
class ExternalDataDIG :public ExternalData
{
public:
	virtual int BuildProtocolData(int, char*) override;
	virtual ~ExternalDataDIG();
	static ExternalDataDIG* GetInstance();

private:
	ExternalDataDIG();	
	static ExternalDataDIG* _objDIGData;
	
	ExternalDataDIG	(const ExternalDataDIG&) 			= delete;
	ExternalDataDIG& operator=(const ExternalDataDIG&) 	= delete;


};


#endif /* EXTERNALDATADIG_H_ */
