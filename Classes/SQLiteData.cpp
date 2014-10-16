#include "SQLiteData.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "strokeFunc.h"
#include "CharacterExtend.h"

SQLiteData::SQLiteData()
{
}

SQLiteData::~SQLiteData()
{
}

vector<string> SQLiteData::splitStrokeSeq(string seq){
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


void SQLiteData::getHanziData(string hz,CCObject* p){
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
#endif
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	SqliteHelper::getZiDataInfo(sql,p);
	SqliteHelper::closeDB();
}

string SQLiteData::getstrokeFunc(string strokeID){
	CCLog("SQLiteData::getstrokeFunc %s",strokeID.c_str());
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
#endif
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from strokeFunc where strokeID ='"+strokeID+"'";
	strokeFunc* ret = new strokeFunc();
	SqliteHelper::getstrokeFunc(sql,ret);
	SqliteHelper::closeDB();
	return string(ret->getFunc()->getCString());
}

void SQLiteData::getHanziDataExtend(string hz,CCObject* p){
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_judge.db");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_judge.db";
#endif
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	SqliteHelper::getZiDataInfo(sql,p);
	SqliteHelper::closeDB();
}