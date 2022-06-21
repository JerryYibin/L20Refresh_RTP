/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file weldrecipedef.h is to define the enum and struct related to the recipe.

 **********************************************************************************************************/
#ifndef WELDRECIPEDEF_H
#define WELDRECIPEDEF_H

#define STEP_MAX 		5
#define PIC_PATH_LEN 	60
enum WELDMODE
{
    ENERGY_MODE = 0,
    TIME_MODE,
    HEIGHT_MODE,
    ENERGY_HEIGHT_MODE
};

enum WELDAMPLITUDESTEPMMODE
{
    STEP_NO_MODE    = -1,
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
    int               m_TriggerType;
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
    int m_AfterBurstAmplitude;
    int m_DisplayedHeightOffSet;
    int m_MeasuredHeightOffSet;
};

struct WeldStepValueSetting
{
	int m_Order;
	int m_StepValue;      //step_energy_value/step_time_value/step_power_value
	int m_AmplitudeValue;
};

struct WeldParameterSetting
{
    unsigned int m_EnergySetting;
    int          m_TPpressure;
    int          m_WPpressure;
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


struct  WeldRecipeSC
{
    int                   m_RecipeNumber;
    char                  m_RecipePicPath[PIC_PATH_LEN];
    int                   m_IsTeachMode;
    unsigned int          m_UserID;
    WeldParameterSetting  m_WeldParameter;
    QualityWindowSetting  m_QualityWindowSetting;
    AdvancedWeldSetting   m_AdvancedSetting;
};


struct WeldRecipePC
{
    int     m_PreWeldSeek;
    int  	m_FrequencyOffset;
    int     m_DigitalTune;
};


struct WeldRecipeAC
{
    int     m_HeightEncoder;
    int     m_HomePosition;
};


struct WeldRecipe
{
	WeldRecipeSC  m_WeldRecipe_SC;
    WeldRecipePC  m_WeldRecipe_PC;
    WeldRecipeAC  m_WeldRecipe_AC;
};


#endif // WELDRECIPEDEF_H
