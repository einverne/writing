#pragma once
 

#include <iostream>  
#include <stdio.h>  

//string是c++ 的头文件，其内包含了一个string类，string s1就是建立一个string类的对象   

#include <string>  

//cstring.h 是对应于旧C 头文件的std 版本  
#include <cstringt.h>  

#include <locale>  

using namespace std;  

class UTF8ToGBK  
{  
public:  
	UTF8ToGBK(void);  
	~UTF8ToGBK(void);  


	//将unicode编码的string转换成wstring  
	static wstring stringToWstring(const string text);  

	//将utf8格式编码转化成gbk，vs2010的默认的编码格式  
	static string UTF8TOGBK(const string text);  

}; 

