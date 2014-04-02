#include "JudgeManager.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "UTF8ToGBK.h"

JudgeManager::JudgeManager(string hanzi)
{
	this->hanzi = hanzi;
}

JudgeManager::~JudgeManager()
{
	CCLog("JudgeManager destroy!");
}
string JudgeManager::getResult(string points_output){
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("test.db");
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from table_test where name='"+hanzi+"'";
	CharacterEntity* p = new CharacterEntity();
	SqliteHelper::getDataInfo(sql,p);
	SqliteHelper::closeDB();
	string path = "lua/ZiList/"+to_string(p->getID()->getValue());

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string Globalpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(string(path+"/funcs.txt").c_str());
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");
	string rulespath = CCFileUtils::sharedFileUtils()->fullPathForFilename(string(path+"/rules.txt").c_str());

	gReader.InitLuaScriptReader();
	char* temp = new char[points_output.length() + 1];
	strcpy(temp,points_output.c_str());
	gReader.GetWriteZiInfo(temp);
	delete [] temp;
 	char * retStr = new char[50];
 	gReader.GetZiName(hanzi);
 	gReader.SetRulesFunc(rulespath.c_str());
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	gReader.SetGlobalFunc(Globalpath.c_str());
 	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
 	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
 	gReader.ExitLuaScriptReader();
	string ret(retStr);
	delete [] retStr;
	return ret;
	//return string("1");
}