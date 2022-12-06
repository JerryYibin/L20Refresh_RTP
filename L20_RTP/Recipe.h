/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 The file Recipe.h is a public interface that provides external.

 **********************************************************************************************************/
#ifndef RECIPE_H_
#define RECIPE_H_
#include <vector>
#include "RecipeDefine.h"
using namespace std;
class Recipe {
public:
	enum STEPMODE_DATA_TYPE
	{
		ORDER = 0,
		STEPVALUE,
		AMPLITUDE,
		TOTAL
	};
	static shared_ptr<WeldRecipeSC> ActiveRecipeSC;
	static shared_ptr<WeldRecipeSC> RecipeSC;
	static vector<WeldRecipeLibForUI>	WeldRecipeLibraryForUI;
public:
	Recipe();
	virtual ~Recipe();
	static void SetActiveRecipeGeneralParam(WeldParameterSetting* generalParam);
	static void SetActiveRecipeQualityParam(QualityWindowSetting* qualityParam);
	static void SetActiveRecipeAdvancedParam(AdvancedWeldSetting* advancedParam);
public:
	//WeldRecipeSC m_Recipe;
};

#endif /* RECIPE_H_ */
