#include "JudgeManager.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"

#include <algorithm>
using namespace std;


vector<string> splitStrokeSeq(string seq){
	string::size_type pos1,pos2;
	vector<string> strvec;
	pos2 = seq.find('-');
	pos1 = 0;
	while (string::npos != pos2)
	{
		strvec.push_back(seq.substr(pos1,pos2-pos1));
		pos1 = pos2 +1;
		pos2 = seq.find('-',pos1);
	}
// 	strvec.push_back(seq.substr(pos1));
	return strvec;
}

JudgeManager::JudgeManager(string hanzi)
{
	this->hanzi = hanzi;

	p = new CharacterEntity();
	SQLiteData::getHanziData(hanzi,p);
	stringstream ss;
	ss << p->getID()->getValue();
	string path = "lua/ZiList/"+ ss.str();
	CCLog("path %s",path.c_str());
	CCLog("rules %s",p->getRules()->getCString());


	CCString* temp = p->getSEQ();
	CCLog("seq %s",temp->getCString());
	string str(temp->getCString());
	vector<string> strvec = splitStrokeSeq(str);
	//ШЅжи
	std::sort(strvec.begin(),strvec.end());
	strvec.erase(std::unique(strvec.begin(),strvec.end()),strvec.end());
	vector<string>::iterator iter = strvec.begin(),iter2 = strvec.end();

	while (iter != iter2)
	{
		funcs += SQLiteData::getstrokeFunc(*iter);
		iter ++;
	}
}

JudgeManager::~JudgeManager()
{
	CCLog("JudgeManager destroy!");
}



string JudgeManager::getResult(string points_output){
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

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");

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
