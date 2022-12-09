/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <iostream>
#include "AlarmDefine.h"
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
std::shared_ptr<AlarmLogDefine> AlarmLogDefine::GetAlarmLog(){
	std::shared_ptr<AlarmLogDefine> ptrAlarmLogDefine = nullptr;
	switch (GetSystemType()) {
	case GSXP1: {
		ptrAlarmLogDefine = std::make_shared<P1_AlarmLogDefine>();
	}
		break;

	case L20: {
		ptrAlarmLogDefine = std::make_shared<L20_AlarmLogDefine>();
	}
		break;

	default: {
		std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
		break;
	}

	return ptrAlarmLogDefine;
}

/**************************************************************************//**
* 
* \brief   - Set data to L20 Alarm Log.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_AlarmLogDefine::Set(int indlx, void* para){
	int iResult = ERROR;
	if (nullptr != para) {
		switch (indlx) {
		case AlarmLogDefine::PARALIST::WELD_COUNT:
			iResult = OK;
			memcpy(&this->WeldCount, para, sizeof(unsigned int));
			break;
		case AlarmLogDefine::PARALIST::RECIPE_NAME:
			iResult = OK;
			memcpy(this->RecipeName, para, RECIPE_NAME_LENGTH);
			break;
		case AlarmLogDefine::PARALIST::DATE_TIME:
			iResult = OK;
			memcpy(this->DateTime, para, DATA_TIME_LENGTH);
			break;
		case ALARM_TYPE:
			iResult = OK;
			memcpy(&this->AlarmType, para, sizeof(unsigned int));
			break;
		default:
			break;
		}
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Get data to L20 Alarm Log.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_AlarmLogDefine::Get(int indlx, void* para){
	int iResult = ERROR;
	if (nullptr != para) {
		switch (indlx) {
		case AlarmLogDefine::PARALIST::WELD_COUNT:
			iResult = OK;
			memcpy(para, &this->WeldCount, sizeof(unsigned int));
			break;
		case AlarmLogDefine::PARALIST::RECIPE_NAME:
			iResult = OK;
			memcpy(para, this->RecipeName, RECIPE_NAME_LENGTH);
			break;
		case AlarmLogDefine::PARALIST::DATE_TIME:
			iResult = OK;
			memcpy(para, this->DateTime, DATA_TIME_LENGTH);
			break;
		case ALARM_TYPE:
			iResult = OK;
			memcpy(para, &this->AlarmType, sizeof(unsigned int));
			break;
		default:
			break;
		}
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 Alarm Log.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_AlarmLogDefine::Init(){
	
}

/**************************************************************************//**
* 
* \brief   - Set data to P1 Alarm Log.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_AlarmLogDefine::Set(int, void*){
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get data to P1 Alarm Log.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_AlarmLogDefine::Get(int, void*){
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 Alarm Log.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_AlarmLogDefine::Init(){
	
}
