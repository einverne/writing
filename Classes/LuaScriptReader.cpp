// LuaScriptReader.cpp : Defines the entry point for the DLL application.


#include "LuaScriptReader.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

string StandardZiInfo;                  //用于接收标准字信息
string WriteZiInfo;                     //手写字信息
string Hanzi;
string UnitRule;						//部件Lua
string ZiRule;							//整字Lua
string GlobalFunc;                      //全局函数信息
string Rules;							//规则信息
string Level;

// This is the constructor of a class that has been exported.
// see LuaScriptReader.h for the class definition
CLuaScriptReader::CLuaScriptReader():m_plua(NULL)
{
	m_plua = NULL;
	return; 
}

CLuaScriptReader::~CLuaScriptReader(){
	
}

int GetWriteInfoFromC(lua_State *plua){
	lua_pushstring(plua, WriteZiInfo.c_str());
	return 1;    
}

int GetStandardZiInfoFromC(lua_State *plua){
	lua_pushstring(plua, StandardZiInfo.c_str());
	return 1;    
}

int GetGlobalFuncFromC(lua_State *plua){
	lua_pushstring(plua, GlobalFunc.c_str());
	return 1;    
}

int GetZiNameFromC(lua_State *plua){
	lua_pushstring(plua, Hanzi.c_str());
	return 1;
}

int GetRulesFromC(lua_State *plua){
	lua_pushstring(plua, Rules.c_str());
	return 1;
}

int GetStrokeLevelFromC(lua_State *plua){
	lua_pushstring(plua, Level.c_str());
	return 1;
}

int GetUnitRulesFromC(lua_State *plua) {
	lua_pushstring(plua, UnitRule.c_str());
	return 1;
}

int GetZiRulesFromC(lua_State* plua){
	lua_pushstring(plua, ZiRule.c_str());
	return 1;
}

bool CLuaScriptReader::InitLuaScriptReader(){
	if( m_plua == NULL ){
        m_plua = luaL_newstate();
        luaL_openlibs(m_plua);
		if( m_plua == NULL ) return false;
		lua_register(m_plua, "GetWriteInfoFromC", GetWriteInfoFromC);
		lua_register(m_plua, "GetStandardZiInfoFromC", GetStandardZiInfoFromC);
		lua_register(m_plua,"GetGlobalFuncFromC",GetGlobalFuncFromC);
		lua_register(m_plua,"GetZiNameFromC",GetZiNameFromC);
		lua_register(m_plua,"GetRulesFromC",GetRulesFromC);
		lua_register(m_plua,"GetStrokeLevelFromC",GetStrokeLevelFromC);
		lua_register(m_plua,"GetUnitRulesFromC",GetUnitRulesFromC);
		lua_register(m_plua,"GetZiRulesFromC",GetZiRulesFromC);
	}
	return true;
}


void CLuaScriptReader::ExitLuaScriptReader(){
	if( m_plua != NULL ){
		lua_close(m_plua);
		m_plua = NULL;
	}
}

bool CLuaScriptReader::RunScriptBuffer(const char *buff,const char *name){
	int error = 0;
	char callname[256] = "";

	if( m_plua == NULL || buff == NULL ) return false;
	if( *name == 0 ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}
	error = luaL_loadbuffer(m_plua, buff, strlen(buff),callname) || lua_pcall(m_plua, 0, 0, 0);
	if (error){
		fprintf(stderr, "%s", lua_tostring(m_plua, -1));
		CCLog("%s\n", lua_tostring(m_plua, -1));
		lua_pop(m_plua, 1);
	}else{
		//CCLog("luaL_loadbuffer2 success");
	}
	return true;
}

bool CLuaScriptReader::RunScriptBuffer(const char *buff,char* ret_string,const char *name){
	int error = 0;
	char callname[256] = "";
	if( m_plua == NULL || buff == NULL || ret_string == NULL ) return false;
	if( name == NULL ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}
	error = luaL_loadbuffer(m_plua, buff, strlen(buff),callname) || lua_pcall(m_plua, 0, 1, 0);
	if (error){
		fprintf(stderr, "%s\n", lua_tostring(m_plua, -1));
		CCLog("%s\n",lua_tostring(m_plua, -1));
		lua_pop(m_plua, 1);
	}else{
 		sprintf(ret_string, "%s", lua_tostring(m_plua, -1));
	}
	return true;
}

// void CLuaScriptReader::SetGlobalFunc(const char * filename){
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	FILE* fpFile = fopen(filename,"r");
// 	if (fpFile == NULL)
// 	{
// 		printf("cannot open file");
// 		return;
// 	}
// 	char line[1024] = "";
// 	while (fgets(line, 1024,fpFile) != NULL)
// 	{
// 		strcat(GlobalFunc,line);
// 	}
// #endif
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 		unsigned long int size =0;
// 		unsigned char* temp = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
// 		CCString* ccStr = CCString::createWithData(temp,size);
// 		strcpy(GlobalFunc,ccStr->getCString());
// // 		CCLog("GlobalFunc %s",GlobalFunc);
// #endif
// 	return;
// }

void CLuaScriptReader::setGlobalFunc(string funcs){
	GlobalFunc = funcs;
}

// void CLuaScriptReader::SetRulesFunc(const char* filename){
// 	CCLog("SetRulesFunc(const char* filename)");
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	FILE* fpFile = fopen(filename,"r");
// 	if (fpFile == NULL)
// 	{
// 		printf("cannot open file");
// 		return;
// 	}
// 	char line[1024] = "";
// 	while (fgets(line, 1024,fpFile) != NULL)
// 	{
// 		strcat(Rules,line);
// 	}
// #endif
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	unsigned long int size = 0;
// 	unsigned char* temp = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
// 	CCString* ccStr = CCString::createWithData(temp,size);
// 	strcpy(Rules,ccStr->getCString());
// 	CCLog("Rules %s",Rules);
// #endif
// 	return;
// }
// 
void CLuaScriptReader::setRulesFunc(string rules){
	Rules = rules.c_str();
}


bool CLuaScriptReader::RunMixedFile(const char *filename,const char *name)
{
	char callname[256] = "";
	if (m_plua == NULL || filename == NULL ) return false;
	if( name == NULL ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	strcat(filebuff,"\n");
	strcat(filebuff,GlobalFunc.c_str());
	RunScriptBuffer(filebuff,callname);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM ==CC_PLATFORM_IOS
	unsigned long size = 0;
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	if (!filebuff)
	{
		delete[] filebuff;
	}
	char* filebuff1 = (char*)ccStr->getCString();
	strcat(filebuff1,"\n");
	strcat(filebuff1,GlobalFunc.c_str());
	RunScriptBuffer(filebuff1,callname);
#endif
	return true;
}

bool CLuaScriptReader::RunScriptFile(const char *filename,char* ret_string,const char *name){
	if( m_plua == NULL || ret_string == NULL || filename == NULL ) return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	RunScriptBuffer(filebuff,ret_string,name);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM ==CC_PLATFORM_IOS
	unsigned long size = 0;
	CCLog("filepath name %s",filename);
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"rb",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	if (!filebuff)
	{
		delete[] filebuff;
	}
	CCLog("ccstr end");
	RunScriptBuffer(ccStr->getCString(),ret_string,name);
#endif
	return true;
}


bool CLuaScriptReader::RunScriptFile(const char *filename,const char* name){
	if( m_plua == NULL || filename == NULL ) return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	RunScriptBuffer(filebuff,name);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM ==CC_PLATFORM_IOS
	unsigned long size = 0;
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	if (!filebuff)
	{
		delete[] filebuff;
	}
	RunScriptBuffer(ccStr->getCString(),name);
#endif
	return true;
}

bool CLuaScriptReader::setWriteZiInfo(const char* wz){
	WriteZiInfo = string(wz);
	return true;
}
void CLuaScriptReader::setZiName(string hanzi){
	Hanzi = hanzi;
	return;
}

void CLuaScriptReader::setLevel(string level){
	Level = level;
	return;
}


bool CLuaScriptReader::setStandardZiInfo(string stdinfo){
	StandardZiInfo = stdinfo;
	return true;
}