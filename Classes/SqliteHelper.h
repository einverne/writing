#ifndef __SqliteHelper_H__
#define __SqliteHelper_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include "CharacterEntity.h"
#include "CharacterExtend.h"
#include "TCharacterMarkSet.h"
using namespace std;
USING_NS_CC;

class SQLiteHelper
{
public:
	/**
	* 用来创建一个db数据库 db为数据库的名字
	* @param db
	* @return
	*/
	static void initDB(const char* db);

	/**
	* 用来判断表格是否存在
	* @param name
	* @return
	*/
	static bool tableIsExist(string name);

	/**
	* 用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
	* @param sql
	* @param name
	* @return
	*/
	static void createTable(string sql,string name);

	/**
	* 用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
	* @param sql
	* @param name
	* @return
	*/
	static void deleteTable(string sql,string name);
	
	/**
	* 用来向表中插入一条数据
	* @param sql @示例语句sqlstr=" insert into MyTable(name) values( '擎天柱')"; 
	* @return 返回插入数据行的id
	*/
	static int insertData(string sql);

	/**
	* 用来向表中删除一条数据
	* @param sql @示例语句sqlstr="delete from MyTable where ID = 2"; 
	* @return
	*/
	static void deleteData(string sql);

	/**
	* 用来向表中修改一条数据
	* @param sql
	* @return
	*/
	static void updateData(string sql);

	/**
	* 获取一个记录的条数
	* @示例语句string sql = "select count(*) from user";
	* @示例语句  取得表格字段的语句string sqlsssss = "select * from user";
	* @param sql
	* @return
	*/
	static int getDataCount(string sql);

	/**
	* 读取一条记录的信息
	* @param sql
	* @param pSend
	* @return
	*/
	static void getDataInfo(string sql,CCObject* pSend);

	/**
	* 读取ziData表中一条数据
	* @param sql
	* @param p
	* @return
	*/
	static void getZiDataInfo(string sql,CharacterEntity* p);

	/**
	* 松紧规则读取
	* @param sql
	* @param p
	* @return
	*/
	static void getZiDataInfoExtend(string sql,CharacterExtend* p);

	/**
	* 读取strokeFunc表中一条数据
	* @param sql
	* @param funcbody
	* @return
	*/
	static void getstrokeFunc(string sql,CCObject* funcbody);

	/**
	* 关闭打开的数据库
	* @return
	*/
	static void closeDB();

	/**
	* 从数据groupCharacter中获取一组16个字数据
	* @param sql
	* @return vector<string>
	*/
	static vector<string> getGroupCharacter(string sql);

	/**
	* 从数据库中获取一单元数据
	* @param sql 语句
	* @return
	*/
	static string getUnit(string sql);

	/**
	* 
	* @param sql
	* @return
	*/
	static vector<string> getUnitIDs(string sql);

	/**
	* 从ziNotes中 读取数据
	* @param sql sql语句
	* @return
	*/
	static vector<vector <string> > getNote(string sql);

	/**
	* get Radical rule from radical table
	* @param sql
	* @return
	*/
	static string getRadicalRule(string sql);

	
	static void getMarkData(string sql, TCharacterMarkSet mark);
private:

};


#endif