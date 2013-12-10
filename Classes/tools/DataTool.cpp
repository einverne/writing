#include "DataTool.h"

using namespace std;

DataTool::DataTool(void)
{
}


DataTool::~DataTool(void)
{
}

string DataTool::GB2312ToUTF8(string strGBK){
	string strOutUTF8 = "";

#if WIN32

	WCHAR * str1;  

	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  

	str1 = new WCHAR[n];  

	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  

	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  

	char * str2 = new char[n];  

	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  

	strOutUTF8 = str2;  

	delete[]str1;  

	str1 = NULL;  

	delete[]str2;  

	str2 = NULL;

#else

	strOutUTF8 = strGBK;

#endif

	return strOutUTF8;
}