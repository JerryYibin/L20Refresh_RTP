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
#define FREQ_FACTOR		((20450.0 - 19450.0)/ 0xFFFF)
#define FREQ_MAX		20450.0
#define DAC_FACTOR 		0x03FF
#define PRESSURE_MAX	120000
#define AMPLITUDE_MAX	72
#include "Common.h"
#include <vector>
using namespace std;
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
	
	static int 	  Average				(const int* data, const int size);
	
	static int    StringToTokens        (const string strData, const char delimiter, vector<string>&vTokens);
	static int    TokensToString        (const vector<string>&vTokens, const char delimiter, string &strData);
};

#endif /* UTILITY_H_ */
