#ifndef __JudgeManager_H__
#define __JudgeManager_H__

#include "cocos2d.h"
#include "LuaScriptReader.h"
#include "CharacterEntity.h"
USING_NS_CC;

class JudgeManager
{
public:
	JudgeManager(string hanzi);
	~JudgeManager();
	string getResult(string points_output);

	CLuaScriptReader gReader;
	string hanzi;
private:
	string funcs;
	CharacterEntity* p;
};



#endif
