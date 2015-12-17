#ifndef __JudgeManager_H__
#define __JudgeManager_H__

#include "cocos2d.h"
#include "LuaScriptReader.h"
#include "CharacterEntity.h"
#include "CharacterExtend.h"

USING_NS_CC;
using namespace std;

// manager Lua Engine
class JudgeManager
{
public:
	JudgeManager();
	~JudgeManager();
	string getResult(string hanzi , string points_output,CharacterEntity* p, string funcs);
	void initLuaEngine();

	/**
	* run Lua script get result from lua
	* @param hanzi Chinese character judge
	* @param points_output writing points
	* @param all_points template character points
	* @param p Character info, include all lua script
	* @param funcs not used now
	* @return
	*/
	string getResult(string hanzi , string points_output, string all_points, CharacterExtend* p , string funcs);
	void exitLuaEngine();
private:
	CLuaScriptReader gReader;
};



#endif
