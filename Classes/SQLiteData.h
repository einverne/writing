#ifndef __SQLiteData_H_
#define __SQLiteData_H_

#include "cocos2d.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "strokeFunc.h"
#include "CharacterExtend.h"
#include "tools/DataTool.h"
#include <vector>
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
	static void getHanziData(string hz,CharacterEntity* p);

	/**
	* 获取数据中strokeFunc表信息
	* @param strokeID
	* @return
	*/
//	static string getstrokeFunc(string strokeID);

	/**
	* 解析数据库中strokeIDSeq字段
	* @param seq
	* @return
	*/
	static vector<string> splitStrokeSeq(string seq);

	/**
	* 获取松紧规则的汉字
	* @param hz 汉字名 对应ziName列名
	* @param p 返回CharacterExtend 值
	* @return
	*/
	static void getHanziDataExtend(string hz,CharacterExtend* p);

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
//	static vector<string> getGroupCharacter(string index);

	/**
	* 更新一行中第indexOfCharacter个汉字
	* @param index 行号
	* @param indexOfCharacter 第几个汉字
	* @param character 汉字
	* @return true成功 false失败
	*/
//	static bool updateGroupCharacter(string index, string indexOfCharacter, string character);

	// usage of these two functions
	// 	vector<string> groupCharacter = SQLiteData::getGroupCharacter(DataTool::intTostring(0));
	//	SQLiteData::updateGroupCharacter(DataTool::intTostring(1),DataTool::intTostring(1),DataTool::getChinese("zi"));


	/**
	* 从数据库中查询index号Unit中前count个字，保存到数组桌哦你
	* @param index Unit的编号
	* @param count 该单元中前count个单词
	* @return
	*/
	static vector<vector <string> > getUnit(string unit_id,int count=16);

	/**
	* 更新一单元数据
	* @param index 单元ID
	* @param unit 单元的数组
	* @return
	*/
	static bool updateUnit(string unit_id,vector<vector <string> > unit);

	/**
	* 向数据库中插入一单元数据
	* @param unit 包含的unit的数据
	* @return 返回该Unit的ID
	*/
	static string insertUnit(vector<vector <string> > unit);

	/**
	* 向数据库中插入一单元数据
	* @param unit_info 该单元的基本信息介绍
	* @param unit 包含的unit的数据
	* @return 返回该Unit的ID
	*/
    static string insertUnit(string unit_info,vector<vector <string> > unit);

	/**
	* 删除数据库中unit id的单元数据
	* @param unit_id
	* @return
	*/
	static bool deleteUnit(string unit_id);

	/**
	* 获取Unit的数量
	* @return
	*/
	static int getUnitCount();

	/**
	* 获取数据库中单元ID
	* @return
	*/
	static vector<string> getUnitIDs();

	/**
	* 向数据库中插入一条笔记记录
	* @param unit_id	单元id
	* @param zi_id		汉字id
	* @param note		笔记 点数据信息
	* @return			返回插入的行id
	*/
	static string insertNote(string unit_id,string zi_id,string note);

	/**
	* 从数据库中删除一条记录
	* @param id	记录的id
	* @return
	*/
	static bool deleteNote(string id);

	/**
	* 从数据库 ziNotes 表中读取 unit_id 并且 ziid 的手写汉字值
	* @param unit_id 单元ID
	* @param zi_id 字ID，每个汉字的ID是唯一的
	* @return vector<vector <string> >
	*/
	static vector<vector <string> > getNote(string unit_id, string zi_id);

	static vector<string> getRadicalRules(vector<string> ids);
};



#endif
