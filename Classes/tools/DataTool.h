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
	//��intת��string�ַ���
	static string intTostring(int a);

	static int stringToInt(string str);

	/**
		���ַ������浽�ļ���
	*/

	static string floatToString(float f);

	static float stringToFloat(string str);

	/**
	* �����ַ������ļ�
	* @param str
	* @param filename
	* @return
	*/
	static void storeToFile(const char* str,char* filename);

	/**
	* ���ļ��ж�ȡ�ַ���
	* @param filename
	* @return
	*/
	static string readFromFile(char* filename);

	/**
	 * ��ȡ�����ַ�
	 * @param key �����ַ�����key value������xml��
	 * @return
	 */
	static string getChinese(string key);

	/**
	* ��str�ָ�� ��ʻ� ����
	* @param str
	* @return
	*/
	static vector< vector<CCPoint> > spliteString(string str);

	/**
	* ���ַ�����splitsymbol�ָ������
	* @param str
	* @param splitSymbols
	* @return
	*/
	static vector<string> spliteStringBy(string str, string splitSymbols);

	/**
	* ���ļ�������SD�� ��Ŀ¼
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
