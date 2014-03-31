#include "JudgeManager.h"

JudgeManager::JudgeManager(string hanzi)
{
	this->hanzi = hanzi;
}

JudgeManager::~JudgeManager()
{
	CCLog("JudgeManager destroy!");
}
string JudgeManager::getResult(string points_output){
	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string Globalpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/ZiList/°Ë/funcs.txt");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");
	string rulespath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/ZiList/°Ë/rules.txt");

	gReader.InitLuaScriptReader();
	char* temp = new char[points_output.length() + 1];
	strcpy(temp,points_output.c_str());
	gReader.GetWriteZiInfo(temp);
	delete [] temp;
 	char * retStr = new char[50];
 	gReader.GetZiName(hanzi);
 	gReader.SetRulesFunc(rulespath.c_str());
 	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
 	gReader.SetGlobalFunc(Globalpath.c_str());
 	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
 	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
 	gReader.ExitLuaScriptReader();
	string ret(retStr);
	delete [] retStr;
	return ret;
	//return string("1");
}