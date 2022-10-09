/*
 * Utility.cpp
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#include "Utility.h"
#include <sstream>
Utility::Utility() {
	// TODO Auto-generated constructor stub

}

Utility::~Utility() {
	// TODO Auto-generated destructor stub
}


UINT32 Utility::HEX2Pressure(UINT16 hexadecimal)
{
	return hexadecimal * PRESSURE_MAX / DAC_FACTOR;
}

UINT32 Utility::HEX2Power(UINT16 hexadcimal)
{
	return hexadcimal * POWER_MAX / POWER_FACTOR;
}

UINT32 Utility::HEX2Energy(UINT32 hexadcimal)
{
	float tmp = (float)(hexadcimal / POWER_FACTOR);
	return  tmp * (float)(POWER_MAX / 1000); 
}

UINT32 Utility::HEX2Frequency(UINT16 hexadcimal)
{
	return 0;
}

UINT32 Utility::HEX2Amplitude(UINT16 hexadcimal)
{
	return hexadcimal * AMPLITUDE_MAX / DAC_FACTOR;
}

UINT32 Utility::HEX2Memory(UINT16 hexadcimal)
{
	return 0;
}

UINT32 Utility::HEX2Reference(UINT16 hexadcimal)
{
	return 0;
}


UINT32 Utility::HEX2Width(UINT16 hexadcimal)
{
	return 0;
}


UINT16 Utility::TunePoint2HEX(UINT32 physical)
{
	return 0;
}

UINT16 Utility::Amplitude2HEX(UINT32 physical)
{
	return physical * DAC_FACTOR / AMPLITUDE_MAX;
}

UINT16 Utility::FrequencyOffset2HEX(INT32 physical)
{
	return 0;
}

UINT16 Utility::Pressure2HEX(UINT32 physical)
{
	return physical * DAC_FACTOR / PRESSURE_MAX;
}

UINT32 Utility::Energy2HEX(UINT32 physical)
{
	float tmp = physical * (float)(1000.00 / POWER_MAX);
	return tmp * (float)POWER_FACTOR;
}

/**************************************************************************//**
* \brief   - The data average calculation.	                            
*
* \param   - data is the source, data size
*
* \return  - If there is not any issue during data calculating, it will be return average value; else it will return ERROR.
*
******************************************************************************/
int Utility::Average(const int* data, const int size)
{
	unsigned int sum = 0;
	int iResult = 0;
	if(data == nullptr)
		return 0;
	if(size == 0)
		return 0;
	
	for(int i = 0; i < size; i++)
	{
		sum += *data;
		data++;
	}
	iResult = sum / size;
	return iResult;
}

/**************************************************************************//**
 * \brief  - Tokenise the string with given delimiter and stores it in vector
 * 
 * \param  - const char *pData, vector<string>&vTokens,char delimiter
 *
 * \return  - int
 *
 ******************************************************************************/
int Utility::StringToTokens(const string strData, const char delimiter, vector<string>&vTokens)
{
	stringstream sstream(strData);
	string strToken;
	int dwTokeCnt = 0;

	while(getline(sstream, strToken, delimiter))
	{
		dwTokeCnt++;
		vTokens.push_back(strToken);
	}

	return dwTokeCnt;
}

/**************************************************************************//**
 * \brief  - vector data will be stored in string with given delimiter
 * 
 * \param  - string &strData, vector<string>&vTokens,char delimiter
 *
 * \return  - int
 *
 ******************************************************************************/
int Utility::TokensToString(const vector<string>&vTokens, const char delimiter, string &strData)
{
	stringstream sstream (strData);
	string strToken;
	int dwTokeCnt = 0,dwIndex = 0;
	dwTokeCnt = vTokens.size();
	strData.clear();
	
	/* Inserting upto last but one elements */
	for(dwIndex = 0;dwIndex < dwTokeCnt-1;dwIndex++)
	{
		strData.append(vTokens[dwIndex].c_str());
		strData.append(1,delimiter);
	}
	/* Inserting the last element */
	strData.append(vTokens[dwIndex].c_str());

	return dwTokeCnt;
}
