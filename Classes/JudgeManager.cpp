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

void JudgeManager::initLuaEngine(){
	gReader.InitLuaScriptReader();
	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI_1208.lua");
	string standardpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/StandardZiInfo.lua");
	string JSONpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/JSON.lua");
	gReader.RunScriptFile(JSONpath.c_str(),"JSON.lua");
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	gReader.RunScriptFile(standardpath.c_str(),"StandardZiInfo.lua");
 	gReader.RunScriptFile(basepath.c_str(),"BaseLib.lua");
	gReader.RunScriptFile(apipath.c_str(),"RunAPI_1208.lua");

}

string JudgeManager::getResult(string hanzi , string points_output, string all_points, CharacterExtend* p , string funcs){
	CCLog("JudgeManager %s",funcs.c_str());
	string Mainpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/Main.lua");

	gReader.setWriteZiInfo(points_output.c_str());
	gReader.setStandardZiInfo(all_points);
	char retStr[200];
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

	//set Unit rule
	string unitrule = p->getRuleUnit();
	gReader.setUnitRule(unitrule);

	//set zi rule
	string zirule = p->getRuleZi();
	gReader.setZiRule(zirule);

	gReader.RunScriptFile(Mainpath.c_str(),retStr,"Main.lua");

	CCLog("retStr after judge %s",retStr);
	string ret = retStr;
	return ret;
}

void JudgeManager::exitLuaEngine(){
	gReader.ExitLuaScriptReader();
}