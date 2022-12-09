/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef EXTERNALDATATCP_H_
#define EXTERNALDATATCP_H_
#include "ExternalData.h"
#include "Common.h"
#define L20REFRESHACTUATOR 1
class ExternalDataTCP :public ExternalData
{
public:

	//TODO:Verify the real INT32 or UINT32 
	struct WeldData
	{
		UINT32 		CycleCount;
		UINT32 		ActuatorType;
		UINT32		PowerSupply;
		UINT32		Alarm;
		UINT32		AmplitudeSetting;
		INT32 		EnergySetting;
		INT32 		TimeSetting;
		UINT32		TriggerPressureSetting;
		UINT32		WeldPressureSetting;
		UINT32		WeldModeSetting;
		UINT32		ActualWeldTime;
		INT32 		ActualWeldEnergy;
		INT32 		ActualPeakPower;
		UINT32		ActualPreHeight;
		UINT32		ActualPostHeight;
		UINT32		QualityTimeUpper;	
		UINT32		QualityTimeLower;
		UINT32		QualityPowerUpper;	
		UINT32		QualityPowerLower;	
		UINT32		QualityPreHeightUpper;
		UINT32		QualityPreHeightLower;
		UINT32		QualityPostHeightUpper;
		UINT32		QualityPostHeightLower;
	};

	virtual int BuildProtocolData(int, char*) override;
	virtual ~ExternalDataTCP();
	static ExternalDataTCP* GetInstance();

private:
	ExternalDataTCP();	
	static ExternalDataTCP* _objTCPData;
	
	ExternalDataTCP	(const ExternalDataTCP&) 			= delete;
	ExternalDataTCP& operator=(const ExternalDataTCP&) 	= delete;



};






#endif /* EXTERNALDATATCP_H_ */
