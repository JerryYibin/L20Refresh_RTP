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
#include "SystemType.h"
#include "commons.h"

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
		case WeldResults::PARALIST::ACTUAL_WIDTH:
			memcpy(&this->ActualWidth, para, sizeof(int));
			break;
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
		case WeldResults::PARALIST::ALARM_FLAGS:
			memcpy(&this->AlarmFlags, para, sizeof(int));
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
		case WeldResults::PARALIST::ACTUAL_WIDTH:
			memcpy(para, &this->ActualWidth, sizeof(int));
			break;
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
		case WeldResults::PARALIST::ALARM_FLAGS:
			memcpy(para, &this->AlarmFlags, sizeof(int));
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
