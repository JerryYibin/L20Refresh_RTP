/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file weldrecipedef.h is to define the enum and struct related to the recipe.

 **********************************************************************************************************/
#include <iostream>
#include "RecipeDefine.h"
#include "SystemType.h"
#include "commons.h"
#include "Utility.h"

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldRecipeSC::WeldRecipeSC()
{
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
WeldRecipeSC::~WeldRecipeSC()
{
}

/**************************************************************************//**
* 
* \brief   - Select different SC structures for different devices.
*
* \param   - None.
*
* \return  - Structure pointer.
*
******************************************************************************/
std::shared_ptr<WeldRecipeSC> WeldRecipeSC::GetWeldRecipeSC()
{
	std::shared_ptr<WeldRecipeSC> ptrWeldRecipeSC = nullptr;
	switch (GetSystemType())
	{
	case GSXP1:
	{
		ptrWeldRecipeSC = std::make_shared<P1_WeldRecipeSC>();
	}
	break;

	case L20:
	{
		ptrWeldRecipeSC = std::make_shared<L20_WeldRecipeSC>();
	}
	break;

	default:
	{
		std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
	break;
	}
	
	return ptrWeldRecipeSC;
}

/**************************************************************************//**
* 
* \brief   - Select different PC structures for different devices.
*
* \param   - None.
*
* \return  - Structure pointer.
*
******************************************************************************/
std::shared_ptr<WeldRecipePC> WeldRecipePC::GetWeldRecipePC()
{
	std::shared_ptr<WeldRecipePC> ptrWeldRecipePC = nullptr;
	switch (GetSystemType())
	{
	case GSXP1:
	{
		ptrWeldRecipePC = std::make_shared<P1_WeldRecipePC>();
	}
	break;

	case L20:
	{
		ptrWeldRecipePC = std::make_shared<L20_WeldRecipePC>();
	}
	break;

	default:
	{
		std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
	break;
	}
	
	return ptrWeldRecipePC;
}

/**************************************************************************//**
* 
* \brief   - Select different AC structures for different devices.
*
* \param   - None.
*
* \return  - Structure pointer.
*
******************************************************************************/
std::shared_ptr<WeldRecipeAC> WeldRecipeAC::GetWeldRecipeAC()
{
	std::shared_ptr<WeldRecipeAC> ptrWeldRecipeAC = nullptr;
	switch (GetSystemType())
	{
	case GSXP1:
	{
		ptrWeldRecipeAC = std::make_shared<P1_WeldRecipeAC>();
	}
	break;

	case L20:
	{
		ptrWeldRecipeAC = std::make_shared<L20_WeldRecipeAC>();
	}
	break;

	default:
	{
		std::cout << "MODEL_T : --------NO System Detected-----------" << std::endl;
	}
	break;
	}
	
	return ptrWeldRecipeAC;
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
L20_WeldRecipeSC::L20_WeldRecipeSC()
{
	Init();
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
L20_WeldRecipeSC::~L20_WeldRecipeSC()
{}

/**************************************************************************//**
* 
* \brief   - Size of L20_WeldRecipeSC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int L20_WeldRecipeSC::Size()
{
	return sizeof(L20_WeldRecipeSC);
}

/**************************************************************************//**
* 
* \brief   - Set data to L20 Weld RecipeSC.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_WeldRecipeSC::Set(void* para, int indlx)
{
	int iResult = TRUE;
	if (nullptr != para) {
		switch (indlx) {
		case WeldRecipeSC::PARALIST::RECIPE_PIC_PATH:
			memcpy(this->m_RecipePicPath, para, sizeof(para) + 1);
			break;
		case WeldRecipeSC::PARALIST::IS_TEACH_MODE:
			memcpy(&this->m_IsTeachMode, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::BATCH_SIZE:
			memcpy(&this->m_BatchSize, para, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::WELD_PARAMETER:
			memcpy(&this->m_WeldParameter, para, sizeof(WeldParameterSetting));
			break;
		case WeldRecipeSC::PARALIST::ENERGY_SETTING:
			memcpy(&this->m_WeldParameter.m_EnergySetting, para, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::TP_PRESSURE:
			memcpy(&this->m_WeldParameter.m_TPressure, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::WP_PRESSURE:
			memcpy(&this->m_WeldParameter.m_WPressure, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AMPLITUDE:
			memcpy(&this->m_WeldParameter.m_Amplitude, para, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::ENERGY_STEP:
			memcpy(this->m_WeldParameter.m_EnergyStep, para, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::TIME_STEP:
			memcpy(this->m_WeldParameter.m_TimeStep, para, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::POWER_STEP:
			memcpy(this->m_WeldParameter.m_PowerStep, para, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::WIDTH_SETTING:
			memcpy(&this->m_WeldParameter.m_WidthSetting, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::QUALITY_WINDOW_SETTING:
			memcpy(&this->m_QualityWindowSetting, para, sizeof(QualityWindowSetting));
			break;
		case WeldRecipeSC::PARALIST::TIME_MAX:
			memcpy(&this->m_QualityWindowSetting.m_TimeMax, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::TIME_MIN:
			memcpy(&this->m_QualityWindowSetting.m_TimeMin, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::PEAK_POWER_MAX:
			memcpy(&this->m_QualityWindowSetting.m_PeakPowerMax, para, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::PEAK_POWER_MIN:
			memcpy(&this->m_QualityWindowSetting.m_PeakPowerMin, para, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX:
			memcpy(&this->m_QualityWindowSetting.m_PreHeightMax, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN:
			memcpy(&this->m_QualityWindowSetting.m_PreHeightMin, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HEIGHT_MAX:
			memcpy(&this->m_QualityWindowSetting.m_HeightMax, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HEIGHT_MIN:
			memcpy(&this->m_QualityWindowSetting.m_HeightMin, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::ADVANCED_SETTING:
			memcpy(&this->m_AdvancedSetting, para, sizeof(AdvancedWeldSetting));
			break;
		case WeldRecipeSC::PARALIST::WELD_MODE:
			memcpy(&this->m_AdvancedSetting.m_WeldMode, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::WELD_STEP_MODE:
			memcpy(&this->m_AdvancedSetting.m_WeldStepMode, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::TRIGGER:
			memcpy(&this->m_AdvancedSetting.m_Trigger, para, sizeof(Trigger));
			break;
		case WeldRecipeSC::PARALIST::PRE_BURST:
			memcpy(&this->m_AdvancedSetting.m_PreBurst, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HOLD_TIME:
			memcpy(&this->m_AdvancedSetting.m_HoldTime, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::SQUEEZE_TIME:
			memcpy(&this->m_AdvancedSetting.m_SqueezeTime, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_DELAY:
			memcpy(&this->m_AdvancedSetting.m_AfterBurstDelay, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_TIME:
			memcpy(&this->m_AdvancedSetting.m_AfterBurstTime, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_AMPLITUDE:
			memcpy(&this->m_AdvancedSetting.m_AfterBurstAmplitude, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::DISPLAYED_HEIGHT_OFFSET:
			memcpy(&this->m_AdvancedSetting.m_DisplayedHeightOffset, para, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::MEASURED_HEIGHT_OFFSET:
			memcpy(&this->m_AdvancedSetting.m_MeasuredHeightOffset, para, sizeof(int));
			break;
		default:
			iResult = FALSE;
			break;
		}
	} else {
		iResult = FALSE;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Get data to L20 Weld RecipeSC.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int L20_WeldRecipeSC::Get(int indlx, void* para)
{
	int iResult = TRUE;
	if (nullptr != para) {
		switch (indlx) {
		case WeldRecipeSC::PARALIST::RECIPE_PIC_PATH:
			memcpy(para, this->m_RecipePicPath, sizeof(para) + 1);
			break;
		case WeldRecipeSC::PARALIST::IS_TEACH_MODE:
			memcpy(para, &this->m_IsTeachMode, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::BATCH_SIZE:
			memcpy(para, &this->m_BatchSize, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::WELD_PARAMETER:
			memcpy(para, &this->m_WeldParameter, sizeof(WeldParameterSetting));
			break;
		case WeldRecipeSC::PARALIST::ENERGY_SETTING:
			memcpy(para, &this->m_WeldParameter.m_EnergySetting, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::TP_PRESSURE:
			memcpy(para, &this->m_WeldParameter.m_TPressure, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::WP_PRESSURE:
			memcpy(para, &this->m_WeldParameter.m_WPressure, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AMPLITUDE:
			memcpy(para, &this->m_WeldParameter.m_Amplitude, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::ENERGY_STEP:
			memcpy(para, this->m_WeldParameter.m_EnergyStep, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::TIME_STEP:
			memcpy(para, this->m_WeldParameter.m_TimeStep, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::POWER_STEP:
			memcpy(para, this->m_WeldParameter.m_PowerStep, sizeof(WeldStepValueSetting)*STEP_MAX);
			break;
		case WeldRecipeSC::PARALIST::WIDTH_SETTING:
			memcpy(para, &this->m_WeldParameter.m_WidthSetting, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::QUALITY_WINDOW_SETTING:
			memcpy(para, &this->m_QualityWindowSetting, sizeof(QualityWindowSetting));
			break;
		case WeldRecipeSC::PARALIST::TIME_MAX:
			memcpy(para, &this->m_QualityWindowSetting.m_TimeMax, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::TIME_MIN:
			memcpy(para, &this->m_QualityWindowSetting.m_TimeMin, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::PEAK_POWER_MAX:
			memcpy(para, &this->m_QualityWindowSetting.m_PeakPowerMax, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::PEAK_POWER_MIN:
			memcpy(para, &this->m_QualityWindowSetting.m_PeakPowerMin, sizeof(unsigned int));
			break;
		case WeldRecipeSC::PARALIST::PRE_HEIGHT_MAX:
			memcpy(para, &this->m_QualityWindowSetting.m_PreHeightMax, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::PRE_HEIGHT_MIN:
			memcpy(para, &this->m_QualityWindowSetting.m_PreHeightMin, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HEIGHT_MAX:
			memcpy(para, &this->m_QualityWindowSetting.m_HeightMax, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HEIGHT_MIN:
			memcpy(para, &this->m_QualityWindowSetting.m_HeightMin, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::ADVANCED_SETTING:
			memcpy(para, &this->m_AdvancedSetting, sizeof(AdvancedWeldSetting));
			break;
		case WeldRecipeSC::PARALIST::WELD_MODE:
			memcpy(para, &this->m_AdvancedSetting.m_WeldMode, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::WELD_STEP_MODE:
			memcpy(para, &this->m_AdvancedSetting.m_WeldStepMode, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::TRIGGER:
			memcpy(para, &this->m_AdvancedSetting.m_Trigger, sizeof(Trigger));
			break;
		case WeldRecipeSC::PARALIST::PRE_BURST:
			memcpy(para, &this->m_AdvancedSetting.m_PreBurst, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::HOLD_TIME:
			memcpy(para, &this->m_AdvancedSetting.m_HoldTime, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::SQUEEZE_TIME:
			memcpy(para, &this->m_AdvancedSetting.m_SqueezeTime, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_DELAY:
			memcpy(para, &this->m_AdvancedSetting.m_AfterBurstDelay, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_TIME:
			memcpy(para, &this->m_AdvancedSetting.m_AfterBurstTime, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::AFTER_BURST_AMPLITUDE:
			memcpy(para, &this->m_AdvancedSetting.m_AfterBurstAmplitude, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::DISPLAYED_HEIGHT_OFFSET:
			memcpy(para, &this->m_AdvancedSetting.m_DisplayedHeightOffset, sizeof(int));
			break;
		case WeldRecipeSC::PARALIST::MEASURED_HEIGHT_OFFSET:
			memcpy(para, &this->m_AdvancedSetting.m_MeasuredHeightOffset, sizeof(int));
			break;
		default:
			iResult = FALSE;
			break;
		}
	} else {
		iResult = FALSE;
	}
	return iResult;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of L20 Weld Recipe.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void L20_WeldRecipeSC::Init()
{
	m_IsTeachMode							= 0;
	m_BatchSize								= 0;
	m_WeldParameter.m_EnergySetting 		= ENERGY_SETTING_NUM;
	m_WeldParameter.m_Amplitude 			= AMPLITUDE_NUM;
	m_WeldParameter.m_TPressure 			= TPRESSURE_NUM;
	m_WeldParameter.m_WPressure 			= WPRESSURE_NUM;
	m_AdvancedSetting.m_WeldStepMode 		= WELD_STEP_MODE_NUM;
	m_QualityWindowSetting.m_PreHeightMin 	= PRE_HEIGHT_MIN_NUM;
	m_QualityWindowSetting.m_PreHeightMax 	= PRE_HEIGHT_MAX_NUM;
	m_QualityWindowSetting.m_HeightMin 		= HEIGHT_MIN_NUM;
	m_QualityWindowSetting.m_HeightMax 		= HEIGHT_MAX_NUM;
	m_QualityWindowSetting.m_PeakPowerMin 	= PEAK_POWER_MIN_NUM;
	m_QualityWindowSetting.m_PeakPowerMax 	= Utility::GetSystemPower() * 1.2;
	m_QualityWindowSetting.m_TimeMin 		= TIME_MIN_NUM;
	m_QualityWindowSetting.m_TimeMax 		= TIME_MAX_NUM;
	m_AdvancedSetting.m_WeldMode 			= WELD_MODE_NUM;
	m_AdvancedSetting.m_AfterBurstAmplitude	= 0;
	m_AdvancedSetting.m_AfterBurstDelay		= 0;
	m_AdvancedSetting.m_AfterBurstTime		= 0;
	m_AdvancedSetting.m_DisplayedHeightOffset = 0;
	m_AdvancedSetting.m_HoldTime			= 0;
	m_AdvancedSetting.m_MeasuredHeightOffset = 0;
	m_AdvancedSetting.m_PreBurst			= 0;
	m_AdvancedSetting.m_SqueezeTime			= 0;
	m_AdvancedSetting.m_Trigger.m_TriggerType			= TRIGGER_DISTANCE_MODE;
	m_AdvancedSetting.m_Trigger.m_PreTriggerDistance	= 0;
	m_AdvancedSetting.m_Trigger.m_PreTriggerForce		= 0;
	m_AdvancedSetting.m_Trigger.m_PreTriggerTime		= 0;
	m_AdvancedSetting.m_WeldMode			= 0;
	m_AdvancedSetting.m_WeldStepMode		= 0;
	m_BatchSize 							= BATCH_SIZE_NUM;
	char tmpName[] 							= DEFINE_NAME;
	memcpy(m_RecipeName, tmpName, sizeof(tmpName));
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
P1_WeldRecipeSC::P1_WeldRecipeSC()
{}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
P1_WeldRecipeSC::~P1_WeldRecipeSC()
{}

/**************************************************************************//**
* 
* \brief   - Size of P1_WeldRecipeSC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int P1_WeldRecipeSC::Size()
{
	return sizeof(P1_WeldRecipeSC);
}

/**************************************************************************//**
* 
* \brief   - Set data to P1 Weld RecipeSC.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_WeldRecipeSC::Set(void* para, int indlx)
{
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Get data to P1 Weld RecipeSC.
*
* \param   - Index: data index; _data: Input data pointer.
*
* \return  - If there is any error happened, it will return ERROR; else it will return OK.
*
******************************************************************************/
int P1_WeldRecipeSC::Get(int indlx, void* para)
{
	return OK;
}

/**************************************************************************//**
* 
* \brief   - Initialize the data members of P1 Weld Recipe.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void P1_WeldRecipeSC::Init()
{
	
}

/**************************************************************************//**
* 
* \brief   - Size of L20_WeldRecipeAC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int L20_WeldRecipeAC::Size(){
	return sizeof(L20_WeldRecipeAC);
}

/**************************************************************************//**
* 
* \brief   - Size of P1_WeldRecipeAC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int P1_WeldRecipeAC::Size(){
	return sizeof(P1_WeldRecipeAC);
}

/**************************************************************************//**
* 
* \brief   - Size of L20_WeldRecipePC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int L20_WeldRecipePC::Size(){
	return sizeof(L20_WeldRecipePC);
}

/**************************************************************************//**
* 
* \brief   - Size of P1_WeldRecipePC structure.
*
* \param   - None.
*
* \return  - Size of structure.
******************************************************************************/
int P1_WeldRecipePC::Size(){
	return sizeof(P1_WeldRecipePC);
}
