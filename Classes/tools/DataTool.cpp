#include "DataTool.h"

using namespace std;
USING_NS_CC;

DataTool::DataTool(void)
{
}


DataTool::~DataTool(void)
{
}

//string DataTool::GB2312ToUTF8(string strGBK){
//	string strOutUTF8 = "";
//
//#if WIN32
//
//	WCHAR * str1;
//
//	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
//
//	str1 = new WCHAR[n];
//
//	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
//
//	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
//
//	char * str2 = new char[n];
//
//	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
//
//	strOutUTF8 = str2;
//
//	delete[]str1;
//
//	str1 = NULL;
//
//	delete[]str2;
//
//	str2 = NULL;
//
//#else
//
//	strOutUTF8 = strGBK;
//
//#endif
//
//	return strOutUTF8;
//}
//
//
////UTF8、GBK之间的转换
//string DataTool::GBKToUTF8(string gbk)
//{
//	//建立一块内存块
//	char *szOut=new char[gbk.size()+2];
//	//将内存全设为0;
//	memset(szOut,0,gbk.size()+2);
//	//拷贝，相识于Strcpy
//	memcpy(szOut,gbk.c_str(),strlen(gbk.c_str()));
//	char* strGBK = szOut;
//	//映射一个字符串到一个宽字符（unicode）的字符串
//	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, NULL,0);
//	unsigned short * wszUtf8 = new unsigned short[len+1];
//	memset(wszUtf8, 0, len * 2 + 2);
//	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, (LPWSTR)wszUtf8, len);
//	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
//	char *szUtf8=new char[len + 1];
//	memset(szUtf8, 0, len + 1);
//	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);
//	string result=szUtf8;
//	delete[] szUtf8;
//	delete[] wszUtf8;
//	delete[] szOut;
//	return result;
//}

string DataTool::intTostring(int a){
	stringstream s;
	s << a;
	return s.str();
}

void DataTool::storeToFile(char* str,char* filename){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	string finame(filename);
	string path = CCFileUtils::sharedFileUtils()->getWritablePath()+finame;
	FILE* file = fopen(path.c_str(),"w");
	if (file != NULL)
	{
		file = fopen(path.c_str(),"wb");
		fwrite(str,strlen(str),1,file);
	}else{
		// 		CCLog("CLuaScriptReader::Print2File file NULL");
	}
	fclose(file);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FILE * pFile;

	pFile = fopen (filename, "wb" );
	fwrite (str, strlen(str), 1 , pFile );
	fclose (pFile);
#endif
}

string DataTool::readFromFile(char* filename){
	string ret;
	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	unsigned long size = 0;
	char* filecontent = (char*)CCFileUtils::sharedFileUtils()->getFileData(filepath.c_str(),"r",&size);
	ret = filecontent;
	return ret;
}