#include "SQLiteData.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "strokeFunc.h"

SQLiteData::SQLiteData()
{
}

SQLiteData::~SQLiteData()
{
}

void SQLiteData::getHanziData(string hz,CCObject* p){
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long int size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(dbpath.c_str(),"rb",&size);
	CCLog("file %s",pFileContent);
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
	FILE* file = fopen(dbpath.c_str(),"w");
	if (file != NULL)
	{
		CCLog("file not NULL");
		file = fopen(dbpath.c_str(),"wb");
		fwrite(pFileContent,size,1,file);
		CC_SAFE_DELETE_ARRAY(pFileContent);
	}else{
		CCLog("file NULL");
	}
	fclose(file);
#endif
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	SqliteHelper::getZiDataInfo(sql,p);
	SqliteHelper::closeDB();
}

string SQLiteData::getstrokeFunc(string strokeID){
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");
	SqliteHelper::initDB(dbpath.c_str());
	string sql = "select * from strokeFunc where strokeID ='"+strokeID+"'";
	strokeFunc* ret = new strokeFunc();
	SqliteHelper::getstrokeFunc(sql,ret);
	SqliteHelper::closeDB();
	return string(ret->getFunc()->getCString());
}