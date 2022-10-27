/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

class EEPROM {
public:
	enum SYSTEM_TYPE
	{
		INVALID_SYSTEM = -1,
		GSXP1,
		GSXE1,
		L20
	};
	struct SYSTEM_CONFIG
	{
		int SystemType;
		int SystemFrequency;
		int SystemPower;
		int SystemTunePoint;
		int SystemFrequencyOffset;
		int SystemSeriesNumber;
	};
public:
	EEPROM();
	virtual ~EEPROM();
};

#endif /* EEPROM_H_ */
