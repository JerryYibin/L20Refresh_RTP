/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef EXTERNALDATA_H_
#define EXTERNALDATA_H_
#include "Common.h"
#define MAX_LEN		2048
class ExternalData 
{

public:
	struct ETHMESSAGE
	{
		UINT32	msgID;
		UINT32 	Length;
		char	Buffer[MAX_LEN];	
	};
	enum DATATYPE
	{
		WELDDATA = 1,
		POWERCURVE,
		HEIGHTCURVE,
		FREQUENCYCURVE,
		PRESET,
		SYSCONFIG,
		PWRCURVESEC1,
		PWRCURVESEC2,
		PWRCURVESEC3,
		PWRCURVESEC4,
		HEARTBEAT
	};
	ExternalData();
	virtual int BuildProtocolData(int, char*) = 0;

	virtual 				~ExternalData	();
	ExternalData	(const ExternalData&) 			= delete;
	ExternalData& operator=(const ExternalData&) 	= delete;



};





#endif /* EXTERNALDATA_H_ */
