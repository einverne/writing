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
	gReader.setWriteZiInfo(points_output.c_str());
 	char * retStr = new char[50];
 	gReader.setZiName(hanzi);

  	gReader.setRulesFunc(p->getRules());

	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
	gReader.setGlobalFunc(funcs);
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
	gReader.setWriteZiInfo(points_output.c_str());
	gReader.setStandardZiInfo(all_points);
/*	char * retStr = new char[50];*/
	char retStr[50];
	retStr[0] = '\0';
 	gReader.setZiName(hanzi);

	//ËÉ½ô¹æÔò
	string r = DataTool::readFromFile("setting.xml");
 	gReader.setLevel(r);
 	if (r.compare("1") == 0)
 	{
 		gReader.setRulesFunc(p->getruleLoose());
 	}else if(r.compare("2") == 0){
 		gReader.setRulesFunc(p->getRuleTight());
 	}

// 	DataTool::storeToFile(funcs.c_str(),"func.txt");
//	CCLog("setGlobalFunc %s",funcs.c_str());
//	gReader.setGlobalFunc(funcs);

 	CCLog("WriteZiInfo");
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	CCLog("StandardZiInfo");
 	gReader.RunScriptFile(standardpath.c_str(),"StandardZiInfo.lua");

 	CCLog("baselib");
// 	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
 	gReader.RunScriptFile(basepath.c_str(),"BaseLib.lua");

 	CCLog("runapi");
	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
//	gReader.RunScriptFile(apipath.c_str(),"RunAPI.lua");

	CCLog("ExitLuaScriptReader");
	gReader.ExitLuaScriptReader();
	CCLog("retStr after judge %s",retStr);
	string ret = retStr;
//	delete [] retStr;
	return ret;
//	return string("1\r\n");
}
