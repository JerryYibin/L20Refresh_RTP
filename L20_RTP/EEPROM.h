/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_
#include <string.h>
#include <iostream>
#include <ioLib.h>
#include <stdio.h>
#include <taskLib.h>
#include "SystemType.h"
#include "SystemConfigDefine.h"
#define SER_LEN						12
//#define EEPROM_DEV_NAME             "/eeprom/0"
#define EEPROM_DATA_START_ADDR      0
using namespace std;
class EEPROM {
public:
	struct SYSTEM_CONFIG
	{
		EN_SYSTEM_TYPE 			Type;
		SYSTEMCONFIG::FREQUENCY	Frequency;
		SYSTEMCONFIG::POWER 	Power;
		int 					TunePoint;
		int 					FrequencyOffset;
		char 					SeriesNumber[SER_LEN];
		int 					CRC;
	};
public:
	static SYSTEM_CONFIG* _System;
public:
	static EEPROM*	GetInstance					();
	virtual 		~EEPROM						();
	int				Read						();
	int 			Write						();
	EEPROM				(const EEPROM&) 	= delete;
	EEPROM& operator=	(const EEPROM&) 	= delete;
private:
	static EEPROM* 			_EepromObj;
	static string 			EEPROM_DEV_NAME;
private:
			EEPROM			();
	void 	init			();
	int 	eepromIoRead	(const char *devName, const UINT32 startAddr, const UINT32 length, void *pRdBuf);
	int 	eepromIoWrite	(const char *devName, const UINT32 startAddr, const UINT32 length, const void *pWrBuf);
};

#endif /* EEPROM_H_ */
