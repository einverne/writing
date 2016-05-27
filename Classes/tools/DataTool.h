#ifndef _DataTool_H_
#define _DataTool_H_

#pragma once
#include "cocos2d.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
USING_NS_CC;
using namespace std;

class DataTool
{
public:
	// convert int to string
	static string intTostring(int a);

	static int stringToInt(string str);

	/**
	* convert float to string
	* @param f
	* @return
	*/
	static string floatToString(float f);

	/**
	* convert string to float
	* @param str
	* @return
	*/
	static float stringToFloat(string str);

	/**
	* save string to file
	* @param str
	* @param filename
	* @return
	*/
	static void storeToFile(const char* str, const char* filename);
    
	/**
	* read string from file
	* @param filename
	* @return
	*/
	static string readFromFile(const char* filename);

	/**
	 * get Chinese string
	 * @param key Chinese character has been saved in xml as key and value
	 * @return
	 */
	static string getChinese(string key);

	/**
	* split string into multi strokes vector
	* @param str
	* @return
	*/
	static vector< vector<CCPoint> > spliteString(string str);

	/**
	* split string by splitSymbols into vector
	* @param str
	* @param splitSymbols
	* @return
	*/
	static vector<string> spliteStringBy(string str, string splitSymbols);

	/**
	* 检查第一个字符串中是否存在第二个字符串
	* @param phrase
	* @param sym
	* @return
	*/
	static bool isExist(string phrase, string sym);

	/**
	* copy file into SD root
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
	* output a string with n decimal points default n = 1
	* @param a_value
	* @param n
	* @return
	*/
	template <typename T>
	static string to_string_with_precision(const T a_value, const int n = 1);
};
#endif
