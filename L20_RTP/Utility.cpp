/*
 * Utility.cpp
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#include "Utility.h"

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
