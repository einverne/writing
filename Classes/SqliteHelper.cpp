#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "strokeFunc.h"

sqlite3 *pDB = NULL;//数据库指针 
char * errMsg = NULL;//错误信息 
std::string sqlstr;//SQL指令 
int result;//sqlite3_exec返回值		

void SqliteHelper::initDB(const char* db){
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件
	result = sqlite3_open(db,&pDB);
	if (result != SQLITE_OK)
	{
		CCLog("open sqlite failed,error:%d ，error reason:%s\n" , result, errMsg );
	}else{
		CCLog("open sqlite success~ code:%d",result);
	}
}

//tableIsExist的回调函数
int isExisted( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	bool *isExisted_=(bool*)para; 
	*isExisted_=(**column_value)!='0'; 
	return 0; 
}

bool SqliteHelper::tableIsExist( string name )
{
	if (pDB!=NULL)
	{
		//判断表是否存在
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
		result =sqlite3_exec(pDB,sqlstr.c_str(),isExisted,&tableIsExisted,&errMsg);
		return tableIsExisted;
	}
	return false;
}

//在数据库中判断名为name的表示否存在，如果不存在则创建这张表
//@示例语句string sqls = "create table user(id integer,username text,password text)";
void SqliteHelper::createTable( string sql,string name )
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加 
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	}
}

//删除表格
//@示例语句sqlstr="drop table name"; 
void SqliteHelper::deleteTable( string sql,string name )
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	}
}


//插入数据
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) "; 
void SqliteHelper::insertData( string sql ){
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
}


//删除数据
//@示例语句sqlstr="delete from MyTable_1 where ID = 2"; 
void SqliteHelper::deleteData( string sql )
{
	result=sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
}

//修改数据
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3"; 
void SqliteHelper::updateData( string sql )
{
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
}

//getDataCount的回调函数
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	int *count=(int*)para; 
	*count=n_column; 
	return 0; 
}

//获取记录的条数
//@示例语句string sqlsssss = "select count(*) from user";
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";
int SqliteHelper::getDataCount( string sql )
{
	int count=0;
	sqlite3_exec( pDB, sql.c_str() , loadRecordCount, &count, &errMsg );
	return count;
}

//getDataInfo的回调函数
int loadRecord( void * para, int n_column, char ** column_value, char ** column_name)
{ 
// 	CCLog("ID=%s,name=%s,password=%s,word=%s",column_value[0],column_value[1],column_value[2],column_value[3]);
	CCLog("ID=%s name=%s",column_value[0],column_value[1]);
	((CharacterEntity*)para)->setID(CCInteger::create(atoi(column_value[0])));
	((CharacterEntity*)para)->setName(ccs(column_value[1]));
	((CharacterEntity*)para)->setXML(ccs(column_value[2]));
	((CharacterEntity*)para)->setRules(ccs(column_value[3]));
	return 0;
}
//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据
void SqliteHelper::getDataInfo( string sql,CCObject *pSend )
{
	int ret = sqlite3_exec( pDB, sql.c_str() , loadRecord, pSend, &errMsg );
	CCLog("return getDataInfo error code:%d error:%s",ret,errMsg);
}

int loadziRecord(void * para, int n_column, char ** column_value, char ** column_name){
	((CharacterEntity*)para)->setID(CCInteger::create(atoi(column_value[0])));		//ID
	((CharacterEntity*)para)->setName(ccs(column_value[1]));						//ziName
	((CharacterEntity*)para)->setSEQ(ccs(column_value[2]));							//strokeIDSeq
	CCLog("%s",column_value[2]);
	((CharacterEntity*)para)->setRules(ccs(column_value[3]));						//rules
	((CharacterEntity*)para)->setXML(ccs(column_value[4]));							//xml
	return 0;
}

void SqliteHelper::getZiDataInfo(string sql,CCObject* p){
	int ret = sqlite3_exec(pDB , sql.c_str() , loadziRecord , p,&errMsg);
	CCLog("getZiDataInfo error code:%d error:%s",ret,errMsg);

}

int getFuncBody(void * para, int n_column, char ** column_value, char ** column_name){
	CCLog("getFuncBody");
	((strokeFunc*)para)->setFunc(ccs(column_value[2]));
	return 0;
}

void SqliteHelper::getstrokeFunc(string sql,CCObject* funcbody){
	int ret = sqlite3_exec(pDB,sql.c_str(),getFuncBody,funcbody,&errMsg);
	CCLog("SqliteHelper::getstrokeFunc error code:%d error:%s",ret,errMsg);

}

//关闭数据库
void SqliteHelper::closeDB()
{
	result = sqlite3_close(pDB); 
	if (result == SQLITE_OK)
	{
		CCLog("close database succeed");
	}else
	{
		CCLog("close database failed");
	}
}
