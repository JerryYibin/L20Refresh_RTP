/*
 * Utility.cpp
 *
 *  Created on: May 13, 2022
 *      Author: JerryW.Wang
 */

#include "Utility.h"
#include <jansson.h>
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
* \brief   - The struct2Json shall be overload function for WeldStepValueSetting.
* 			 The format of JSON shall be 
* 			 {"Order1": [ModeValue1, Amplitude1], 
* 			  "Order2": [ModeValue2, Amplitude2],
* 			  "Order3": [ModeValue3, Amplitude3]
* 			 }                             
*
* \param   - WeldStepValueSetting, size and output JSONString
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int	Utility::Struct2JSON(const WeldStepValueSetting* _ptrArray, const unsigned int size, string& jsonStr)
{
    char *result;
    if((_ptrArray == nullptr) || (size == 0) || (size > STEP_MAX))
	{
        return ERROR;
	}

    json_t *jsonArray[size];
    for(int i = 0; i < size; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t *all  = json_object();

    for(UINT32 i = 0; i < size; i++)
	{
        json_array_append_new(jsonArray[i], json_integer(_ptrArray[i].m_StepValue));
        json_array_append_new(jsonArray[i],	json_integer(_ptrArray[i].m_AmplitudeValue));
        json_object_set_new(all, std::to_string(i).c_str(), jsonArray[i]);
	}
    
    result = json_dumps(all, 0);

    json_delete(all);
    jsonStr = result;
    return OK;
}

/**************************************************************************//**
* \brief   - The json2struct shall be overload function for WeldStepValueSetting.
* 			 The format of JSON shall be 
* 			 {"Order1": [ModeValue1, Amplitude1], 
* 			  "Order2": [ModeValue2, Amplitude2],
* 			  "Order3": [ModeValue3, Amplitude3]
* 			 }                             
*
* \param   - JSONString and WeldStepValueSetting
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int Utility::JSON2Struct(const string jsonStr, WeldStepValueSetting* _ptrArray)
{
    json_t *all;
    json_t *array;
    json_t *mem;
    int index = 0;

    if(_ptrArray == nullptr)
	{
        printf("invalid _ptrArray\n");
    	return ERROR;
	}

    all = json_loads(jsonStr.c_str(), 0, NULL);
    if(all == NULL)
    {
        printf("invalid json\n");
        return ERROR;
    }
    
    for(int i = 0; i < STEP_MAX; i++)
    {
    	array = json_object_get(all, std::to_string(i).c_str());
    	index = 0;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		_ptrArray[i].m_StepValue = json_integer_value(mem);
    	}
    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		_ptrArray[i].m_AmplitudeValue = json_integer_value(mem);
    	}
    }
    
    json_delete(all);
    return OK;
}

/**************************************************************************//**
* \brief   - The vector2Json is the function for the data converting from vector to JSON.
* 			 The format of JSON shall be 
* 			 {"Order1": [Frequency1, Power1, Height1, Amplitude1], 
* 			  "Order2": [Frequency2, Power2, Height2, Amplitude2],
* 			  "Order3": [Frequency3, Power3, Height3, Amplitude3]
* 			 }                             
*
* \param   - vector<WELD_SIGNATURE> pointer, and output JSONString
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int Utility::Vector2JSON(const vector<WELD_SIGNATURE>* _ptrVector, string& jsonStr)
{
    char *result;
    if((_ptrVector->size() == 0) || (_ptrVector->size() > HMI_SIGNA_POINT_MAX))
	{
        return ERROR;
	}
    
    json_t *jsonArray[SIGNATURE_DATA_TYPE::TOTAL] = {nullptr};
    for(int i = 0; i < SIGNATURE_DATA_TYPE::TOTAL; i++)
    {
    	jsonArray[i] = json_array();
    }
    json_t *all  = json_object();

    for(int i = 0; i < _ptrVector->size(); i++)
	{
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Frquency));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Power));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Height));
        json_array_append_new(jsonArray[i], json_integer(_ptrVector->at(i).Amplitude));
        json_object_set_new(all, std::to_string(i).c_str(), jsonArray[i]);
	}

    result = json_dumps(all, 0);

    json_delete(all);
    jsonStr = result;
    return OK;
}

/**************************************************************************//**
* \brief   - The json2Vector is the function for the data converting from JSON to vector.
* 			 The format of JSON shall be 
* 			 {"Order1": [Frequency1, Power1, Height1, Amplitude1], 
* 			  "Order2": [Frequency2, Power2, Height2, Amplitude2],
* 			  "Order3": [Frequency3, Power3, Height3, Amplitude3]
* 			 }                             
*
* \param   - input JSON string output vector<WELD_SIGNATURE> pointer
*
* \return  - If there is not any issue during data converting, it will be return OK; else it will return ERROR.
*
******************************************************************************/
int Utility::JSON2Vector(const string jsonStr, vector<WELD_SIGNATURE>* _ptrVector)
{
    int size = 0, index = 0;
    json_t *all, *array, *mem;
    WELD_SIGNATURE signature;

    if(jsonStr.empty() == true)
        return ERROR;
    if(_ptrVector == nullptr)
    	return ERROR;

    all = json_loads(jsonStr.c_str(), 0, NULL);
    if(all == NULL)
	{
        printf("invalid json\n");
        return ERROR;
	}

    for(int i = 0; (array = json_object_get(all, std::to_string(i).c_str()))!=NULL; i++)
    {
    	index = 0;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Frquency = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Power = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Height = json_integer_value(mem);
    	}

    	index++;
    	if(index < json_array_size(array))
    	{
    		mem = json_array_get(array, index);
    		signature.Amplitude = json_integer_value(mem);
    	}
    	
    	_ptrVector->push_back(signature);
    }
    
    json_delete(all);
    return OK;
}

