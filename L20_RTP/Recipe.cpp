/*
 * ActiveRecipe.cpp
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */
#include "Recipe.h"
#include "CommonProperty.h"
Recipe::Recipe() {
	// TODO Auto-generated constructor stub

}

Recipe::~Recipe() {
	// TODO Auto-generated destructor stub
}

void Recipe::SetActiveRecipeGeneralParam(WeldParameterSetting* generalParam)
{
	memcpy(&CommonProperty::ActiveRecipeSC.m_WeldParameter, generalParam, sizeof(WeldParameterSetting));
}

void Recipe::SetActiveRecipeQualityParam(QualityWindowSetting* qualityParam)
{
	memcpy(&CommonProperty::ActiveRecipeSC.m_QualityWindowSetting, qualityParam, sizeof(QualityWindowSetting));
}

void Recipe::SetActiveRecipeAdvancedParam(AdvancedWeldSetting* advancedParam)
{
	memcpy(&CommonProperty::ActiveRecipeSC.m_AdvancedSetting, advancedParam, sizeof(AdvancedWeldSetting));
}
