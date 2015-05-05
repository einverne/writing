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

	/**
	* 获取数据库中汉字信息
	* @param hz
	* @param p
	* @return
	*/
	static void getHanziData(string hz,CCObject* p);

	/**
	* 获取数据中strokeFunc表信息
	* @param strokeID
	* @return
	*/
	static string getstrokeFunc(string strokeID);

	/**
	* 解析数据库中strokeIDSeq字段
	* @param seq
	* @return
	*/
	static vector<string> splitStrokeSeq(string seq);

	/**
	* 获取松紧规则的汉字
	* @param hz
	* @param p
	* @return
	*/
	static void getHanziDataExtend(string hz,CCObject* p);

	/**
	* judge a Character is exist in db
	* @param hz
	* @return
	*/
	static bool isExist(string hz);
};



#endif