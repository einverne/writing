#ifndef __SQLiteData_H_
#define __SQLiteData_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class SQLiteData
{
public:

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
	* @param hz 汉字名 对应ziName列名
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

	/**
	* 获取数据库中一行16个字，返回数组
	* @param index id号，例如1
	* @return vector<string>
	*/
	static vector<string> getGroupCharacter(string index);

	/**
	* 更新一行中第indexOfCharacter个汉字
	* @param index 行号
	* @param indexOfCharacter 第几个汉字
	* @param character 汉字
	* @return true成功 false失败
	*/
	static bool updateGroupCharacter(string index, string indexOfCharacter, string character);

	/**
	* 从数据库中查询index号Unit中前count个字，保存到数组桌哦你
	* @param index Unit的编号
	* @param count 该单元中前count个单词
	* @return
	*/
	static vector<vector<string>> getUnit(string index,int count=16);

};



#endif