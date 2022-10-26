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
#include <memory>
#include "RecipeUI.h"

struct WeldRecipeSC
{
	WeldRecipeSC();
	virtual ~WeldRecipeSC();
	enum PARALIST {
		RECIPE_PIC_PATH = 0,
		IS_TEACH_MODE,
		BATCH_SIZE,

		WELD_PARAMETER,
		ENERGY_SETTING,
		TP_PRESSURE,
		WP_PRESSURE,
		AMPLITUDE,
		ENERGY_STEP,
		
		TIME_STEP,

		POWER_STEP,

		WIDTH_SETTING,
		QUALITY_WINDOW_SETTING,
		TIME_MAX,
		TIME_MIN,
		PEAK_POWER_MAX,
		PEAK_POWER_MIN,
		PRE_HEIGHT_MAX,
		PRE_HEIGHT_MIN,
		HEIGHT_MAX,
		HEIGHT_MIN,

		ADVANCED_SETTING,
		WELD_MODE,
		WELD_STEP_MODE,
		TRIGGER,
		PRE_BURST,
		HOLD_TIME,
		SQUEEZE_TIME,
		AFTER_BURST_DELAY,
		AFTER_BURST_TIME,
		AFTER_BURST_AMPLITUDE,
		DISPLAYED_HEIGHT_OFFSET,
		MEASURED_HEIGHT_OFFSET,
	};
	
	int					  m_RecipeID;
    int                   m_RecipeNumber;
    char		          m_RecipeName[RECIPE_LEN];
    virtual int	Size() = 0;
    virtual int Set(void*, int) = 0;
    virtual int Get(int, void*) = 0;
    virtual void InitialWeldRecipe() = 0;
    static std::shared_ptr<WeldRecipeSC> GetWeldRecipeSC();
};

struct WeldRecipePC
{
    int  	m_FrequencyOffset;
    int     m_PreWeldSeek;
    int     m_DigitalTune;
    virtual int	Size() = 0;
    static std::shared_ptr<WeldRecipePC> GetWeldRecipePC();
};

struct WeldRecipeAC
{
    virtual int	Size() = 0;
    static std::shared_ptr<WeldRecipeAC> GetWeldRecipeAC();
};

struct WeldRecipe
{
	WeldRecipeSC*  m_WeldRecipe_SC;
    WeldRecipePC*  m_WeldRecipe_PC;
    WeldRecipeAC*  m_WeldRecipe_AC;
};

struct L20_WeldRecipeSC : public WeldRecipeSC
{
	L20_WeldRecipeSC();
	~L20_WeldRecipeSC();
    char                  m_RecipePicPath[PIC_PATH_LEN];
    int                   m_IsTeachMode;
    unsigned int		  m_BatchSize; //TODO For active recipe only, don't allow to save it with the recipe in the database. It will be saved into the Last Weld
    WeldParameterSetting  m_WeldParameter;
    QualityWindowSetting  m_QualityWindowSetting;
    AdvancedWeldSetting   m_AdvancedSetting;
    
    int Size() 						override;
    int Set(void*, int) 			override;
    int Get(int, void*) 			override;
    void InitialWeldRecipe()		override;
};

struct P1_WeldRecipeSC : public WeldRecipeSC
{
	P1_WeldRecipeSC();
	~P1_WeldRecipeSC();
    int Size() 						override;
    int Set(void*, int) 			override;
    int Get(int, void*) 			override;
    void InitialWeldRecipe()		override;
};

struct L20_WeldRecipeAC : public WeldRecipeAC
{
    int     m_HeightEncoder;
    int     m_HomePosition;
    int Size()						override;
};

struct P1_WeldRecipeAC : public WeldRecipeAC
{
    int Size()						override;
};

struct L20_WeldRecipePC : public WeldRecipePC
{
    int Size()						override;
};

struct P1_WeldRecipePC : public WeldRecipePC
{
    int Size()						override;
};

#endif // WELDRECIPEDEF_H
