#ifndef __SQLiteData_H_
#define __SQLiteData_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class SQLiteData
{
public:
	SQLiteData();
	~SQLiteData();
	/*
		获取数据库中汉字信息
	*/
	static void getHanziData(string hz,CCObject* p);
	/*
		获取数据中strokeFunc表信息
	*/
	static string getstrokeFunc(string strokeID);
	/*
		解析数据库中strokeIDSeq字段
	*/
	static vector<string> splitStrokeSeq(string seq);
	/*
		获取松紧规则的汉字
	*/
	static void getHanziDataExtend(string hz,CCObject* p);
};



#endif