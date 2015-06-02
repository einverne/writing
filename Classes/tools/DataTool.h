#ifndef _DataTool_H_
#define _DataTool_H_

#pragma once
#include "cocos2d.h"
#include <iostream>
#include <iomanip>
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

	static int stringToInt(string str);

	/**
		将字符串保存到文件中
	*/

	static string floatToString(float f);

	static float stringToFloat(string str);

	/**
	* 保存字符串到文件
	* @param str
	* @param filename
	* @return
	*/
	static void storeToFile(const char* str,char* filename);

	/**
	* 从文件中读取字符串
	* @param filename
	* @return
	*/
	static string readFromFile(char* filename);

	/**
	 * 获取中文字符
	 * @param key 中文字符串已key value保存在xml中
	 * @return
	 */
	static string getChinese(string key);

	/**
	* 将str分割成 多笔画 序列
	* @param str
	* @return
	*/
	static vector< vector<CCPoint> > spliteString(string str);

	/**
	* 将字符串以splitsymbol分割成数组
	* @param str
	* @param splitSymbols
	* @return
	*/
	static vector<string> spliteStringBy(string str, string splitSymbols);

	/**
	* 将文件拷贝到SD卡 根目录
	* @param filename
	* @return
	*/
	static void copyFileToSD(string filename);

	/**
	* convert a_value int float double to string
	* @param a_value
	* @return
	*/
	template <typename T>
	static string to_string(const T a_value);

	/**
	* output a string with n decimal points default n = 2
	* @param a_value
	* @param n
	* @return
	*/
	template <typename T>
	static string to_string_with_precision(const T a_value, const int n = 2);
};
#endif
