/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The class is the encapsulation for UIController only. 
 The Set & Get can be overload following the different system type.  
 
***************************************************************************/
#include "RecipeWrapper.h"
#include "vxWorks.h"
//#include "Utility.h"
//#include "Recipe.h"

int RecipeWrapper::Set(const char* messagebuf, WeldRecipeSC* _RecipeObj)
{
	if (messagebuf == nullptr)
		return ERROR;
	if (_RecipeObj == nullptr)
		return ERROR;

	WeldRecipeSCForUI pRecipeForUI;
    memcpy(&pRecipeForUI, messagebuf, sizeof(WeldRecipeSCForUI));

    _RecipeObj->m_RecipeID = pRecipeForUI.m_RecipeID;
    _RecipeObj->m_RecipeNumber = pRecipeForUI.m_RecipeNumber;
    
	memcpy(_RecipeObj->m_RecipeName, pRecipeForUI.m_RecipeName, RECIPE_NAME_LENGTH);

	_RecipeObj->Set(pRecipeForUI.m_RecipePicPath, 			WeldRecipeSC::PARALIST::RECIPE_PIC_PATH);
	_RecipeObj->Set(&pRecipeForUI.m_IsTeachMode, 			WeldRecipeSC::PARALIST::IS_TEACH_MODE);
	_RecipeObj->Set(&pRecipeForUI.m_BatchSize, 				WeldRecipeSC::PARALIST::BATCH_SIZE);
	_RecipeObj->Set(&pRecipeForUI.m_WeldParameter, 			WeldRecipeSC::PARALIST::WELD_PARAMETER);
	_RecipeObj->Set(&pRecipeForUI.m_QualityWindowSetting, 	WeldRecipeSC::PARALIST::QUALITY_WINDOW_SETTING);
	_RecipeObj->Set(&pRecipeForUI.m_AdvancedSetting, 		WeldRecipeSC::PARALIST::ADVANCED_SETTING);
	
	return OK;
}

int RecipeWrapper::Get(WeldRecipeSC* _RecipeObj, char* messagebuf)
{
	if (messagebuf == nullptr)
		return ERROR;
	if (_RecipeObj == nullptr)
		return ERROR;

	WeldRecipeSCForUI pRecipeForUI;
	pRecipeForUI.m_RecipeID = _RecipeObj->m_RecipeID;
	pRecipeForUI.m_RecipeNumber = _RecipeObj->m_RecipeNumber;
	
	memcpy(pRecipeForUI.m_RecipeName, _RecipeObj->m_RecipeName, RECIPE_NAME_LENGTH);
	
	_RecipeObj->Get(WeldRecipeSC::PARALIST::RECIPE_PIC_PATH, 		pRecipeForUI.m_RecipePicPath);
	_RecipeObj->Get(WeldRecipeSC::PARALIST::IS_TEACH_MODE, 			&pRecipeForUI.m_IsTeachMode);
	_RecipeObj->Get(WeldRecipeSC::PARALIST::BATCH_SIZE, 			&pRecipeForUI.m_BatchSize);
	_RecipeObj->Get(WeldRecipeSC::PARALIST::WELD_PARAMETER, 		&pRecipeForUI.m_WeldParameter);
	_RecipeObj->Get(WeldRecipeSC::PARALIST::QUALITY_WINDOW_SETTING, &pRecipeForUI.m_QualityWindowSetting);
	_RecipeObj->Get(WeldRecipeSC::PARALIST::ADVANCED_SETTING, 		&pRecipeForUI.m_AdvancedSetting);
	
	memcpy(messagebuf, &pRecipeForUI, sizeof(WeldRecipeSCForUI));
	return OK;
}
