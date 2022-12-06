/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <iostream>
#include "WeldResultSignatureDefine.h"
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
shared_ptr<WeldResultSignatureDefine> WeldResultSignatureDefine::GetWeldSignature(){
	shared_ptr<WeldResultSignatureDefine> ptrWeldSignature = nullptr;
	switch (GetSystemType()) {
	case GSXP1: {
		ptrWeldSignature = make_shared<P1_WeldResultSignatureDefine>();
	}
		break;

	case L20: {
		ptrWeldSignature = make_shared<L20_WeldResultSignatureDefine>();
	}
		break;

	default: {
		cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
		break;
	}

	return ptrWeldSignature;
}

/**************************************************************************//**
* 
* \brief   - Set data to L20 Weld Signature.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_WeldResultSignatureDefine::Set(const int index, const unsigned int para)
{
	int iResult = OK;
	switch (index) 
	{
	case WeldResultSignatureDefine::HEIGHT:
		Height = para;
		break;
	case WeldResultSignatureDefine::AMPLITUDE:
		Amplitude = para;
		break;
	case WeldResultSignatureDefine::FRQUENCY:
		Frquency = para;
		break;
	case WeldResultSignatureDefine::POWER:
		Power = para;
		break;
	default:
		iResult = ERROR;
		break;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Get data to L20 Weld Signature.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
unsigned int L20_WeldResultSignatureDefine::Get(const int index, int* ok)
{
	int iOK = OK;
	int iResult = 0;
	switch (index) 
	{
	case WeldResultSignatureDefine::HEIGHT:
		iResult = Height;
		break;
	case WeldResultSignatureDefine::AMPLITUDE:
		iResult = Amplitude;
		break;
	case WeldResultSignatureDefine::FRQUENCY:
		iResult = Frquency;
		break;
	case WeldResultSignatureDefine::POWER:
		iResult = Power;
		break;
	default:
		iResult = ERROR;
		iOK = ERROR;
		break;
	}
	
	if(ok != nullptr)
		*ok = iOK;
	
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 Weld Signature.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_WeldResultSignatureDefine::Init(){
	
}

/**************************************************************************//**
* 
* \brief   - Set data to P1 Weld Signature.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_WeldResultSignatureDefine::Set(const int index, const unsigned int para)
{
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get data to P1 Weld Signature.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
unsigned int P1_WeldResultSignatureDefine::Get(const int, int* ok)
{
	return 0;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 Weld Signature.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_WeldResultSignatureDefine::Init(){
	
}
