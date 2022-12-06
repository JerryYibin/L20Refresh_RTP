/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

 Implement the functions in the WeldResultsDefine.h file
 
***************************************************************************/
#include <iostream>
#include "WeldResultsDefine.h"
#include "WeldResults.h"
#include "SystemType.h"
#include "commons.h"

std::shared_ptr<WeldResults> WeldResults::_WeldResults =  WeldResults::GetWeldResults();

/**************************************************************************//**
* 
* \brief   - Select different structures for different devices.
*
* \param   - None.
*
* \return  - Structure pointer.
*
******************************************************************************/
std::shared_ptr<WeldResults> WeldResults::GetWeldResults() {
	std::shared_ptr<WeldResults> ptrWeldResults = nullptr;
	switch (GetSystemType()) {
	case GSXP1: {
		ptrWeldResults = std::make_shared<P1_WeldResults>();
	}
		break;

	case L20: {
		ptrWeldResults = std::make_shared<L20_WeldResults>();
	}
		break;

	default: {
		std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
		break;
	}

	return ptrWeldResults;
}

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResults::WeldResults() {

}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResults::~WeldResults() {

}

/**************************************************************************//**
* 
* \brief   - Set data to L20 Weld Results.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_WeldResults::Set(int indlx, void *para) {
	int iResult = OK;
	if (nullptr != para) {
		switch (indlx) {
		case WeldResults::PARALIST::TRIGGER_PRESSURE:
			memcpy(&this->TriggerPressure, para, sizeof(int));
			break;
		case WeldResults::PARALIST::WELD_PRESSURE:
			memcpy(&this->WeldPressure, para, sizeof(int));
			break;
		case WeldResults::PARALIST::PRE_HEIGHT:
			memcpy(&this->PreHeight, para, sizeof(int));
			break;
		case WeldResults::PARALIST::POST_HEIGHT:
			memcpy(&this->PostHeight, para, sizeof(int));
			break;
		case WeldResults::PARALIST::ALARM_ID:
			memcpy(&this->AlarmID, para, sizeof(int));
			break;
		case ENERGY_SETTING:
			memcpy(&this->EnergySetting, para, sizeof(int));
			break;
		case AMPLITUDE_SETTING:
			memcpy(&this->AmplitudeSetting, para, sizeof(int));
			break;
		case T_PRESSURE_SETTING:
			memcpy(&this->TPressureSetting, para, sizeof(int));
			break;
		case W_PRESSURE_SETTING:
			memcpy(&this->WPressureSetting, para, sizeof(int));
			break;
		case MAX_WELD_TIME:
			memcpy(&this->MaxWeldTime, para, sizeof(int));
			break;
		case MIN_WELD_TIME:
			memcpy(&this->MinWeldTime, para, sizeof(int));
			break;
		case MAX_POWER:
			memcpy(&this->MaxPower, para, sizeof(int));
			break;
		case MIN_POWER:
			memcpy(&this->MinPower, para, sizeof(int));
			break;
		case MAX_PRE_HEIGHT:
			memcpy(&this->MaxPreHeight, para, sizeof(int));
			break;
		case MIN_PRE_HEIGHT:
			memcpy(&this->MinPreHeight, para, sizeof(int));
			break;
		case MAX_POST_HEIGHT:
			memcpy(&this->MaxPostHeight, para, sizeof(int));
			break;
		case MIN_POST_HEIGHT:
			memcpy(&this->MinPostHeight, para, sizeof(int));
			break;
		default:
			iResult = ERROR;
			break;
		}
	} else {
		iResult = ERROR;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Get data from L20 Weld Results.
*
* \param   - Index: data index; _data: output data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_WeldResults::Get(int indlx, void *para) {
	int iResult = OK;
	if (nullptr != para) {
		switch (indlx) {
		case WeldResults::PARALIST::TRIGGER_PRESSURE:
			memcpy(para, &this->TriggerPressure, sizeof(int));
			break;
		case WeldResults::PARALIST::WELD_PRESSURE:
			memcpy(para, &this->WeldPressure, sizeof(int));
			break;
		case WeldResults::PARALIST::PRE_HEIGHT:
			memcpy(para, &this->PreHeight, sizeof(int));
			break;
		case WeldResults::PARALIST::POST_HEIGHT:
			memcpy(para, &this->PostHeight, sizeof(int));
			break;
		case WeldResults::PARALIST::ALARM_ID:
			memcpy(para, &this->AlarmID, sizeof(int));
			break;
		case ENERGY_SETTING:
			memcpy(para, &this->EnergySetting, sizeof(int));
			break;
		case AMPLITUDE_SETTING:
			memcpy(para, &this->AmplitudeSetting, sizeof(int));
			break;
		case T_PRESSURE_SETTING:
			memcpy(para, &this->TPressureSetting, sizeof(int));
			break;
		case W_PRESSURE_SETTING:
			memcpy(para, &this->WPressureSetting, sizeof(int));
			break;
		case MAX_WELD_TIME:
			memcpy(para, &this->MaxWeldTime, sizeof(int));
			break;
		case MIN_WELD_TIME:
			memcpy(para, &this->MinWeldTime, sizeof(int));
			break;
		case MAX_POWER:
			memcpy(para, &this->MaxPower, sizeof(int));
			break;
		case MIN_POWER:
			memcpy(para, &this->MinPower, sizeof(int));
			break;
		case MAX_PRE_HEIGHT:
			memcpy(para, &this->MaxPreHeight, sizeof(int));
			break;
		case MIN_PRE_HEIGHT:
			memcpy(para, &this->MinPreHeight, sizeof(int));
			break;
		case MAX_POST_HEIGHT:
			memcpy(para, &this->MaxPostHeight, sizeof(int));
			break;
		case MIN_POST_HEIGHT:
			memcpy(para, &this->MinPostHeight, sizeof(int));
			break;
		default:
			iResult = ERROR;
			break;
		}
	} else {
		iResult = ERROR;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 Weld Result.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_WeldResults::Init() {
}

/**************************************************************************//**
* 
* \brief   - Set data to P1 Weld Results.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_WeldResults::Set(int, void*) {
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get data to P1 Weld Results.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_WeldResults::Get(int, void*) {
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 Weld Result.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_WeldResults::Init() {
}
