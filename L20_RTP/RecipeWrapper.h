#include "RecipeDefine.h"
#include <iostream>
using namespace std;
class RecipeWrapper
{
public:
	static int Set(const char* messagebuf, WeldRecipeSC* _RecipeObj);
	static int Get(WeldRecipeSC* _RecipeObj, char* messagebuf);
};
