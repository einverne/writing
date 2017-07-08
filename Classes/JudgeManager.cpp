#include "JudgeManager.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"
#include "tools/DataTool.h"

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

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");

	CLuaScriptReader gReader;

	gReader.InitLuaScriptReader();
	gReader.setWriteZiInfo(points_output.c_str());
 	char * retStr = new char[50];
 	gReader.setZiName(hanzi);

    string luas = p->getRules();
  	gReader.setRulesFunc(luas);

    const char* writeziinfo = "WriteZiInfo.lua";
	gReader.RunScriptFile(filepath.c_str(),writeziinfo);
	gReader.setGlobalFunc(funcs);
    const char* baselua = "BaseLib.lua";
 	gReader.RunMixedFile(basepath.c_str(),baselua);
    const char* runapilua = "RunAPI.lua";
 	gReader.RunScriptFile(apipath.c_str(),retStr,runapilua);
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
	char retStr[50];
	retStr[0] = '\0';
 	gReader.setZiName(hanzi);

	// get easy or hard setting from setting.xml config file
    const char* settingname = "setting.xml";
	string r = DataTool::readFromFile(settingname);
 	gReader.setLevel(r);
 	if (r.compare("1") == 0)
 	{
        string looselua = p->getRuleLoose();
 		gReader.setRulesFunc(looselua);
 	}else if(r.compare("2") == 0){
        string tightlua = p->getRuleTight();
 		gReader.setRulesFunc(tightlua);
 	}

 	CCLog("WriteZiInfo");
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	CCLog("StandardZiInfo");
 	gReader.RunScriptFile(standardpath.c_str(),"StandardZiInfo.lua");

 	CCLog("baselib");
 	gReader.RunScriptFile(basepath.c_str(),"BaseLib.lua");

 	CCLog("runapi");
	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");

//	gReader.ExitLuaScriptReader();
	CCLog("retStr after judge %s",retStr);
	string ret = retStr;
//	delete [] retStr;
	return ret;
//	return string("1\r\n");
}
