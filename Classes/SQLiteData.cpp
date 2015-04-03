#include "SQLiteData.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
string judgepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_judge.db");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
string judgepath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_judge.db";
#endif

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
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string infodbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string infodbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
#endif
	SQLiteHelper::initDB(infodbpath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	SQLiteHelper::getZiDataInfo(sql,p);
	SQLiteHelper::closeDB();
}

string SQLiteData::getstrokeFunc(string strokeID){
	CCLog("SQLiteData::getstrokeFunc %s",strokeID.c_str());
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select * from strokeFunc where strokeID ='"+strokeID+"'";
	strokeFunc* ret = new strokeFunc();
	SQLiteHelper::getstrokeFunc(sql,ret);
	SQLiteHelper::closeDB();
	return string(ret->getFunc()->getCString());
}

void SQLiteData::getHanziDataExtend(string hz,CCObject* p){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	SQLiteHelper::getZiDataInfoExtend(sql,p);
	SQLiteHelper::closeDB();
}

bool SQLiteData::isExist(string hz){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select * from ziData where ziName ='"+hz+"'";
	int count = SQLiteHelper::getDataCount(sql);
	SQLiteHelper::closeDB();
	if (count > 0)
	{
		return true;
	}else{
		return false;
	}
	return true;
}

// vector<string> SQLiteData::getGroupCharacter(string index){
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_judge.db");
// #endif
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	string dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_judge.db";
// #endif
// 	SqliteHelper::initDB(dbpath.c_str());
// 	string sql = "select * from groupCharacter where id ='"+index+"'";
// 	vector<string> resultOfGroupCharacter = SqliteHelper::getGroupCharacter(sql);
// 	SqliteHelper::closeDB();
// 	return resultOfGroupCharacter;
// }
// 
// bool SQLiteData::updateGroupCharacter(string index, string indexOfCharacter, string character){
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_judge.db");
// #endif
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	string dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_judge.db";
// #endif
// 	SqliteHelper::initDB(dbpath.c_str());
// 	string sql = "update groupCharacter set C"+index+" = '"+character+"' where id = "+index;
// 	SqliteHelper::updateData(sql);
// 	return true;
// }

vector<string> splitSingle(string singlestring){
	string::size_type pos1,pos2;
	vector<string> strRe;
	pos2 = singlestring.find(':');
	pos1 = 0;
	while (string::npos != pos2)
	{
		strRe.push_back(singlestring.substr(pos1,pos2-pos1));
		pos1 = pos2+1;
		pos2 = singlestring.find(':',pos1);
	}
	return strRe;
}

vector< vector<string> > splitUnitString(string charstring){
	string::size_type pos1,pos2;
	vector< vector<string> > strvec;
	pos2 = charstring.find('/');
	pos1 = 0;
	while (string::npos != pos2)
	{
		strvec.push_back(splitSingle(charstring.substr(pos1,pos2-pos1)));
		pos1 = pos2 +1;
		pos2 = charstring.find('/',pos1);
	}
	return strvec;
}

string createUnitString(vector< vector<string> > unit){
	string ret;
	for (int i = 0 ; i < unit.size(); i++)
	{
		vector<string> singleChar = unit.at(i);
		for (int j = 0; j< singleChar.size(); j++)
		{
			ret += singleChar.at(j)+":";
		}
		ret += "/";
	}
	return ret;
}

vector< vector<string> > SQLiteData::getUnit(string unit_id,int count){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select * from unit where id ='"+unit_id+"'";
	string charstring = SQLiteHelper::getUnit(sql);
	vector< vector<string> > result = splitUnitString(charstring);
	vector< vector<string> > re;
	for (int i = 0 ; i < count ; ++i)
	{
		re.push_back(result.at(i));
	}
	SQLiteHelper::closeDB();
	return re;
}

bool SQLiteData::updateUnit(string unit_id,vector< vector<string> > unit){
	SQLiteHelper::initDB(judgepath.c_str());
	string unitStr = createUnitString(unit);
	string sql = "update unit set characters='"+unitStr+"' where id = '"+unit_id+"'";
	SQLiteHelper::updateData(sql);
	SQLiteHelper::closeDB();
	return true;
}

string SQLiteData::insertUnit(vector< vector<string> > unit){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "insert into unit (characters) values ('"+createUnitString(unit)+"')";
	int id = SQLiteHelper::insertData(sql);
	SQLiteHelper::closeDB();
	return DataTool::intTostring(id);
}

string SQLiteData::insertUnit(string unit_info,vector< vector<string> > unit){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "insert into unit (unit_info,characters) values ('"+unit_info+"','"+createUnitString(unit)+"')";
	int id = SQLiteHelper::insertData(sql);
	SQLiteHelper::closeDB();
	return DataTool::intTostring(id);
}

bool SQLiteData::deleteUnit(string unit_id){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "delete from unit where id = "+unit_id+"";
	SQLiteHelper::deleteData(sql);
	SQLiteHelper::closeDB();
	return true;
}

int SQLiteData::getUnitCount(){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select count(*) from unit";
	int count = SQLiteHelper::getDataCount(sql);
	SQLiteHelper::closeDB();
	return count;
}

vector<string> SQLiteData::getUnitIDs(){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "select id from unit";
	vector<string> count = SQLiteHelper::getUnitIDs(sql);
	SQLiteHelper::closeDB();
	return count;
}

string SQLiteData::insertNote(string unit_id,string zi_id,string note){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "insert into ziNotes (unitid, ziid, note) values ("+unit_id+","+zi_id+",'"+note+"')";
	int id = SQLiteHelper::insertData(sql);
	SQLiteHelper::closeDB();
	return DataTool::intTostring(id);
}

bool SQLiteData::deleteNote(string id){
	SQLiteHelper::initDB(judgepath.c_str());
	string sql = "delete from ziNotes where id = "+id+"";
	SQLiteHelper::deleteData(sql);
	SQLiteHelper::closeDB();
	return true;
}