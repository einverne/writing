
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

	string GB2312ToUTF8(string strGBK);
};
#endif
