#include "DataTool.h"

string DataTool::intTostring(int a){
	stringstream s;
	s << a;
	return s.str();
}

int DataTool::stringToInt(string str){
	stringstream ss(str);
	int t = 0;
	if (! (ss >> t))
	{
		CCLog("Error convert string to int");
	}
	return t;
}

string DataTool::floatToString(float f){
	stringstream s;
	s << f;
	return s.str();
}

float DataTool::stringToFloat(string str){
	stringstream ss(str);
	float f = 0.0;
	if(!(ss >> f)){
		CCLog("Error convert string to float");
	}
	return f;
}

void DataTool::storeToFile(const char* str, const char* filename){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	string finame(filename);
	string path = CCFileUtils::sharedFileUtils()->getWritablePath()+finame;
	FILE* file = fopen(path.c_str(),"w");
	if (file != NULL)
	{
		file = fopen(path.c_str(),"wb");
		fwrite(str,strlen(str),1,file);
	}else{
        CCLog("DataTool storeToFile failed");
	}
	fclose(file);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FILE * pFile;
	pFile = fopen(filename, "wb" );
	fwrite (str, strlen(str), 1 , pFile );
	fclose (pFile);
#endif
}

string DataTool::readFromFile(const char* filename){
	string ret;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	string filepath = CCFileUtils::sharedFileUtils()->getWritablePath()+filename;
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
#endif
	unsigned long size = 0;
	unsigned char* filecontent = CCFileUtils::sharedFileUtils()->getFileData(filepath.c_str(),"r",&size);
	//read file must add two lines
	CCString* cstr = CCString::createWithData(filecontent,size);
	delete[] filecontent;
//	const char* re = cstr->getCString();
//	ret = re;
	ret = string(cstr->getCString());
	return ret;
}

string DataTool::getChinese(string key){
	CCDictionary* strings = CCDictionary::createWithContentsOfFile("fonts/chinese.xml");
	string strChinese = ((CCString*)strings->objectForKey(key))->m_sString;
//	CC_SAFE_RELEASE(strings);
	return strChinese;
}

vector< vector<CCPoint> > DataTool::spliteString(string str){
	//str   point x/ point y/x/y/x/y/@
	vector< vector<CCPoint> > ret;
	vector<string> oneStroke = DataTool::spliteStringBy(str,"@");
	vector<string>::iterator iter;
	for (iter = oneStroke.begin(); iter != oneStroke.end(); iter++)
	{
		string strokeStr = (string)*iter;
		vector<string> pointxy = DataTool::spliteStringBy(strokeStr,"/");
		vector<string>::iterator iterxy;
		vector<CCPoint> pointsStroke;
		for (iterxy = pointxy.begin(); iterxy != pointxy.end(); iterxy++)
		{
			string pointx = (string)*iterxy;
			float x = DataTool::stringToFloat(pointx);
			iterxy++;
			string pointy = (string)*iterxy;
			float y = DataTool::stringToFloat(pointy);
			CCPoint point = ccp(x,y);
			
			pointsStroke.push_back(point);
		}
		ret.push_back(pointsStroke);
	}
	return ret;
}

vector<string> DataTool::spliteStringBy(string str, string splitSymbols){
	string::size_type pos1,pos2;
	vector<string> strvec;
	pos2 = str.find(splitSymbols);
	pos1 = 0;
	while (string::npos != pos2)
	{
		strvec.push_back(str.substr(pos1,pos2-pos1));
		pos1 = pos2 +1;
		pos2 = str.find(splitSymbols,pos1);
	}
	// 	strvec.push_back(seq.substr(pos1));
	return strvec;
}

bool DataTool::isExist(string phrase, string sym){
	if (phrase.length() <= 0 || sym.length() <=0)
	{
		return false;
	}
	if (phrase.find(sym) != string::npos)
	{
		return true;
	} else
	{
		return false;
	}
}

void DataTool::copyFileToSD(string filename){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	string originpath = CCFileUtils::sharedFileUtils()->getWritablePath()+filename;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(originpath.c_str(),"rb",&size);
	string destFilePath = "/mnt/sdcard/writing/"+filename;

	FILE* file = fopen(destFilePath.c_str(),"w");
	if (file != NULL)
	{
		file = fopen(destFilePath.c_str(),"wb");
		fwrite(pFileContent,size,1,file);
		CC_SAFE_DELETE_ARRAY(pFileContent);
	}else{

	}
	fclose(file);
#endif
}

template <typename T>
string DataTool::to_string(const T a_value){
	stringstream s;
	s << a_value;
	return s.str();
}
template string DataTool::to_string<int>(const int a_value);
template string DataTool::to_string<float>(const float a_value);
template string DataTool::to_string<double>(const double a_value);

template <typename T>
string DataTool::to_string_with_precision(const T a_value, const int n){
	std::ostringstream out;
	out << std::setprecision(n) << fixed << a_value;
	return out.str();
}

template string DataTool::to_string_with_precision<float>(const float a_value, const int n);
template string DataTool::to_string_with_precision<double>(const double a_value, const int n);