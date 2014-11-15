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
	static void getHanziData(string hz,CCObject* p);
	static string getstrokeFunc(string strokeID);
	static vector<string> splitStrokeSeq(string seq);
	
	static void insertMarkTable(string strokeid, string strokeStr, string markResult);
};



#endif