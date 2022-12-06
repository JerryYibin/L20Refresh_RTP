/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file Recipe.h is a public interface that provides external.

 **********************************************************************************************************/
#include "Recipe.h"
#include "CommonProperty.h"
shared_ptr<WeldRecipeSC> Recipe::ActiveRecipeSC = WeldRecipeSC::GetWeldRecipeSC();
shared_ptr<WeldRecipeSC> Recipe::RecipeSC = nullptr;
vector<WeldRecipeLibForUI>				Recipe::WeldRecipeLibraryForUI;
Recipe::Recipe() {
		ActiveRecipeSC->Init();
}

Recipe::~Recipe() {
	// TODO Auto-generated destructor stub
}

void Recipe::SetActiveRecipeGeneralParam(WeldParameterSetting* generalParam)
{
	Recipe::ActiveRecipeSC->Set(generalParam, WeldRecipeSC::PARALIST::WELD_PARAMETER);
}

void Recipe::SetActiveRecipeQualityParam(QualityWindowSetting* qualityParam)
{
	Recipe::ActiveRecipeSC->Set(qualityParam, WeldRecipeSC::PARALIST::QUALITY_WINDOW_SETTING);
}

void Recipe::SetActiveRecipeAdvancedParam(AdvancedWeldSetting* advancedParam)
{
	Recipe::ActiveRecipeSC->Set(advancedParam, WeldRecipeSC::PARALIST::ADVANCED_SETTING);
}
