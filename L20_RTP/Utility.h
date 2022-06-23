/*
 * Utility.h
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#ifndef UTILITY_H_
#define UTILITY_H_
#define POWER_FACTOR 	(3.0 * 0xFFFF / 4.0)
#define POWER_MAX		4000
#define DAC_FACTOR 		0x03FF
#define PRESSURE_MAX	120000
#define AMPLITUDE_MAX	72
#include "Common.h"
class Utility {
public:
	Utility();
	virtual ~Utility();
	
	static UINT32 HEX2Pressure			(UINT16 hexadecimal);	
	static UINT32 HEX2Power				(UINT16 hexadcimal);
	static UINT32 HEX2Energy			(UINT32 hexadcimal);
	static UINT32 HEX2Frequency			(UINT16 hexadcimal);
	static UINT32 HEX2Amplitude			(UINT16 hexadcimal);
	static UINT32 HEX2Memory			(UINT16 hexadcimal);
	static UINT32 HEX2Reference			(UINT16 hexadcimal);
	static UINT32 HEX2Height			(UINT16 hexadcimal);
	static UINT32 HEX2Width				(UINT16 hexadcimal);
	
	static UINT16 TunePoint2HEX			(UINT32 physical);
	static UINT16 Amplitude2HEX			(UINT32 physical);
	static UINT16 FrequencyOffset2HEX	(INT32 physical);
	static UINT16 Pressure2HEX			(UINT32 physical);
	static UINT32 Energy2HEX			(UINT32 physical);
	
	
};

#endif /* UTILITY_H_ */
