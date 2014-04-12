#include "JudgeManager.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"

JudgeManager::JudgeManager(string hanzi)
{
	this->hanzi = hanzi;
}

JudgeManager::~JudgeManager()
{
	CCLog("JudgeManager destroy!");
}
string JudgeManager::getResult(string points_output){
//	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("test.db");
	string dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"test.db";
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from table_test where name='"+hanzi+"'";
	CharacterEntity* p = new CharacterEntity();
	SqliteHelper::getDataInfo(sql,p);
	SqliteHelper::closeDB();
	stringstream ss;
	ss << p->getID()->getValue();
	string path = "lua/ZiList/"+ ss.str();
	CCLog("path %s",path.c_str());

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string Globalpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(string(path+"/funcs.txt").c_str());
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");
	string rulespath = CCFileUtils::sharedFileUtils()->fullPathForFilename(string(path+"/rules.txt").c_str());

	gReader.InitLuaScriptReader();
//	char* temp = new char[points_output.length() + 1];
//	strcpy(temp,points_output.c_str());
//	gReader.GetWriteZiInfo(temp);
//	delete [] temp;
	gReader.GetWriteZiInfo(points_output.c_str());
 	char * retStr = new char[50];
 	gReader.GetZiName(hanzi);
 	gReader.SetRulesFunc(rulespath.c_str());
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	gReader.SetGlobalFunc(Globalpath.c_str());
 	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
 	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
 	gReader.ExitLuaScriptReader();
 	CCLog("retStr %s",retStr);
	string ret(retStr);
	delete [] retStr;
	return ret;
	//return string("1");
}
