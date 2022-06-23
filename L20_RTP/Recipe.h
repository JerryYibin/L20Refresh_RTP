/*
 * Recipe.h
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#ifndef RECIPE_H_
#define RECIPE_H_
#include "recipedef.h"
class Recipe {
public:
	Recipe();
	virtual ~Recipe();
	static void SetActiveRecipeGeneralParam(WeldParameterSetting* generalParam);
	static void SetActiveRecipeQualityParam(QualityWindowSetting* qualityParam);
	static void SetActiveRecipeAdvancedParam(AdvancedWeldSetting* advancedParam);
public:
	WeldRecipeSC m_Recipe;
};

#endif /* RECIPE_H_ */
