#include "UTF8ToGBK.h"



UTF8ToGBK::UTF8ToGBK(void)  
{  
}  


UTF8ToGBK::~UTF8ToGBK(void)  
{  
}  


//将string转换成wstring  
wstring UTF8ToGBK::stringToWstring(const string str)  
{  

	setlocale(LC_ALL, "chs");   
	const char* _Source = str.c_str();  
	size_t _Dsize = str.size() + 1;  
	wchar_t *_Dest = new wchar_t[_Dsize];  
	wmemset(_Dest, 0, _Dsize);  
	mbstowcs(_Dest,_Source,_Dsize);  
	std::wstring result = _Dest;  
	delete []_Dest;  
	setlocale(LC_ALL, "C");  
	return result;  

}  

//当在WIN32 平台下，将utf8格式编码转化成gbk，vs2010的默认的编码格式  
string UTF8ToGBK::UTF8TOGBK(const string text)  
{  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  


	wstring tes = stringToWstring(text);  

	int asciSize = WideCharToMultiByte(CP_UTF8,0,tes.c_str(),tes.size(),NULL,0,NULL,NULL);  
	if (asciSize == ERROR_NO_UNICODE_TRANSLATION || asciSize == 0)    
	{  
		return string();  
	}  

	char *resultString = new char[asciSize];  
	int conveResult = WideCharToMultiByte(CP_UTF8,0,tes.c_str(),tes.size(),resultString,asciSize,NULL,NULL);  
	if (conveResult != asciSize)  
	{  
		return string();  
	}  
	string buffer = "";  
	buffer.append(resultString,asciSize);  

	delete[] resultString;  
	return buffer;  


#endif  

	return text;  

}  
