#ifndef __JudgeManager_H__
#define __JudgeManager_H__

#include "cocos2d.h"
#include "LuaScriptReader.h"
#include "CharacterEntity.h"
#include "CharacterExtend.h"

USING_NS_CC;
using namespace std;

class JudgeManager
{
public:
	JudgeManager();
	~JudgeManager();
	static string getResult(string hanzi , string points_output,CharacterEntity* p, string funcs);
	static string getResult(string hanzi , string points_output, string all_points, CharacterExtend* p , string funcs);
};



#endif
