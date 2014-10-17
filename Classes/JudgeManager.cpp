#include "JudgeManager.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"
#include "tools\DataTool.h"

#include <algorithm>
using namespace std;


JudgeManager::JudgeManager()
{

}

JudgeManager::~JudgeManager()
{
	CCLog("JudgeManager destroy!");
}



string JudgeManager::getResult(string hanzi,string points_output,CharacterEntity* p, string funcs){
	CCLog("JudgeManager %s",funcs.c_str());

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");

	CLuaScriptReader gReader;

	gReader.InitLuaScriptReader();
	gReader.SetWriteZiInfo(points_output.c_str());
 	char * retStr = new char[50];
 	gReader.SetZiName(hanzi);

  	gReader.SetRulesFunc(p->getRules());

	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
	gReader.SetGlobalFunc(funcs);
 	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
 	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
 	gReader.ExitLuaScriptReader();
 	CCLog("retStr after judge %s",retStr);
	string ret(retStr);
	delete [] retStr;
	return ret;

}

string JudgeManager::getResult(string hanzi , string points_output, string all_points, CharacterExtend* p , string funcs){
	CCLog("JudgeManager %s",funcs.c_str());

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua_ext/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua_ext/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua_ext/RunAPI.lua");
	string standardpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua_ext/StandardZiInfo.lua");

	CLuaScriptReader gReader;

	gReader.InitLuaScriptReader();
	gReader.SetWriteZiInfo(points_output.c_str());
	gReader.GetStandardZiInfo(all_points);
	char * retStr = new char[50];
	retStr[0] = '\0';
	gReader.SetZiName(hanzi);

	//ËÉ½ô¹æÔò
	string r = DataTool::readFromFile("setting.xml");
	gReader.setLevel(r);
	/*gReader.setStandardZiInfo(all_points);*/
	if (r.compare("1") == 0)
	{
		gReader.SetRulesFunc(p->getruleLoose());
	}else if(r.compare("2") == 0){
		gReader.SetRulesFunc(p->getRuleTight());
	}

	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
	gReader.RunScriptFile(standardpath.c_str(),"StandardZiInfo.lua");
	gReader.SetGlobalFunc(funcs);
	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
	gReader.ExitLuaScriptReader();
	CCLog("retStr after judge %s",retStr);
	string ret(retStr);
	delete [] retStr;
	return ret;
}