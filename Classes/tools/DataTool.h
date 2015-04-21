#ifndef _DataTool_H_
#define _DataTool_H_

#pragma once
#include "cocos2d.h"
#include <iostream>
#include <string>
USING_NS_CC;
using namespace std;

class DataTool
{
public:
//	static string GB2312ToUTF8(string strGBK);
//	static string GBKToUTF8(string gbk);
	//将int转成string字符串
	static string intTostring(int a);
	/**
		将字符串保存到文件中
	*/

	static string floatToString(float f);

	static float stringToFloat(string str);

	static void storeToFile(const char* str,char* filename);
	/**
		从文件中读取字符串
	*/
	static string readFromFile(char* filename);

	/**
	 * 获取中文字符
	 * @param key 中文字符串已key value保存在xml中
	 * @return
	 */
	static string getChinese(string key);
};
#endif
