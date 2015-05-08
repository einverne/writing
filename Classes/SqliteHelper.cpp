#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "strokeFunc.h"
#include "CharacterExtend.h"

sqlite3 *pDB = NULL;//数据库指针 
char * errMsg = NULL;//错误信息 
std::string sqlstr;//SQL指令 
int result;//sqlite3_exec返回值		

void SQLiteHelper::initDB(const char* db){
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

bool SQLiteHelper::tableIsExist( string name )
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
void SQLiteHelper::createTable( string sql,string name )
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
void SQLiteHelper::deleteTable( string sql,string name )
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	}
}

int SQLiteHelper::insertData( string sql ){
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
	sqlite3_int64 id = sqlite3_last_insert_rowid(pDB);
	return id;
}

void SQLiteHelper::deleteData( string sql )
{
	result=sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
}

//修改数据
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3"; 
void SQLiteHelper::updateData( string sql )
{
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
}

//getDataCount的回调函数
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	int *count=(int*)para; 
	*count=atoi(column_value[0]);
	return 0;
}

int SQLiteHelper::getDataCount( string sql )
{
	int count=0;
	sqlite3_exec( pDB, sql.c_str() , loadRecordCount, &count, &errMsg );
	if (errMsg)
	{
		sqlite3_free(errMsg);
	}
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
void SQLiteHelper::getDataInfo( string sql,CCObject *pSend )
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

void SQLiteHelper::getZiDataInfo(string sql,CCObject* p){
	int ret = sqlite3_exec(pDB , sql.c_str() , loadziRecord , p,&errMsg);
	CCLog("getZiDataInfo error code:%d error:%s",ret,errMsg);
	if (errMsg)
	{
		sqlite3_free(errMsg);
	}
}

int loadziR(void * para, int n_column, char ** column_value, char ** column_name){
	((CharacterExtend*)para)->setID(CCInteger::create(atoi(column_value[0])));		//ID
	((CharacterExtend*)para)->setName(ccs(column_value[1]));						//ziName
	((CharacterExtend*)para)->setSEQ(ccs(column_value[2]));							//strokeIDSeq
	CCLog("%s",column_value[2]);
	((CharacterExtend*)para)->setruleLoose(ccs(column_value[3]));						//ruleLoose
	((CharacterExtend*)para)->setRuleTight(ccs(column_value[4]));
	((CharacterExtend*)para)->setXML(ccs(column_value[5]));							//xml
	return 0;
}

void SQLiteHelper::getZiDataInfoExtend(string sql,CCObject* p){
	int ret = sqlite3_exec(pDB, sql.c_str(), loadziR, p ,&errMsg);
	if (errMsg)
	{
		sqlite3_free(errMsg);
	}
}

int getFuncBody(void * para, int n_column, char ** column_value, char ** column_name){
	CCLog("getFuncBody");
	((strokeFunc*)para)->setFunc(ccs(column_value[2]));
	return 0;
}

void SQLiteHelper::getstrokeFunc(string sql,CCObject* funcbody){
	int ret = sqlite3_exec(pDB,sql.c_str(),getFuncBody,funcbody,&errMsg);
	CCLog("SqliteHelper::getstrokeFunc error code:%d error:%s",ret,errMsg);

}

int groupCharacterCallback(void * para, int n_column, char ** column_value, char ** column_name){
	vector<string>* result = (vector<string>*)para;
	for (int i=1; i < 17; i++)
	{
		result->push_back(column_value[i]);
	}
	return 0;
}

vector<string> SQLiteHelper::getGroupCharacter(string sql){
	vector<string> result;
	int ret = sqlite3_exec(pDB, sql.c_str(), groupCharacterCallback, &result, &errMsg);
	if (errMsg)
	{
		CCLog("return getDataInfo error code:%d error:%s",ret,errMsg);
		sqlite3_free(errMsg);
	}
	return result;
}

int getUnitCallback(void* para, int n_column, char** column_value, char** column_name){
	string* result = (string*)para;
	if (strcmp(column_name[2],"characters")==0)
	{
		result->append(column_value[2]);
	}
	return 0;
}

string SQLiteHelper::getUnit(string sql){
	string result;
	int ret = sqlite3_exec(pDB, sql.c_str(), getUnitCallback, &result, &errMsg);
	if (errMsg)
	{
		CCLog("return getUnit error code:%d error:%s",ret,errMsg);
		sqlite3_free(errMsg);
	}
	return result;
}

int getIDs(void* para, int n_column, char** column_value, char** column_name){
	vector<string>* ret = (vector<string>*)para;
	ret->push_back(column_value[0]);
	return 0;
}

vector<string> SQLiteHelper::getUnitIDs(string sql){
	vector<string> result;
	int ret = sqlite3_exec(pDB, sql.c_str(), getIDs, &result, &errMsg);
	if (errMsg)
	{
		CCLog("getUnitIDs error code:%d error:%s",ret,errMsg);
		sqlite3_free(errMsg);
	}
	return result;
}

int getNotes(void* para, int n_column, char** column_value, char** column_name){
	vector<vector <string> >* ret = (vector< vector <string> >*) para;
	vector<string> temp;
	if (strcmp(column_name[3],"note") == 0)
	{
		//如果第三列是note
		temp.push_back(column_value[0]);
		temp.push_back(column_value[3]);
	}
	ret->push_back(temp);
	return 0;
}

vector<vector <string> > SQLiteHelper::getNote(string sql){
	vector<vector <string> > result;
	int ret = sqlite3_exec(pDB, sql.c_str(), getNotes, &result, &errMsg);
	if (errMsg)
	{
		CCLog("getNote error code: %d error: %s",ret, errMsg);
		sqlite3_free(errMsg);
	}
	return result;
}

void SQLiteHelper::closeDB()
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
