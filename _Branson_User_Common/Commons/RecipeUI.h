/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file RecipeUI.h is to define the enum and struct related to the recipe.

 **********************************************************************************************************/
#ifndef RECIPEUI_H
#define RECIPEUI_H

#include <vector>

#define MAX_NO_OF_FORCE_STEP 10
#define MAX_NO_OF_AMPLITUDE_STEP 10
#define RECIPE_NAME_LENGTH   20

#define STEP_MAX 		5
#define PIC_PATH_LEN 	60
#define RECIPE_LEN		20

enum WELDMODE
{
    ENERGY_MODE = 0,
    TIME_MODE,
    HEIGHT_MODE,
    ENERGY_HEIGHT_MODE
};

enum WELDAMPLITUDESTEPMMODE
{
    STEP_NO_MODE = -1,
    STEP_ENERGY_MODE = 0,
    STEP_TIME_MODE,
    STEP_POWER_MODE,

};

enum TRIGGERMODE
{
    TRIGGER_DISTANCE_MODE,
    TRIGGER_TIME_MODE,
    TRIGGER_FORCE_MODE
};

struct Trigger
{
	TRIGGERMODE       m_TriggerType;
    unsigned int      m_PreTriggerDistance;
    unsigned int      m_PreTriggerForce;
    unsigned int      m_PreTriggerTime;
};

struct AdvancedWeldSetting
{
    int m_WeldMode;
    int m_WeldStepMode;
    Trigger m_Trigger;
    int m_PreBurst;
    int m_HoldTime;
    int m_SqueezeTime;
    int m_AfterBurstDelay;
    int m_AfterBurstTime;
    int m_AfterBurstAmplitude; //Reserved
    int m_DisplayedHeightOffset;
    int m_MeasuredHeightOffset;
};

struct WeldStepValueSetting
{
	int m_Order;		  //It is the index of step, if the index is -1 the current step is not available. 
	int m_StepValue;      //step_energy_value/step_time_value/step_power_value
	int m_AmplitudeValue;
};

struct WeldParameterSetting
{
    unsigned int m_EnergySetting;
    int          m_TPressure;
    int          m_WPressure;
    unsigned int m_Amplitude;
    WeldStepValueSetting m_EnergyStep[STEP_MAX];
    WeldStepValueSetting m_TimeStep[STEP_MAX];
    WeldStepValueSetting m_PowerStep[STEP_MAX];
    int          m_WidthSetting;
};

struct QualityWindowSetting
{
    int m_TimeMax;
    int m_TimeMin;
    unsigned int m_PeakPowerMax;
    unsigned int m_PeakPowerMin;
    int m_PreHeightMax;
    int m_PreHeightMin;
    int m_HeightMax;
    int m_HeightMin;
};

struct WeldRecipeSetting
{
    WeldParameterSetting m_WeldParameter;
    QualityWindowSetting m_QualityWindowSetting;
    AdvancedWeldSetting  m_AdvancedSetting;
};

struct  WeldRecipeSCForUI
{
    int                   m_RecipeID;
    int                   m_RecipeNumber;
    char                  m_RecipePicPath[PIC_PATH_LEN];
    int                   m_IsTeachMode;
    char                  m_RecipeName[RECIPE_NAME_LENGTH];
    unsigned int		  m_BatchSize; // For active recipe only, don't allow to save it with the recipe in the database. It will be saved into the Last Weld
    WeldParameterSetting  m_WeldParameter;
    QualityWindowSetting  m_QualityWindowSetting;
    AdvancedWeldSetting   m_AdvancedSetting;
};

struct WeldRecipePCForUI
{
    int     m_PreWeldSeek;
    int  	m_FrequencyOffset;
    int     m_DigitalTune;
};

struct WeldRecipeACForUI
{
    int    m_HeightEncoder;
    int    m_HomePosition;
};

struct WeldRecipeForUI
{
	WeldRecipeSCForUI  m_WeldRecipe_SC;
    WeldRecipePCForUI  m_WeldRecipe_PC;
    WeldRecipeACForUI  m_WeldRecipe_AC;
};


#endif // WELDRECIPEDEF_H
