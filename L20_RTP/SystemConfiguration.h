/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef SYSTEMCONFIGURATION_H_
#define SYSTEMCONFIGURATION_H_
#include "SystemConfigDefine.h"
#include "SystemConfigurationUI.h"
#include <vector>
using namespace std;

#define TABLE_PWR_SUP_MEM 3
#define TABLE_TEACH_MODE_MEM 11

class SystemConfiguration 
{
public:
	static SYSTEMCONFIG* _SystemConfig;
	static vector<POWER_SUPPLY_TYPE> PowerSupplyType;
	static vector<TEACH_MODE_SETTING> TeachModeSetting;
public:
	SystemConfiguration();
	virtual ~SystemConfiguration();
	
	static int Set(const SYSTEMCONFIGFORUI* _sysConfigUI);
	static int Get(SYSTEMCONFIGFORUI* _sysConfigUI);
	
	
};

#endif /* SYSTEMCONFIGURATION_H_ */
