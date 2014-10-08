
#ifndef _DataTool_H_
#define _DataTool_H_

#pragma once
#include "cocos2d.h"
using namespace std;


class DataTool
{
public:
	DataTool(void);
	~DataTool(void);

//	static string GB2312ToUTF8(string strGBK);
//	static string GBKToUTF8(string gbk);
	static string intTostring(int a);
};
#endif
