#ifndef __SqliteHelper_H__
#define __SqliteHelper_H__

#include "cocos2d.h"
#include "sqlite3.h"
using namespace std;
USING_NS_CC;

class SqliteHelper
{
public:
	//用来创建一个db数据库 db为数据库的名字
	static void initDB(const char* db);
	//用来判断表格是否存在
	static bool tableIsExist(string name);
	//用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
	static void createTable(string sql,string name);
	//用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
	static void deleteTable(string sql,string name);
	//用来向表中插入一条数据
	static void insertData(string sql);
	//用来向表中删除一条数据
	static void deleteData(string sql);
	//用来向表中修改一条数据
	static void updateData(string sql);
	//获取一个记录的条数
	static int getDataCount(string sql);
	//读取一条记录的信息
	static void getDataInfo(string sql,CCObject* pSend);

	//读取ziData表中一条数据
	static void getZiDataInfo(string sql,CCObject* p);

	//松紧规则读取
	static void getZiDataInfoExtend(string sql,CCObject* p);

	//读取strokeFunc表中一条数据
	static void getstrokeFunc(string sql,CCObject* funcbody);

	//关闭打开的数据库
	static void closeDB();
private:

};


#endif