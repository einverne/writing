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
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("test.db");
// #endif
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	string dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"test.db";
// #endif
// 	SqliteHelper::initDB(dbpath.c_str());
// 	string sql = "select * from table_test where name='"+hanzi+"'";
// 	CharacterEntity* p = new CharacterEntity();
// 	SqliteHelper::getDataInfo(sql,p);
// 	SqliteHelper::closeDB();
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
	char * retStr = new char[50];
	gReader.SetZiName(hanzi);

//	gReader.SetRulesFunc(p->getRules());
	//À…ΩÙπÊ‘Ú
	string r = DataTool::readFromFile("setting.xml");

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